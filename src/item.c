#include "item.h"

#include <assert.h>

itemdef_t item_definitions[MAX_ITEM_DEFINITIONS];

int
item_tradeable(int item_id)
{
    assert(item_id >= 0 && item_id < MAX_ITEM_DEFINITIONS);
    return item_definitions[item_id].flags & IDEF_FLAG_TRADEABLE;
}

int
item_stackable(int item_id)
{
    assert(item_id >= 0 && item_id < MAX_ITEM_DEFINITIONS);
    return item_definitions[item_id].flags & IDEF_FLAG_STACKABLE;
}

int
item_members(int item_id)
{
    assert(item_id >= 0 && item_id < MAX_ITEM_DEFINITIONS);
    return item_definitions[item_id].flags & IDEF_FLAG_MEMBERS;
}

int
item_wieldable(int item_id)
{
    assert(item_id >= 0 && item_id < MAX_ITEM_DEFINITIONS);
    return item_definitions[item_id].flags & IDEF_FLAG_WIELDABLE;
}

/**
 * Returns a 10 bit integer value that indicates
 * the sprite associated with this item.
 */
int
item_get_sprite(int item_id)
{
    assert(item_id >= 0 && item_id < MAX_ITEM_DEFINITIONS);
    return item_definitions[item_id].sprite;
}

int
item_set_sprite(int item_id, int sprite)
{
    assert(sprite >= 0 && sprite <= 1023);

    if (sprite < 0 || sprite > 1023) {
        return 0;
    }
    item_definitions[item_id].sprite = sprite;
    return 1;
}

int
itemdef_set_flags(int item_id, int flags)
{
    assert(item_id >= 0 && item_id < MAX_ITEM_DEFINITIONS);
    item_definitions[item_id].flags |= flags;
    return 1;
}

int
itemdef_unset_flags(int item_id, int flags)
{
    assert(item_id >= 0 && item_id < MAX_ITEM_DEFINITIONS);
    item_definitions[item_id].flags &= ~flags;
    return 1;
}