/*
 * File:   world.h
 * Author: lothy
 *
 * Created on 21 July 2011, 3:51 PM
 */

#ifndef WORLD_H
#define	WORLD_H

#include "client.h"
#include "common.h"
#include "gameobject.h"
#include "item.h"
#include "npc.h"
#include "player.h"

#ifndef MAX_PLAYERS
#define MAX_PLAYERS 1000
#elif MAX_PLAYERS <= 1
#define MAX_PLAYERS 1000
#endif

#ifndef MAX_NPCS
#define MAX_NPCS 6000
#elif MAX_NPCS <= 1
#define MAX_NPCS 6000
#endif

#define PLAYER_LIST_FOREACH(pptr)                  \
    for ((pptr) = (player_list + 1);               \
         (pptr) < (player_list + MAX_PLAYERS + 1);  \
         ++(pptr))

#define NPC_LIST_FOREACH(nptr)                     \
    for ((nptr) = (npc_list + 1);                  \
         (nptr) < (npc_list + MAX_NPCS + 1);        \
         ++(nptr))

typedef struct tile tile_t;

struct tile {
    linkedlist_t players;
    linkedlist_t npcs;
    linkedlist_t items;
    gameobject_t object;
    uint8_t map_value;
    uint8_t object_value;
};

extern linkedlist_t client_list;
extern player_t player_list[MAX_PLAYERS + 1];
extern npc_t npc_list[MAX_NPCS + 1];
extern tile_t tiles[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

int world_init();

player_t *get_unused_player_slot();
void remove_player(player_t *player);
npc_t *get_unused_npc_slot();
void remove_npc(npc_t *npc);

int world_register_player(player_t *player);
int world_unregister_player(player_t *player);

int world_register_item(grounditem_t *item);
int world_unregister_item(grounditem_t *item);

int world_register_npc(npc_t *npc);
int world_unregister_npc(npc_t *npc);

int world_set_player_location(player_t *player,
                              int oldx, int oldy, int newx, int newy);
int world_set_npc_location(npc_t *npc, int oldx, int oldy, int newx, int newy);
int world_set_grounditem_location(grounditem_t *item,
                                  int oldx, int oldy, int newx, int newy);

#endif	/* WORLD_H */