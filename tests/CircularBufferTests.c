#include "CircularBufferTests.h"

static void
test_cbuffer_write_byte(CuTest *tc)
{
    const char *err = "circularbuffer.c write_byte FAIL";
    uint8_t expected1[] = {
        10, 11, 12, 13, 14, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    uint8_t expected2[] = {
        60, 11, 12, 13, 14, 50, 51, 52,
        53, 54, 55, 56, 57, 58, 59
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 10; i < 15; ++i) {
        cbuffer_write_byte(&cbuf, i);
    }

    i = 0;
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("circularbuffer.c write_byte:\n");
    printf("Part 1: cbuffer_t contents:\n");
#endif

    CBUFFER_FOREACH(val, &cbuf) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("\tExpected: %d \tActual: %d\n", expected1[i], *val);
#endif
        CuAssertIntEquals_Msg(tc, err, expected1[i++], *val);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("\n");
#endif

    for (i = 50; i < 61; ++i) {
        cbuffer_write_byte(&cbuf, i);
    }
    i = 0;

#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("Part 2: cbuffer_t contents:\n");
#endif

    CBUFFER_FOREACH(val, &cbuf) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("\tExpected: %d \tActual: %d\n", expected2[i], *val);
#endif
        CuAssertIntEquals_Msg(tc, err, expected2[i++], *val);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_byte PASS\n");
#endif
}

static void
test_cbuffer_create_frame(CuTest *tc)
{
    const char *err = "circularbuffer.c create_frame FAIL";
    uint8_t expected1[] = {
        0, 0, 200, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    uint8_t expected2[] = {
        0, 1, 2, 3, 4, 5, 0, 0,
        150, 1, 1, 1, 1, 1, 1
    };
    uint8_t expected3[] = {
        0, 150, 10, 10, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 0
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    // Commence first part of test
    i = 0;
    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_create_frame(&cbuf, 200);
    CuAssertIntEquals_Msg(tc, err, 0, cbuf.start_offset);
    CuAssertIntEquals_Msg(tc, err, 3, cbuf.write_offset);

    CBUFFER_FOREACH(val, &cbuf) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("%d ", *val);
#endif
        CuAssertIntEquals_Msg(tc, err, expected1[i++], *val);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("\n");
#endif

    // Commence second part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 6; ++i) {
        cbuffer_write_byte(&cbuf, i);
    }
    cbuffer_create_frame(&cbuf, 150);
    CuAssertIntEquals_Msg(tc, err, 6, cbuf.start_offset);
    CuAssertIntEquals_Msg(tc, err, 9, cbuf.write_offset);
    for (i = 0; i < 6; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("%d ", *val);
#endif
        CuAssertIntEquals_Msg(tc, err, expected2[i++], *val);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("\n");
#endif

    // Commence third part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 14; ++i) {
        cbuffer_write_byte(&cbuf, 9);
    }
    cbuffer_create_frame(&cbuf, 150);
    CuAssertIntEquals_Msg(tc, err, 14, cbuf.start_offset);
    CuAssertIntEquals_Msg(tc, err, 2, cbuf.write_offset);
    cbuffer_write_byte(&cbuf, 10);
    cbuffer_write_byte(&cbuf, 10);
    CuAssertIntEquals_Msg(tc, err, 14, cbuf.start_offset);
    CuAssertIntEquals_Msg(tc, err, 4, cbuf.write_offset);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("%d ", *val);
#endif
        CuAssertIntEquals_Msg(tc, err, expected3[i++], *val);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c create_frame PASS\n");
#endif
}

/*
static void
test_cbuffer_create_frame_size_byte(CuTest *tc) {
    const char *err = "circularbuffer.c create_frame_size_byte FAIL";
    uint8_t expected1[] = {
        200, 0, 1, 2, 3, 4, 5, 6,
        7, 8, 9, 10, 11, 12, 13
    };
    uint8_t expected2[] = {
        1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 200, 0
    };
    uint8_t expected3[] = {
        0, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 200
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    // Commence first part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_create_frame_size_byte(&cbuf, 200);
    CuAssertIntEquals_Msg(tc, err, 0, cbuf.start_offset);
    CuAssertIntEquals_Msg(tc, err, 2, cbuf.write_offset);
    for (i = 1; i < 14; ++i) {
        cbuffer_write_byte(&cbuf, i);
    }
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected1[i++], *val);
    }
    //printf("\n");

    // Commence second part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 1; i < 14; ++i) {
        cbuffer_write_byte(&cbuf, i);
    }
    cbuffer_create_frame_size_byte(&cbuf, 200);
    CuAssertIntEquals_Msg(tc, err, 13, cbuf.start_offset);
    CuAssertIntEquals_Msg(tc, err, 15, cbuf.write_offset);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected2[i++], *val);
    }
    //printf("\n");

    // Commence third part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 1; i < 15; ++i) {
        cbuffer_write_byte(&cbuf, i);
    }
    cbuffer_create_frame_size_byte(&cbuf, 200);
    CuAssertIntEquals_Msg(tc, err, 14, cbuf.start_offset);
    CuAssertIntEquals_Msg(tc, err, 1, cbuf.write_offset);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected3[i++], *val);
    }
    //printf("\n");

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c create_frame_size_byte PASS\n");
#endif
}

static void
test_cbuffer_create_frame_size_short(CuTest *tc) {
    const char *err = "circularbuffer.c create_frame_size_short FAIL";
    uint8_t expected1[] = {
        222, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    uint8_t expected2[] = {
        0, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 222, 0
    };
    uint8_t expected3[] = {
        0, 0, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 123
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    // Commence first part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_create_frame_size_short(&cbuf, 222);
    cbuffer_write_byte(&cbuf, 1);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected1[i++], *val);
    }
    //printf("\n");

    // Commence second part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 13; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    cbuffer_create_frame_size_short(&cbuf, 222);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected2[i++], *val);
    }
    //printf("\n");

    // Commence third part of test
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 14; ++i) {
        cbuffer_write_byte(&cbuf, 5);
    }
    cbuffer_create_frame_size_short(&cbuf, 123);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected3[i++], *val);
    }
    //printf("\n");
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c create_frame_size_short PASS\n");
#endif
}

static void
test_cbuffer_end_frame_size_byte(CuTest *tc) {
    const char *err = "circularbuffer.c end_frame_size_byte FAIL";
    uint8_t expected1[] = {
        250, 5, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0
    };
    uint8_t expected2[] = {
        1, 1, 1, 1, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 250, 4
    };
    uint8_t expected3[] = {
        4, 5, 5, 5, 5, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 222
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    // Commence part one
    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_create_frame_size_byte(&cbuf, 250);
    for (i = 0; i < 5; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    cbuffer_end_frame_size_byte(&cbuf);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected1[i++], *val);
    }
    //printf("\n");

    // Commence part two
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 13; ++i) {
        cbuffer_write_byte(&cbuf, 2);
    }
    cbuffer_create_frame_size_byte(&cbuf, 250);
    for (i = 0; i < 4; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    cbuffer_end_frame_size_byte(&cbuf);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected2[i++], *val);
    }
    //printf("\n");

    // Commence part three
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 14; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    cbuffer_create_frame_size_byte(&cbuf, 222);
    for (i = 0; i < 4; ++i) {
        cbuffer_write_byte(&cbuf, 5);
    }
    cbuffer_end_frame_size_byte(&cbuf);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected3[i++], *val);
    }
    //printf("\n");
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c end_frame_size_byte PASS\n");
#endif
}

static void
test_cbuffer_end_frame_size_short(CuTest *tc) {
    const char *err = "circularbuffer.c end_frame_size_short FAIL";
    uint8_t expected1[] = {
        222, 0, 5, 3, 3, 3, 3, 3,
        0, 0, 0, 0, 0, 0, 0
    };
    uint8_t expected2[] = {
        2, 2, 2, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 222, 0, 3
    };
    uint8_t expected3[] = {
        5, 9, 9, 9, 9, 9, 1, 1,
        1, 1, 1, 1, 1, 222, 0
    };
    uint8_t expected4[] = {
        0, 6, 8, 8, 8, 8, 8, 8,
        1, 1, 1, 1, 1, 1, 222
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    // Commence part one
    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_create_frame_size_short(&cbuf, 222);
    for (i = 0; i < 5; ++i) {
        cbuffer_write_byte(&cbuf, 3);
    }
    cbuffer_end_frame_size_short(&cbuf);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected1[i++], *val);
    }
    //printf("\n");

    // Commence part two
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 12; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    cbuffer_create_frame_size_short(&cbuf, 222);
    for (i = 0; i < 3; ++i) {
        cbuffer_write_byte(&cbuf, 2);
    }
    cbuffer_end_frame_size_short(&cbuf);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected2[i++], *val);
    }
    //printf("\n");

    // Commence part three
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 13; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    cbuffer_create_frame_size_short(&cbuf, 222);
    for (i = 0; i < 5; ++i) {
        cbuffer_write_byte(&cbuf, 9);
    }
    cbuffer_end_frame_size_short(&cbuf);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected3[i++], *val);
    }
    //printf("\n");

    // Commence part four
    memset(&cbuf, 0, sizeof (cbuffer_t));
    for (i = 0; i < 14; ++i) {
        cbuffer_write_byte(&cbuf, 1);
    }
    cbuffer_create_frame_size_short(&cbuf, 222);
    for (i = 0; i < 6; ++i) {
        cbuffer_write_byte(&cbuf, 8);
    }
    cbuffer_end_frame_size_short(&cbuf);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected4[i++], *val);
    }
    //printf("\n");
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c end_frame_size_short PASS\n");
#endif
}
 */

static void
test_cbuffer_write_short(CuTest *tc)
{
    const char *err = "circularbuffer.c write_short FAIL";
    uint8_t expected[] = {
        255, 255, 0, 0, 255, 0, 0, 255,
        254, 254, 0, 0, 0, 0, 0
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_write_short(&cbuf, 0xFFFF);
    cbuffer_write_short(&cbuf, 0);
    cbuffer_write_short(&cbuf, 0xFF00);
    cbuffer_write_short(&cbuf, 0x00FF);
    cbuffer_write_short(&cbuf, 0xFEFE);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected[i++], *val);
    }
    //printf("\n");

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_short PASS\n");
#endif
}

static void
test_cbuffer_write_int(CuTest *tc)
{
    const char *err = "circularbuffer.c write_int FAIL";
    uint8_t expected[] = {
        190, 187, 204, 221, 221, 204, 187, 170,
        255, 238, 221, 204, 202, 254, 186
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_write_int(&cbuf, 0xAABBCCDD);
    cbuffer_write_int(&cbuf, 0xDDCCBBAA);
    cbuffer_write_int(&cbuf, 0xFFEEDDCC);
    cbuffer_write_int(&cbuf, 0xCAFEBABE);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected[i++], *val);
    }
    //printf("\n");
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_int PASS\n");
#endif
}

