/*
 * File:   tile.h
 * Author: lothy
 *
 * Created on 7 August 2011, 7:10 PM
 */

#ifndef TILE_H
#define	TILE_H

#include "common.h"
#include "gameobject.h"
#include "linkedlist.h"

#include <stdint.h>

typedef struct tile tile_t;

struct tile {
    linkedlist_t players;
    linkedlist_t npcs;
    linkedlist_t items;
    gameobject_t object;
    uint8_t map_value;
    uint8_t object_value;
};

extern tile_t tiles[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

#endif	/* TILE_H */
