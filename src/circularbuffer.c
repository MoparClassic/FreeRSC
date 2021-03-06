#include "circularbuffer.h"
#include "util.h"

#include <assert.h>
#include <string.h>
#include <sys/socket.h>

const int BIT_MASK[] = {
    0, 0x1, 0x3, 0x7,
    0xf, 0x1f, 0x3f, 0x7f,
    0xff, 0x1ff, 0x3ff, 0x7ff,
    0xfff, 0x1fff, 0x3fff, 0x7fff,
    0xffff, 0x1ffff, 0x3ffff, 0x7ffff,
    0xfffff, 0x1fffff, 0x3fffff, 0x7fffff,
    0xffffff, 0x1ffffff, 0x3ffffff, 0x7ffffff,
    0xfffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff, -1
};

int cbuffer_create_frame(cbuffer_t *cbuf, uint32_t opcode)
{
    assert(cbuf);
    assert(opcode >= 0 && opcode <= 255);

    if (cbuf->start_offset > (BUFFER_SIZE * 4) / 5) {
        printf("cbuffer_create_frame flushing buffer\n");
        cbuffer_send(cbuf);
    }

    cbuf->buffer[cbuf->start_offset + 2] = (uint8_t) opcode;
    cbuf->buffer[cbuf->start_offset + 3] = 0;
    cbuf->write_offset = cbuf->start_offset + 3;
    return 1;
}

int cbuffer_mark_read_position(cbuffer_t *cbuf)
{
    assert(cbuf);

    cbuf->read_marker = cbuf->read_offset;
    return 1;
}

int cbuffer_rewind_read_position(cbuffer_t *cbuf)
{
    assert(cbuf);

    cbuf->read_offset = cbuf->read_marker;
    return 1;
}

int cbuffer_available(cbuffer_t *cbuf)
{
    assert(cbuf);

    if (cbuf->write_offset >= cbuf->read_offset) {
        return cbuf->write_offset - cbuf->read_offset;
    }
    else {
        return cbuf->write_offset + BUFFER_SIZE - cbuf->read_offset;
    }
}

int cbuffer_format_packet(cbuffer_t *cbuf)
{
    int size;

    assert(cbuf); /* cbuf must not be a NULL pointer */

    size = cbuf->write_offset - cbuf->start_offset - 2;
    if (size >= 160) {
        cbuf->buffer[cbuf->start_offset] = (uint8_t) (160 + size / 256);
        cbuf->buffer[cbuf->start_offset + 1] = (uint8_t) size;
    }
    else {
        cbuf->buffer[cbuf->start_offset] = (uint8_t) size;
        --cbuf->write_offset;
        cbuf->buffer[cbuf->start_offset + 1] = (uint8_t) cbuf->buffer[cbuf->write_offset];
    }
    cbuf->start_offset = cbuf->write_offset;
    return 1;
}

int cbuffer_send(cbuffer_t *cbuf)
{
    assert(cbuf); /* cbuf must not be a NULL pointer */

    send(cbuf->fd, cbuf->buffer, cbuf->write_offset, 0);
    cbuf->start_offset = 0;
    cbuf->write_offset = 3;
    return 1;
}

int cbuffer_send_data(cbuffer_t *cbuf, void *data, size_t sz)
{
    assert(cbuf); /* cbuf must not be a NULL pointer */
    assert(data); /* data must not be a NULL pointer */

    send(cbuf->fd, data, sz, 0);
    return 1;
}

int cbuffer_write_byte(cbuffer_t *cbuf, uint8_t value)
{
    assert(cbuf);

    cbuf->buffer[cbuf->write_offset++] = value;
    return 1;
}

int cbuffer_write_short(cbuffer_t *cbuf, uint16_t value)
{
    int ret;

    assert(cbuf);

    ret = cbuffer_write_byte(cbuf, (uint8_t) (value >> 8));
    cbuffer_write_byte(cbuf, (uint8_t) value);
    return ret;
}

int cbuffer_write_int(cbuffer_t *cbuf, uint32_t value)
{
    assert(cbuf);

    cbuffer_write_byte(cbuf, (uint8_t) (value >> 24));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 16));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 8));
    cbuffer_write_byte(cbuf, (uint8_t) value);
    return 1;
}

int cbuffer_write_long(cbuffer_t *cbuf, uint64_t value)
{
    assert(cbuf);

    cbuffer_write_byte(cbuf, (uint8_t) (value >> 56));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 48));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 40));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 32));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 24));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 16));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 8));
    cbuffer_write_byte(cbuf, (uint8_t) value);
    return 1;
}

