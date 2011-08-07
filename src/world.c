#include "world.h"
#include "tile.h"

#include <assert.h>
#include <string.h>

linkedlist_t client_list;
player_t player_list[MAX_PLAYERS + 1];
npc_t npc_list[MAX_NPCS + 1];
tile_t tiles[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

int world_init()
{
    memset(player_list, 0, sizeof(player_list));
    memset(npc_list, 0, sizeof(npc_list));
    memset(tiles, 0, sizeof(tile_t) * MAX_MAP_WIDTH * MAX_MAP_HEIGHT);
}

player_t *
get_unused_player_slot()
{
    player_t *player = NULL;
    static int i = 0;
    int j;

    for (j = 0; j < MAX_PLAYERS; ++j, ++i) {
        i %= MAX_PLAYERS;
        if (player_list[i + 1].index == UNUSED) {
            player = &player_list[i + 1];
            memset(player, 0, sizeof(player_t));
            player->index = i + 1;
            break;
        }
    }
    i = (i + 1) % MAX_PLAYERS;
    return player;
}

void remove_player(player_t *player)
{
    assert(player); /* player must not be a NULL pointer */
    
    linkedlist_clear(&player->bubbles_to_display);
    linkedlist_clear(&player->chat_messages_to_display);
    linkedlist_clear(&player->hit_updates_to_display);
    linkedlist_clear(&player->projectiles_to_display);
    linkedlist_clear(&player->appearance_updates_to_display);
    linkedlist_clear(&player->npc_hit_updates_to_display);
    linkedlist_clear(&player->npc_chat_messages_to_display);

    linkedlist_clear(&player->known_appearance_ids);
    eslist_clear(&player->watched_players);
    eslist_clear(&player->watched_objects);
    eslist_clear(&player->watched_items);
    eslist_clear(&player->watched_npcs);
    player->index = UNUSED;
}

npc_t *
get_unused_npc_slot()
{
    npc_t *npc = NULL;
    static int i = 0;
    int j;

    for (j = 0; j < MAX_NPCS; ++j, ++i) {
        i %= MAX_NPCS;
        if (npc_list[i].index == UNUSED) {
            npc = &npc_list[i];
            memset(npc, 0, sizeof(npc_t));
            npc->index = i;
            break;
        }
    }
    i = (i + 1) % MAX_NPCS;
    return npc;
}

void remove_npc(npc_t *npc)
{
    assert(npc); /* npc must not be a NULL pointer */
    
    memset(npc, 0, sizeof(npc_t));
    npc->index = UNUSED;
}

int world_register_player(player_t *player)
{

}

int world_unregister_player(player_t *player)
{

}

int world_register_item(grounditem_t *item)
{

}

int world_unregister_item(grounditem_t *item)
{

}

int world_register_npc(npc_t *npc)
{

}

int world_unregister_npc(npc_t *npc)
{

}

int world_set_player_location(player_t *player, int oldx, int oldy,
                              int newx, int newy)
{
    tile_t *old_tile;
    tile_t *new_tile;

    assert(player); /* player must not be a NULL pointer */

    /* Co-ordinates must be valid */
    assert(oldx >= 0 && oldx < MAX_MAP_WIDTH);
    assert(oldy >= 0 && oldy < MAX_MAP_HEIGHT);
    assert(newx >= 0 && newx < MAX_MAP_WIDTH);
    assert(newy >= 0 && newy < MAX_MAP_HEIGHT);

    old_tile = &tiles[oldx][oldy];
    new_tile = &tiles[newx][newy];

    player->x = newx;
    player->y = newy;
    linkedlist_remove(&old_tile->players, player);
    linkedlist_prepend(&new_tile->players, player);
    return 1;
}

int world_set_npc_location(npc_t *npc, int oldx, int oldy,
                           int newx, int newy)
{
    tile_t *old_tile;
    tile_t *new_tile;

    assert(npc); /* npc must not be a NULL pointer */

    /* Co-ordinates must be valid */
    assert(oldx >= 0 && oldx < MAX_MAP_WIDTH);
    assert(oldy >= 0 && oldy < MAX_MAP_HEIGHT);
    assert(newx >= 0 && newx < MAX_MAP_WIDTH);
    assert(newy >= 0 && newy < MAX_MAP_HEIGHT);

    old_tile = &tiles[oldx][oldy];
    new_tile = &tiles[newx][newy];

    npc->x = newx;
    npc->y = newy;
    linkedlist_remove(&old_tile->npcs, npc);
    linkedlist_prepend(&new_tile->npcs, npc);
    return 1;
}

int world_set_grounditem_location(grounditem_t *item,
                                  int oldx, int oldy, int newx, int newy)
{
    tile_t *old_tile;
    tile_t *new_tile;

    assert(item); /* item must not be a NULL pointer */

    /* Co-ordinates must be valid */
    assert(oldx >= 0 && oldx < MAX_MAP_WIDTH);
    assert(oldy >= 0 && oldy < MAX_MAP_HEIGHT);
    assert(newx >= 0 && newx < MAX_MAP_WIDTH);
    assert(newy >= 0 && newy < MAX_MAP_HEIGHT);

    old_tile = &tiles[oldx][oldy];
    new_tile = &tiles[newx][newy];

    item->x = newx;
    item->y = newy;
    linkedlist_remove(&old_tile->items, item);
    linkedlist_prepend(&new_tile->items, item);
    return 1;
}