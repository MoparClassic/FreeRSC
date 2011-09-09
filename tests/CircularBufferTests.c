#include "CircularBufferTests.h"

static void
test_cbuffer_write_byte(CuTest *tc)
{
    const char *err = "circularbuffer.c write_byte FAIL";
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_byte PASS\n");
#endif
}

static void
test_cbuffer_create_frame(CuTest *tc)
{
    const char *err = "circularbuffer.c create_frame FAIL";
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c create_frame PASS\n");
#endif
}

static void
test_cbuffer_write_short(CuTest *tc)
{
    const char *err = "circularbuffer.c write_short FAIL";
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_short PASS\n");
#endif
}

static void
test_cbuffer_write_int(CuTest *tc)
{
    const char *err = "circularbuffer.c write_int FAIL";
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_int PASS\n");
#endif
}

static void
test_cbuffer_write_long(CuTest *tc)
{
    const char *err = "circularbuffer.c write_long FAIL";
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_long PASS\n");
#endif
}

static void
test_cbuffer_bit_functions(CuTest *tc)
{
    const char *err = "circularbuffer.c add_bits FAIL";
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c add_bits PASS\n");
#endif
}

static void
test_cbuffer_format_packet(CuTest *tc)
{
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    memset(&cbuf, 0, sizeof(cbuffer_t));

    cbuffer_create_frame(&cbuf, 100);
    cbuffer_write_long(&cbuf, 0xFFFFFFFFFFFFFFFFLL);
    cbuffer_format_packet(&cbuf);

    cbuffer_create_frame(&cbuf, 150);
    cbuffer_write_long(&cbuf, 0xEEEEEEEEEEEEEEEELL);
    cbuffer_format_packet(&cbuf);

    cbuffer_create_frame(&cbuf, 200);
    cbuffer_write_long(&cbuf, 0xDDDDDDDDDDDDDDDDLL);
    cbuffer_write_long(&cbuf, 0xCCCCCCCCCCCCCCCCLL);
    cbuffer_write_long(&cbuf, 0xBBBBBBBBBBBBBBBBLL);
    cbuffer_format_packet(&cbuf);

    printf("Buffer contents:\n");
    CBUFFER_FOREACH(val, &cbuf) {
        printf("%d, ", *val);
    }
    printf("\n");
}

CuSuite * get_circular_buffer_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, &test_cbuffer_write_byte);
    SUITE_ADD_TEST(suite, &test_cbuffer_create_frame);
    SUITE_ADD_TEST(suite, &test_cbuffer_write_short);
    SUITE_ADD_TEST(suite, &test_cbuffer_write_int);
    SUITE_ADD_TEST(suite, &test_cbuffer_write_long);
    SUITE_ADD_TEST(suite, &test_cbuffer_bit_functions);
    SUITE_ADD_TEST(suite, &test_cbuffer_format_packet);
    return suite;
}
