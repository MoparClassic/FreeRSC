/*
 * File:   circularbuffer.h
 * Author: lothy
 *
 * Created on 13 July 2011, 3:57 PM
 */

#ifndef INCLUDED_CIRCULARBUFFER_H
#define	INCLUDED_CIRCULARBUFFER_H

#include <stdint.h>
#include <stdlib.h>

#ifdef UNIT_TESTING
#define BUFFER_SIZE 50
#else
#define BUFFER_SIZE 4096
#endif

#define WRAP_WRITE_PTR(x, y) ((x) % (y))

#define CBUFFER_FOREACH(e, cbuf)                               \
    for ((cbuf)->iter = (cbuf)->buffer, (e) = (cbuf)->iter;    \
         (cbuf)->iter < (cbuf)->buffer + BUFFER_SIZE;          \
         (e) = ++(cbuf)->iter)

typedef struct circular_buffer cbuffer_t;

struct circular_buffer {
    int fd;
    unsigned int start_offset;
    unsigned int write_offset;
    unsigned int read_offset;
    unsigned int read_marker;
    unsigned int bit_index;
    uint8_t buffer[BUFFER_SIZE];
    uint8_t *iter;
};

int cbuffer_create_frame(cbuffer_t *cbuf, uint32_t opcode);
int cbuffer_mark_read_position(cbuffer_t *cbuf);
int cbuffer_rewind_read_position(cbuffer_t *cbuf);
int cbuffer_available(cbuffer_t *cbuf);
int cbuffer_format_packet(cbuffer_t *cbuf);
int cbuffer_send(cbuffer_t *cbuf);
int cbuffer_send_data(cbuffer_t *cbuf, void *data, size_t sz);

int cbuffer_write_byte(cbuffer_t *cbuf, uint8_t value);
int cbuffer_write_short(cbuffer_t *cbuf, uint16_t value);
int cbuffer_write_int(cbuffer_t *cbuf, uint32_t value);
int cbuffer_write_long(cbuffer_t *cbuf, uint64_t value);
int cbuffer_write_bytes(cbuffer_t *cbuf, void *data, size_t sz);
void cbuffer_init_bit_access(cbuffer_t *cbuf);
void cbuffer_finish_bit_access(cbuffer_t *cbuf);
void cbuffer_write_bit(cbuffer_t *cbuf, uint32_t value);
void cbuffer_write_bits(cbuffer_t *cbuf, uint32_t value, uint32_t num_bits);

uint8_t cbuffer_read_byte(cbuffer_t *cbuf);
uint16_t cbuffer_read_short(cbuffer_t *cbuf);
uint32_t cbuffer_read_int(cbuffer_t *cbuf);
uint64_t cbuffer_read_long(cbuffer_t *cbuf);
int cbuffer_read_bytes(cbuffer_t *cbuf, uint8_t *dst, size_t siz);
int cbuffer_read_fixedlen_string(cbuffer_t *cbuf, int readlen,
                                 char *dst, size_t siz);
void cbuffer_skip(cbuffer_t *cbuf, int skip);

#endif	/* INCLUDED_CIRCULARBUFFER_H */
