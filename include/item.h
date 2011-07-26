/*
 * File:   item.h
 * Author: lothy
 *
 * Created on 12 July 2011, 11:54 AM
 */

#ifndef ITEM_H
#define	ITEM_H

#include "common.h"

#define MAX_ITEM_DEFINITIONS    1500

#define IDEF_FLAG_TRADEABLE     0x0001
#define IDEF_FLAG_STACKABLE     0x0002
#define IDEF_FLAG_MEMBERS       0x0004
#define IDEF_FLAG_WIELDABLE     0x0008
#define IDEF_FLAG_UNUSED_ONE    0x0010
#define IDEF_FLAG_UNUSED_TWO    0x0020

typedef struct itemdef itemdef_t;
typedef struct grounditem grounditem_t;

struct itemdef {
    char name[30];
    char description[80];
    char command[40];
    uint16_t item_id;
    uint16_t flags;
    uint16_t sprite;
    uint32_t base_price;
    uint32_t mask;
};

struct grounditem {
    uint16_t item_id;
    uint32_t amount;
    uint64_t dropper;
    uint16_t flags;
    uint16_t x;
    uint16_t y;
    uint32_t spawn_time;
};

extern itemdef_t item_definitions[MAX_ITEM_DEFINITIONS];

int grounditem_set_flags(grounditem_t *item, int flags);
int grounditem_unset_flags(grounditem_t *item, int flags);

int item_tradeable(int item_id);
int item_stackable(int item_id);
int item_members(int item_id);
int item_wieldable(int item_id);

int item_get_sprite(int item_id);
int item_set_sprite(int item_id, int sprite);

int itemdef_set_flags(int item_id, int flags);
int itemdef_unset_flags(int item_id, int flags);


#endif	/* ITEM_H */
