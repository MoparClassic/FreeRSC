#include "dataoperations.h"

#include <assert.h>
#include <gmp.h>

uint8_t dops_read_byte(void *data, int ofs)
{
    uint8_t *buf = (uint8_t *) data;

    assert(data); /* data must not be a NULL pointer */
    assert(ofs >= 0); /* Offset must be a valid array index */

    return buf[ofs];
}

uint16_t dops_read_short(void *data, int ofs)
{
    uint8_t *buf = (uint8_t *) data;
    uint16_t result = 0;

    assert(data); /* data must not be a NULL pointer */
    assert(ofs >= 0); /* Offset must be a valid array index */

    result = buf[ofs++] << 8;
    result |= buf[ofs];
    return result;
}

uint32_t dops_read_int(void *data, int ofs)
{
    uint8_t *buf = (uint8_t *) data;
    uint32_t result = 0;

    assert(data); /* data must not be a NULL pointer */
    assert(ofs >= 0); /* Offset must be a valid array index */

    result = buf[ofs++] << 24;
    result |= buf[ofs++] << 16;
    result |= buf[ofs++] << 8;
    result |= buf[ofs];
    return result;
}

uint64_t dops_read_long(void *data, int ofs)
{
    uint64_t result = 0LL;

    assert(data); /* data must not be a NULL pointer */
    assert(ofs >= 0); /* Offset must be a valid array index */

    result = dops_read_int(data, ofs);
    result <<= 32;
    result |= dops_read_int(data, ofs + 4);
    return result;
}

int dops_decrypt_data(void *data, size_t siz, int ofs)
{
    uint8_t buf[siz];
    mpz_t a, b, c;

    assert(data); /* data must not be a NULL pointer */

    memcpy(buf, data, siz);
    return 1;
}