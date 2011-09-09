#include "inventory.h"
#include "item.h"

#include <assert.h>

static int adjust_inventory(player_t *player, int item_index);
static int adjust_bank(player_t *player, int item_index);

int add_inventory_item(player_t *player, int item_id, int amount)
{
    int idx;
    invitem_t *item;

    assert(player); /* player must not be a NULL pointer */
    assert(item_id > 0 && item_id < MAX_ITEM_DEFINITIONS); /* Valid item_id */
    assert(amount > 0); /* We don't accept zero or negative amounts */

    for (idx = 0; idx < MAX_INVENTORY_SIZE; ++idx) {
        item = &player->inventory_items[idx];
        if (item->item_id == item_id) {
            if (item_stackable(item_id)) {
                item->amount += amount;
                return 1;
            }
        }
        else if (item->item_id == 0) {
            item->item_id = item_id;
            item->amount = amount;
            return 1;
        }
    }

    /* Reaching this point implies that the inventory is full */
    return 0;
}

int remove_inventory_item(player_t *player, int item_id, int amount)
{
    int idx;
    invitem_t *item;

    assert(player); /* player must not be a NULL pointer */
    assert(item_id > 0 && item_id < MAX_ITEM_DEFINITIONS); /* Valid item_id */
    assert(amount > 0); /* We don't accept zero or negative amounts */

    for (idx = 0; idx < MAX_INVENTORY_SIZE; ++idx) {
        item = &player->inventory_items[idx];
        if (item->item_id == item_id) {
            if (item_stackable(item_id)) {
                if (item->amount > amount) {
                    item->amount -= amount;
                    return 1;
                }
                else {
                    adjust_inventory(player, idx);
                    return 1;
                }
            }
            else {
                adjust_inventory(player, idx);
                return 1;
            }
        }
    }

    /* The player did not have the item, so nothing was removed */
    return 0;
}

int add_bank_item(player_t *player, int item_id, int amount)
{
    int idx;

    assert(player); /* player must not be a NULL pointer */
    assert(item_id > 0 && item_id < MAX_ITEM_DEFINITIONS); /* Valid item_id */
    assert(amount > 0); /* We don't accept zero or negative amounts */

    for (idx = 0; idx < MAX_BANK_SIZE; ++idx) {
        if (player->bank_item_ids[idx] == item_id) {
            player->bank_item_amounts[idx] += amount;
            return 1;
        }
        else if (player->bank_item_ids[idx] == 0) {
            player->bank_item_ids[idx] = item_id;
            player->bank_item_amounts[idx] = amount;
            return 1;
        }
    }

    /* Reaching this point implies that the bank is full */
    return 0;
}

int remove_bank_item(player_t *player, int item_id, int amount)
{
    int idx;

    assert(player); /* player must not be a NULL pointer */
    assert(item_id > 0 && item_id < MAX_ITEM_DEFINITIONS); /* Valid item_id */
    assert(amount > 0);

    for (idx = 0; idx < MAX_BANK_SIZE; ++idx) {
        if (player->bank_item_ids[idx] == item_id) {
            if (item_stackable(item_id)) {
                if (player->bank_item_amounts[idx] > amount) {
                    player->bank_item_amounts[idx] -= amount;
                    return 1;
                }
                else {
                    adjust_bank(player, idx);
                    return 1;
                }
            }
            else {
                adjust_bank(player, idx);
                return 1;
            }
        }
    }

    /* The player did not have the item, so nothing was removed */
    return 0;
}

static int
adjust_inventory(player_t *player, int item_index)
{
    int i, j;
    int moved = MAX_INVENTORY_SIZE - item_index - 1;

    assert(player); /* player must not be a NULL pointer */
    assert(item_index >= 0 && item_index < MAX_INVENTORY_SIZE);

    for (i = item_index, j = 0; j < moved; ++i, ++j) {
        player->inventory_items[i] = player->inventory_items[i + 1];
    }

    memset(&player->inventory_items[i], 0, sizeof(invitem_t));
}

static int
adjust_bank(player_t *player, int item_index)
{
    int i, j;
    int moved = MAX_BANK_SIZE - item_index - 1;

    assert(player); /* player must not be a NULL pointer */
    assert(item_index >= 0 && item_index < MAX_BANK_SIZE);

    for (i = item_index, j = 0; j < moved; ++i, ++j) {
        player->bank_item_ids[i] = player->bank_item_ids[i + 1];
        player->bank_item_amounts[i] = player->bank_item_amounts[i + 1];
    }

    player->bank_item_ids[i] = 0;
    player->bank_item_amounts[i] = 0;
}
