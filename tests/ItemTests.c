#include "ItemTests.h"

static void
test_item_set_flags(CuTest *tc)
{
    const char *err = "item.c item_set_flags FAIL";
    int expected = 0;
    int i;
    itemdef_t *item = &item_definitions[0];

    memset(item, 0, sizeof (itemdef_t));
    item->item_id = 0;

    for (i = 1; expected < 0xFFFF; i *= 2) {
        expected = (expected << 1) | 1;
        itemdef_set_flags(item->item_id, i);
        CuAssertIntEquals_Msg(tc, err, expected, item->flags);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("item.c item_set_flags PASS\n");
#endif
}

static void
test_unset_item_flags(CuTest *tc)
{
    const char *err = "item.c item_unset_flags FAIL";
    unsigned int expected;
    int i;
    itemdef_t *item = &item_definitions[0];

    memset(item, 0, sizeof(itemdef_t));
    item->item_id = 0;
    item->flags = 0xFFFF; //Set every bit to 1 in the info field
    expected = 0xFFFF;

    for (i = 32768; expected != 0; i -= i / 2) {
        expected >>= 1;
        itemdef_unset_flags(item->item_id, i);
        CuAssertIntEquals_Msg(tc, err, expected, item->flags);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("item.c item_unset_flags PASS\n");
#endif
}

static void
test_get_item_sprite(CuTest *tc)
{
    const char *err = "item.c item_get_sprite FAIL";
    unsigned int expected = 0;
    int sprite;
    itemdef_t *item = &item_definitions[0];

    memset(item, 0, sizeof(itemdef_t));
    item->item_id = 0;

    expected = 1023;
    item->sprite = 1023;
    sprite = item_get_sprite(item->item_id);
    CuAssertIntEquals_Msg(tc, err, expected, sprite);
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("item.c item_get_sprite PASS\n");
#endif
}

static void
test_set_item_sprite(CuTest *tc)
{
    const char *err = "item.c item_set_sprite FAIL";
    int expected;
    itemdef_t *item = &item_definitions[0];

    memset(item, 0, sizeof(itemdef_t));
    item->item_id = 0;

    expected = 1023;
    item_set_sprite(item->item_id, 1023);
    CuAssertIntEquals_Msg(tc, err, expected, item->sprite);

    expected = 0;
    item_set_sprite(item->item_id, 0);
    CuAssertIntEquals_Msg(tc, err, expected, item->sprite);

    expected = 512;
    item_set_sprite(item->item_id, 512);
    CuAssertIntEquals_Msg(tc, err, expected, item->sprite);

    /*
     * This attempt to set the sprite to 1024
     * should fail due to the invariant check
     *
     * This means that the value will still be
     * 512, from the call to item_set_sprite
     * above.
     */
    expected = 512;
    item_set_sprite(item->item_id, 1024);
    CuAssertIntEquals_Msg(tc, err, expected, item->sprite);

    /*
     * Invariant check does not allow negative
     * integers, so value will still be 512.
     */
    expected = 512;
    item_set_sprite(item->item_id, -1);
    CuAssertIntEquals_Msg(tc, err, expected, item->sprite);
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("item.c item_set_sprite PASS\n");
#endif
}

CuSuite *
get_item_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, &test_item_set_flags);
    SUITE_ADD_TEST(suite, &test_unset_item_flags);
    SUITE_ADD_TEST(suite, &test_get_item_sprite);
    SUITE_ADD_TEST(suite, &test_set_item_sprite);
    return suite;
}