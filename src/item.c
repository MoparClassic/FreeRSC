#include "item.h"

itemdef_t item_definitions[MAX_ITEM_DEFINITIONS];

int
grounditem_set_flags(grounditem_t *item, int flags)
{
    item->flags |= flags;
    return 1;
}

int
grounditem_unset_flags(grounditem_t *item, int flags)
{
    item->flags &= ~flags;
    return 1;
}

int
item_tradeable(int item_id)
{
    itemdef_t *idef = &item_definitions[item_id];
    return idef->flags & IDEF_FLAG_TRADEABLE;
}

int
item_stackable(int item_id)
{
    itemdef_t *idef = &item_definitions[item_id];
    return idef->flags & IDEF_FLAG_STACKABLE;
}

int
item_members(int item_id)
{
    itemdef_t *idef = &item_definitions[item_id];
    return idef->flags & IDEF_FLAG_MEMBERS;
}

int
item_wieldable(int item_id)
{
    itemdef_t *idef = &item_definitions[item_id];
    return idef->flags & IDEF_FLAG_WIELDABLE;
}

/**
 * Returns a 10 bit integer value that indicates
 * the sprite associated with this item.
 */
int
item_get_sprite(int item_id)
{
    itemdef_t *idef = &item_definitions[item_id];
    return idef->sprite;
}

int
item_set_sprite(int item_id, int sprite)
{
    itemdef_t *idef;

    if (sprite < 0 || sprite > 1023) {
        return 0;
    }

    idef = &item_definitions[item_id];
    idef->sprite = sprite;
    return 1;
}

int
itemdef_set_flags(int item_id, int flags)
{
    itemdef_t *idef = &item_definitions[item_id];
    idef->flags |= flags;
    return 1;
}

int
itemdef_unset_flags(int item_id, int flags)
{
    itemdef_t *idef = &item_definitions[item_id];
    idef->flags &= ~flags;
    return 1;
}