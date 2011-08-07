/*
 * File:   item.h
 * Author: lothy
 *
 * Created on 12 July 2011, 11:54 AM
 */

#ifndef ITEM_H
#define	ITEM_H

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
    unsigned int item_id;
    unsigned int flags;
    unsigned int sprite;
    unsigned int base_price;
    unsigned int mask;
};

struct grounditem {
    unsigned int item_id;
    unsigned int amount;
    unsigned int dropper;
    unsigned int flags;
    unsigned int x;
    unsigned int y;
    unsigned int spawn_time;
};

extern itemdef_t item_definitions[MAX_ITEM_DEFINITIONS];

int item_tradeable(int item_id);
int item_stackable(int item_id);
int item_members(int item_id);
int item_wieldable(int item_id);

int item_get_sprite(int item_id);
int item_set_sprite(int item_id, int sprite);

int itemdef_set_flags(int item_id, int flags);
int itemdef_unset_flags(int item_id, int flags);

#endif	/* ITEM_H */
