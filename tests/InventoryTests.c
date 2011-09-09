#include "InventoryTests.h"
#include <string.h>

static void
print_player_inventory(player_t *p)
{
    int i;

    printf("Inventory: \n");
    for (i = 0; i < MAX_INVENTORY_SIZE; ++i) {
        printf("(S:%d, ID:%d, A:%d)\n",
               i,
               p->inventory_items[i].item_id,
               p->inventory_items[i].amount);
    }
    printf("\n");
}

static void
print_player_bank(player_t *p)
{
    int i;

    printf("Bank: \n");
    for (i = 0; i < MAX_BANK_SIZE; ++i) {
        printf("(S:%d, ID:%d, A:%d)",
               i,
               p->bank_item_ids[i],
               p->bank_item_amounts[i]);
    }
    printf("\n\n");
}

static void
test_add_inventory_item(CuTest *tc)
{
    const char *err = "inventory.c add_inventory_item FAIL";
    player_t player;
    int i;
    int ret;

    memset(&player, 0, sizeof(player));

    for (i = 0; i < MAX_INVENTORY_SIZE; ++i) {
        ret = add_inventory_item(&player, i + 100, 1);
        if (ret != 1) {
            CuFail(tc, err);
        }
    }

    /* Inventory is full, so this item won't be added */
    ret = add_inventory_item(&player, 1000, 1);
    CuAssertIntEquals_Msg(tc, err, 0, ret);

    //print_player_inventory(&player);

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("inventory.c add_inventory_item PASS\n");
#endif
}

static void
test_remove_inventory_item(CuTest *tc)
{
    const char *err = "inventory.c remove_inventory_item FAIL";
    player_t player;
    int i, e;
    int item_id;
    int item_amount;

    memset(&player, 0, sizeof(player));
    itemdef_set_flags(111, IDEF_FLAG_STACKABLE);
    itemdef_set_flags(120, IDEF_FLAG_STACKABLE);

    /* Populate the player's inventory so we have items to remove */
    for (i = 0; i < MAX_INVENTORY_SIZE; ++i) {
        add_inventory_item(&player, i + 100, i + 1);
    }

    //print_player_inventory(&player);

    /*
     * Test removal of items from the inventory.
     * If an item is not stackable then the amount argument
     * that is passed to the remove_inventory_item function
     * is irrelevant.
     * However, the amount argument should still be valid.
     * In other words, 0 < amount.
     */
    remove_inventory_item(&player, 100, 1);
    remove_inventory_item(&player, 111, 10); /* Stackable */
    remove_inventory_item(&player, 120, 10); /* Stackable */
    remove_inventory_item(&player, 112, 1);
    remove_inventory_item(&player, 115, 1);
    //print_player_inventory(&player);

    for (i = 0, e = 100; i < MAX_INVENTORY_SIZE; ++i, ++e) {
        item_id = player.inventory_items[i].item_id;
        item_amount = player.inventory_items[i].amount;

        /* These item ids were removed */
        if (e == 100 || e == 112 || e == 115) {
            ++e;
        }
        if (i >= 27) {
            e = 0;
        }

        /* Check to see if item amount has been changed properly */
        if (item_id == 111) {
            CuAssertIntEquals_Msg(tc, err, 2, item_amount);
            continue;
        }

        /* Check to see if item amount has been changed properly */
        if (item_id == 120) {
            CuAssertIntEquals_Msg(tc, err, 11, item_amount);
            continue;
        }

        CuAssertIntEquals_Msg(tc, err, e, item_id);
    }

    itemdef_unset_flags(111, IDEF_FLAG_STACKABLE);
    itemdef_unset_flags(120, IDEF_FLAG_STACKABLE);
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("inventory.c remove_inventory_item PASS\n");
#endif
}

static void
test_add_bank_item(CuTest *tc)
{
    const char *err = "inventory.c add_bank_item FAIL";
    player_t player;
    int i;
    int ret;

    memset(&player, 0, sizeof(player));

    for (i = 0; i < MAX_BANK_SIZE; ++i) {
        ret = add_bank_item(&player, i + 100, 1);
        if (ret != 1) {
            CuFail(tc, err);
        }
    }

    /* Inventory is full, so this item won't be added */
    ret = add_bank_item(&player, 1000, 1);
    CuAssertIntEquals_Msg(tc, err, 0, ret);

    //print_player_inventory(&player);
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("inventory.c add_bank_item PASS\n");
#endif
}

static void
test_remove_bank_item(CuTest *tc)
{
    const char *err = "inventory.c remove_bank_item FAIL";
    player_t player;
    int i, e;
    int item_id;
    int item_amount;

    memset(&player, 0, sizeof(player));
    itemdef_set_flags(215, IDEF_FLAG_STACKABLE);

    /* Populate the player's inventory so we have items to remove */
    for (i = 0; i < MAX_BANK_SIZE; ++i) {
        add_bank_item(&player, i + 100, i + 1);
    }

    /*
     * Test removal of items from the bank.
     * If an item is not stackable then the amount argument
     * that is passed to the remove_bank_item function
     * is irrelevant.
     * However, the amount argument should still be valid.
     * In other words, 0 < amount.
     */
    remove_bank_item(&player, 100, 1);
    remove_bank_item(&player, 112, 1);
    remove_bank_item(&player, 115, 1);
    remove_bank_item(&player, 215, 100);

    for (i = 0, e = 100; i < MAX_BANK_SIZE; ++i, ++e) {
        item_id = player.bank_item_ids[i];
        item_amount = player.bank_item_amounts[i];

        /* These item ids were removed */
        if (e == 100 || e == 112 || e == 115) {
            ++e;
        }

        //printf("i: %d, e: %d, id: %d amount: %d\n", i, e, item_id, item_amount);
        if (i >= 189) {
            e = 0;
        }

        if (item_id == 215) {
            CuAssertIntEquals_Msg(tc, err, 16, item_amount);
            continue;
        }

        CuAssertIntEquals_Msg(tc, err, e, item_id);
    }

    itemdef_unset_flags(215, IDEF_FLAG_STACKABLE);
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("inventory.c remove_bank_item PASS\n");
#endif
}

CuSuite *
get_inventory_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, &test_add_inventory_item);
    SUITE_ADD_TEST(suite, &test_remove_inventory_item);
    SUITE_ADD_TEST(suite, &test_add_bank_item);
    SUITE_ADD_TEST(suite, &test_remove_bank_item);
    return suite;
}
