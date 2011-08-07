/* 
 * File:   playerutil.h
 * Author: lothy
 *
 * Created on 7 August 2011, 7:59 PM
 */

#ifndef PLAYERUTIL_H
#define	PLAYERUTIL_H

#include "item.h"
#include "player.h"
#include "linkedlist.h"

int player_revalidate_watched_objects(player_t *player);
int player_revalidate_watched_items(player_t *player);
int player_revalidate_watched_npcs(player_t *player);
int player_update_viewed_objects(player_t *player);
int player_update_viewed_items(player_t *player);
int player_update_viewed_npcs(player_t *player);
int player_get_players_in_view(player_t *player, linkedlist_t *player_list);
int player_get_objects_in_view(player_t *player, linkedlist_t *object_list);
int player_get_items_in_view(player_t *player, linkedlist_t *item_list);
int player_get_npcs_in_view(player_t *player, linkedlist_t *npc_list);

int player_can_see_item(player_t *player, grounditem_t *item);

#endif	/* PLAYERUTIL_H */