int cbuffer_write_bytes(cbuffer_t *cbuf, void *data, size_t sz)
{
    char *d = data;
    int i;

    assert(cbuf);
    assert(data);

    for (i = 0; i < sz; ++i) {
        cbuffer_write_byte(cbuf, *d++);
    }
    return 1;
}

void cbuffer_init_bit_access(cbuffer_t *cbuf)
{
    assert(cbuf);

    cbuf->bit_index = cbuf->write_offset * 8;
}

void cbuffer_finish_bit_access(cbuffer_t *cbuf)
{
    assert(cbuf);

    cbuf->write_offset = (cbuf->bit_index + 7) / 8;
}

void cbuffer_write_bit(cbuffer_t *cbuf, uint32_t value)
{
    assert(cbuf);

    cbuffer_write_bits(cbuf, value ? 1 : 0, 1);
}

void cbuffer_write_bits(cbuffer_t *cbuf, uint32_t value, uint32_t num_bits)
{
    int byte_pos;
    int bit_ofs;
    uint8_t c;

    assert(cbuf);
    assert(num_bits >= 0 && num_bits <= 32);

    byte_pos = (cbuf->bit_index >> 3) % BUFFER_SIZE;
    cbuf->bit_index = cbuf->bit_index % (BUFFER_SIZE * 8);
    bit_ofs = 8 - (cbuf->bit_index & 7);
    cbuf->bit_index += num_bits;

    for (; num_bits > bit_ofs; bit_ofs = 8) {
        c = cbuf->buffer[byte_pos];
        c &= ~BIT_MASK[bit_ofs];
        c |= (value >> (num_bits - bit_ofs)) & BIT_MASK[bit_ofs];
        cbuf->buffer[byte_pos++] = c;
        num_bits -= bit_ofs;
    }

    if (num_bits == bit_ofs) {
        c = cbuf->buffer[byte_pos];
        c &= ~BIT_MASK[bit_ofs];
        c |= value & BIT_MASK[bit_ofs];
        cbuf->buffer[byte_pos] = c;
    }
    else {
        c = cbuf->buffer[byte_pos];
        c &= ~(BIT_MASK[num_bits] << (bit_ofs - num_bits));
        c |= (value & BIT_MASK[num_bits]) << (bit_ofs - num_bits);
        cbuf->buffer[byte_pos] = c;
    }
}

uint8_t cbuffer_read_byte(cbuffer_t *cbuf)
{
    uint8_t ret;

    assert(cbuf);

    cbuf->read_offset = WRAP_WRITE_PTR(cbuf->read_offset, BUFFER_SIZE);
    ret = cbuf->buffer[cbuf->read_offset];
    ++cbuf->read_offset;
    return ret;
}

uint16_t cbuffer_read_short(cbuffer_t *cbuf)
{
    uint16_t value;

    assert(cbuf);

    value = cbuffer_read_byte(cbuf) << 8;
    value |= cbuffer_read_byte(cbuf);
    return value;
}

uint32_t cbuffer_read_int(cbuffer_t *cbuf)
{
    uint32_t value;

    assert(cbuf);

    value = cbuffer_read_byte(cbuf) << 24;
    value |= cbuffer_read_byte(cbuf) << 16;
    value |= cbuffer_read_byte(cbuf) << 8;
    value |= cbuffer_read_byte(cbuf);
    return value;
}

uint64_t cbuffer_read_long(cbuffer_t *cbuf)
{
    uint64_t value;

    assert(cbuf);

    value = cbuffer_read_int(cbuf);
    value <<= 32;
    value |= cbuffer_read_int(cbuf);
    return value;
}

int cbuffer_read_bytes(cbuffer_t *cbuf, uint8_t *dst, size_t siz)
{
    uint8_t *d = dst;
    int i;

    assert(cbuf);
    assert(dst);

    for (i = 0; i < siz; ++i) {
        *d++ = cbuffer_read_byte(cbuf);
    }

    return d - dst;
}

int cbuffer_read_fixedlen_string(cbuffer_t *cbuf,
                                 int readlen, char *dst, size_t siz)
{
    char *d = dst;
    size_t n = siz;
    int nr = readlen + 1;
    int read = 0;

    assert(cbuf);
    assert(dst);

    if (n != 0 && nr != 0) {
        while (--n != 0 && --nr != 0) {
            ++read;
            if ( (*d++ = cbuffer_read_byte(cbuf)) == '\0') {
                break;
            }
        }
    }

    if (n == 0) {
        if (siz != 0) {
            *d = '\0';
        }
    }

    if (nr != 0) {
        while (--nr != 0) {
            cbuffer_read_byte(cbuf);
        }
    }

    return read;
}

void cbuffer_skip(cbuffer_t *cbuf, int skip)
{
    assert(cbuf);

    cbuf->read_offset += skip;
    return;
}