static void
test_cbuffer_write_long(CuTest *tc)
{
    const char *err = "circularbuffer.c write_long FAIL";
    uint8_t expected[] = {
        255, 17, 34, 51, 68, 85, 102, 119,
        136, 153, 170, 187, 204, 221, 238
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    memset(&cbuf, 0, sizeof (cbuffer_t));
    cbuffer_write_long(&cbuf, 0x0011223344556677LL);
    cbuffer_write_long(&cbuf, 0x8899AABBCCDDEEFFLL);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected[i++], *val);
    }
    //printf("\n");
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c write_long PASS\n");
#endif
}

static void
test_cbuffer_bit_functions(CuTest *tc)
{
    const char *err = "circularbuffer.c add_bits FAIL";
    uint8_t expected[] = {
        1, 128, 255, 255, 255, 255, 170, 170,
        85, 248, 128, 0, 0, 0, 0
    };
    cbuffer_t cbuf;
    uint8_t *val;
    int i;

    memset(&cbuf, 0, sizeof (cbuffer_t));

    /*
     * If no bits are written between init_bit_access
     * and finish_bit_access function calls, the
     * write_offset field should have the same value
     * before and after calls to these functions.
     * In this case, zero.
     */
    cbuffer_init_bit_access(&cbuf);
    CuAssertIntEquals_Msg(tc, err, 0, cbuf.bit_index);
    cbuffer_finish_bit_access(&cbuf);
    CuAssertIntEquals_Msg(tc, err, 0, cbuf.write_offset);

    cbuffer_write_byte(&cbuf, 1);
    cbuffer_init_bit_access(&cbuf);
    CuAssertIntEquals_Msg(tc, err, 8, cbuf.bit_index);
    cbuffer_write_bit(&cbuf, 1);
    CuAssertIntEquals_Msg(tc, err, 9, cbuf.bit_index);
    cbuffer_finish_bit_access(&cbuf);
    /*
     * At least one bit was written, so write_offset
     * should point to the position after the byte
     * that was last used to store bits.
     */
    CuAssertIntEquals_Msg(tc, err, 2, cbuf.write_offset);

    cbuffer_init_bit_access(&cbuf);
    CuAssertIntEquals_Msg(tc, err, 16, cbuf.bit_index);

    // Should write four bytes, each with value of 255 (all bits on)
    cbuffer_write_bits(&cbuf, 0xFFFFFFFF, 32);
    CuAssertIntEquals_Msg(tc, err, 48, cbuf.bit_index);

    // Should write one byte with alternating bits
    cbuffer_write_bits(&cbuf, 170, 8);
    CuAssertIntEquals_Msg(tc, err, 56, cbuf.bit_index);

    /*
     * Should write one byte, one bit at a time, alternating bits.
     * Result: 170 Bitfield: 10101010
     */
    for (i = 0; i < 8; ++i) {
        if (i % 2 == 0) {
            cbuffer_write_bit(&cbuf, 1);
        }
        else {
            cbuffer_write_bit(&cbuf, 0);
        }
        CuAssertIntEquals_Msg(tc, err, 56 + i + 1, cbuf.bit_index);
    }

    /*
     * Should write one byte, one bit at a time, alternating bits.
     * Result: 85 Bitfield: 01010101
     */
    for (i = 0; i < 8; ++i) {
        if (i % 2 == 1) {
            cbuffer_write_bit(&cbuf, 1);
        }
        else {
            cbuffer_write_bit(&cbuf, 0);
        }
        CuAssertIntEquals_Msg(tc, err, 64 + i + 1, cbuf.bit_index);
    }

    // Writes 496, or 111110000
    cbuffer_write_bits(&cbuf, 497, 9);
    CuAssertIntEquals_Msg(tc, err, 81, cbuf.bit_index);

    cbuffer_finish_bit_access(&cbuf);
    CuAssertIntEquals_Msg(tc, err, 11, cbuf.write_offset);
    i = 0;

    CBUFFER_FOREACH(val, &cbuf) {
        //printf("%d ", *val);
        CuAssertIntEquals_Msg(tc, err, expected[i++], *val);
    }
    //printf("\n");


#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("circularbuffer.c add_bits PASS\n");
#endif
}

CuSuite * get_circular_buffer_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, &test_cbuffer_write_byte);
    SUITE_ADD_TEST(suite, &test_cbuffer_create_frame);
    /*SUITE_ADD_TEST(suite, &test_cbuffer_create_frame_size_byte);
    SUITE_ADD_TEST(suite, &test_cbuffer_create_frame_size_short);
    SUITE_ADD_TEST(suite, &test_cbuffer_end_frame_size_byte);
    SUITE_ADD_TEST(suite, &test_cbuffer_end_frame_size_short);*/
    SUITE_ADD_TEST(suite, &test_cbuffer_write_short);
    SUITE_ADD_TEST(suite, &test_cbuffer_write_int);
    SUITE_ADD_TEST(suite, &test_cbuffer_write_long);
    SUITE_ADD_TEST(suite, &test_cbuffer_bit_functions);
    return suite;
}
