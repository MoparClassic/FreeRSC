#include "circularbuffer.h"
#include "util.h"

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
    cbuf->start_offset = cbuffer_write_short(cbuf, 0); // Placeholder for size
    cbuffer_write_byte(cbuf, opcode);
    return 1;
}

/*
int cbuffer_create_frame_size_byte(cbuffer_t *cbuf, uint32_t opcode) {
    cbuf->start_offset = cbuffer_write_byte(cbuf, opcode);
    cbuffer_write_byte(cbuf, 0); // Placeholder byte for size
    return 1;
}

int cbuffer_create_frame_size_short(cbuffer_t *cbuf, uint32_t opcode) {
    cbuf->start_offset = cbuffer_write_byte(cbuf, opcode);
    cbuffer_write_short(cbuf, 0); // Placeholder short for size
    return 1;
}

int cbuffer_end_frame_size_byte(cbuffer_t *cbuf) {
    int size;
    int size_idx;

    if (cbuf->write_offset > cbuf->start_offset) {
        size = cbuf->write_offset - cbuf->start_offset - 2;
    } else {
        size = cbuf->write_offset + BUFFER_SIZE - cbuf->start_offset - 2;
    }

    size_idx = (cbuf->start_offset + 1) % BUFFER_SIZE;
    cbuf->buffer[size_idx] = (uint8_t) size;
    return 1;
}

int cbuffer_end_frame_size_short(cbuffer_t *cbuf) {
    int size;
    int size_idx;

    if (cbuf->write_offset > cbuf->start_offset) {
        size = cbuf->write_offset - cbuf->start_offset - 3;
    } else {
        size = cbuf->write_offset + BUFFER_SIZE - cbuf->start_offset - 3;
    }

    size_idx = (cbuf->start_offset + 1) % BUFFER_SIZE;
    cbuf->buffer[size_idx] = (uint8_t) (size >> 8);
    size_idx = (cbuf->start_offset + 2) % BUFFER_SIZE;
    cbuf->buffer[size_idx] = (uint8_t) size;
    return 1;
}
 */

int cbuffer_mark_read_position(cbuffer_t *cbuf)
{
    cbuf->read_marker = cbuf->read_offset;
    return 1;
}

int cbuffer_rewind_read_position(cbuffer_t *cbuf)
{
    cbuf->read_offset = cbuf->read_marker;
    return 1;
}

int cbuffer_available(cbuffer_t *cbuf)
{
    if (cbuf->write_offset >= cbuf->read_offset) {
        return cbuf->write_offset - cbuf->read_offset;
    }
    else {
        return cbuf->write_offset + BUFFER_SIZE - cbuf->read_offset;
    }
}

int cbuffer_send(cbuffer_t *cbuf)
{
    int datlen;
    int plen;
    int payload_len;
    int size_idx;
    void *data;

    if (cbuf->write_offset > cbuf->start_offset) {
        plen = cbuf->write_offset - cbuf->start_offset;
        payload_len = plen - 2; // Account for the two-byte size header
        size_idx = (cbuf->start_offset + 1) % BUFFER_SIZE;
        cbuf->buffer[size_idx] = (uint8_t) (payload_len >> 8);
        size_idx = (cbuf->start_offset + 2) % BUFFER_SIZE;
        cbuf->buffer[size_idx] = (uint8_t) payload_len;

        datlen = plen;
        data = &cbuf->buffer[cbuf->start_offset];
        send(cbuf->fd, data, datlen, 0);
    }
    else {
        plen = cbuf->write_offset + BUFFER_SIZE - cbuf->start_offset;
        payload_len = plen - 2; // Account for the two-byte size header
        size_idx = (cbuf->start_offset + 1) % BUFFER_SIZE;
        cbuf->buffer[size_idx] = (uint8_t) (payload_len >> 8);
        size_idx = (cbuf->start_offset + 2) % BUFFER_SIZE;
        cbuf->buffer[size_idx] = (uint8_t) payload_len;

        datlen = BUFFER_SIZE - plen;
        data = &cbuf->buffer[cbuf->start_offset];
        send(cbuf->fd, data, datlen, 0);

        data = cbuf->buffer;
        datlen = cbuf->write_offset;
        send(cbuf->fd, cbuf->buffer, datlen, 0);
    }
    return 1;
}

int cbuffer_send_data(cbuffer_t *cbuf, void *data, size_t sz)
{
    send(cbuf->fd, data, sz, 0);
    return 1;
}

int cbuffer_write_byte(cbuffer_t *cbuf, uint8_t value)
{
    cbuf->write_offset =
        WRAP_WRITE_PTR(cbuf->write_offset, BUFFER_SIZE);
#if UNIT_TESTING_VERBOSITY_LEVEL == 3
    printf("\tValue: %d Offset: %d\n", value, cbuf->write_offset);
#endif
    cbuf->buffer[cbuf->write_offset] = value;
    return cbuf->write_offset++;
}

int cbuffer_write_short(cbuffer_t *cbuf, uint16_t value)
{
    int ret;
#if UNIT_TESTING_VERBOSITY_LEVEL == 3
    printf("Writing short: %d\n", value);
#endif
    ret = cbuffer_write_byte(cbuf, (uint8_t) (value >> 8));
    cbuffer_write_byte(cbuf, (uint8_t) value);
    return ret;
}

int cbuffer_write_int(cbuffer_t *cbuf, uint32_t value)
{
#if UNIT_TESTING_VERBOSITY_LEVEL == 3
    printf("Writing int: %d\n", value);
#endif
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 24));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 16));
    cbuffer_write_byte(cbuf, (uint8_t) (value >> 8));
    cbuffer_write_byte(cbuf, (uint8_t) value);
    return 1;
}

int cbuffer_write_long(cbuffer_t *cbuf, uint64_t value)
{
#if UNIT_TESTING_VERBOSITY_LEVEL == 3
    printf("Writing long: %llu\n", value);
#endif
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

    for (i = 0; i < sz; ++i) {
        cbuffer_write_byte(cbuf, *d++);
    }
    return 1;
}

void cbuffer_init_bit_access(cbuffer_t *cbuf)
{
    cbuf->bit_index = cbuf->write_offset * 8;
}

void cbuffer_finish_bit_access(cbuffer_t *cbuf)
{
    cbuf->write_offset = (cbuf->bit_index + 7) / 8;
}

void cbuffer_write_bit(cbuffer_t *cbuf, uint32_t value)
{
    cbuffer_write_bits(cbuf, value ? 1 : 0, 1);
}

void cbuffer_write_bits(cbuffer_t *cbuf, uint32_t value, uint32_t num_bits)
{
    int byte_pos;
    int bit_ofs;
    uint8_t c;

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
    cbuf->read_offset = WRAP_WRITE_PTR(cbuf->read_offset, BUFFER_SIZE);
    ret = cbuf->buffer[cbuf->read_offset];
    ++cbuf->read_offset;
    return ret;
}

uint16_t cbuffer_read_short(cbuffer_t *cbuf)
{
    uint16_t value;

    value = cbuffer_read_byte(cbuf) << 8;
    value |= cbuffer_read_byte(cbuf);
    return value;
}

uint32_t cbuffer_read_int(cbuffer_t *cbuf)
{
    uint32_t value;

    value = cbuffer_read_byte(cbuf) << 24;
    value |= cbuffer_read_byte(cbuf) << 16;
    value |= cbuffer_read_byte(cbuf) << 8;
    value |= cbuffer_read_byte(cbuf);
    return value;
}

uint64_t cbuffer_read_long(cbuffer_t *cbuf)
{
    uint64_t value;

    value = cbuffer_read_int(cbuf);
    value <<= 32;
    value |= cbuffer_read_int(cbuf);
    return value;
}

int cbuffer_read_bytes(cbuffer_t *cbuf, uint8_t *dst, size_t siz)
{
    uint8_t *d = dst;
    int i;

    for (i = 0; i < siz; ++i) {
        *d++ = cbuffer_read_byte(cbuf);
    }

    return d - dst;
}

int cbuffer_read_fixedlen_string(cbuffer_t *cbuf, int readlen, char *dst, size_t siz)
{
    char *d = dst;
    size_t n = siz;
    int nr = readlen + 1;
    int read = 0;

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
    cbuf->read_offset += skip;
    return 1;
}