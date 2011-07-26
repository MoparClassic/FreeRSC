#include "gameobject.h"
#include "item.h"
#include "npc.h"
#include "player.h"
#include "world.h"

static int remove_known_appearance(player_t *player, player_t *target);
static int within_range(player_t *player, int x, int y);
static int can_see_item(player_t *player, grounditem_t *item);
static int inform_of_player(player_t *player, player_t *target);
static int player_get_viewed_area(player_t *player, linkedlist_t *tile_list,
                                  int x1, int y1, int x2, int y2);

int
player_get_crown(player_t *player)
{
    if (PF_DISPLAY_CROWN & player->flags) {
        if (PF_ADMIN_CROWN_ALLOWED & player->flags) {
            return 3;
        }
        else if (PF_MOD_CROWN_ALLOWED & player->flags) {
            return 2;
        }
        else if (PF_PMOD_CROWN_ALLOWED & player->flags) {
            return 1;
        }
    }
    return 0; // 0 is no crown
}

int player_set_flags(player_t *player, int flags)
{
    player->flags |= flags;
    return 1;
}

int player_unset_flags(player_t *player, int flags)
{
    player->flags &= ~flags;
    return 1;
}

int player_reset_moved(player_t *player)
{
    return player_unset_flags(player, EF_MOVED);
}

int player_update_position(player_t *player)
{
    return 0;
}

int player_update_appearance_id(player_t *player)
{
    if (EF_APPEARANCE_CHANGED & player->flags) {
        ++player->appearance_id;
    }
    return 1;
}

int player_revalidate_watched_players(player_t *player)
{
    eslist_t *watched_players = &player->watched_players;
    lnode_t *node = watched_players->known_entities.head;
    player_t *target;

    for (; NULL != node; node = node->next) {
        target = (player_t *) node->data;

        if (0 == (PF_LOGGED_IN & target->flags)) {
            // Player is no longer logged in -- remove them
            eslist_remove(watched_players, target);
            remove_known_appearance(player, target);
        }
        else if (within_range(player, target->x, target->y) == 0) {
            // If the player is no longer within range, remove them
            eslist_remove(watched_players, target);
            remove_known_appearance(player, target);
        }
    }
    return 1;
}

int player_revalidate_watched_objects(player_t *player)
{
    eslist_t *watched_objects = &player->watched_objects;
    lnode_t *node = watched_objects->known_entities.head;
    gameobject_t *go;

    for (; NULL != node; node = node->next) {
        go = (gameobject_t *) node->data;

        if (EF_REMOVING & go->flags) {
            // Object is being removed from the game -- remove it
            eslist_remove(watched_objects, go);
        }
        else if (within_range(player, go->x, go->y) == 0) {
            // Object is no longer within range -- remove it
            eslist_remove(watched_objects, go);
        }
    }
    return 1;
}

int player_revalidate_watched_items(player_t *player)
{
    eslist_t *watched_items = &player->watched_items;
    lnode_t *node = watched_items->known_entities.head;
    grounditem_t *item;

    for (; NULL != node; node = node->next) {
        item = (grounditem_t *) node->data;

        if (EF_REMOVING & item->flags) {
            // Item removed from game world -- remove from view
            eslist_remove(watched_items, item);
        }
        else if (within_range(player, item->x, item->y) == 0) {
            // Item no longer in range -- remove it
            eslist_remove(watched_items, item);
        }
        else if (can_see_item(player, item) == 0) {
            // Item can not be seen -- remove it
            eslist_remove(watched_items, item);
        }
    }
    return 1;
}

int player_revalidate_watched_npcs(player_t *player)
{
    eslist_t *watched_npcs = &player->watched_npcs;
    lnode_t *node = watched_npcs->known_entities.head;
    npc_t *npc;

    for (; NULL != node; node = node->next) {
        npc = (npc_t *) node->data;

        if (EF_REMOVING & npc->flags) {
            // NPC removed from game world -- remove from view
            eslist_remove(watched_npcs, npc);
        }
        else if (within_range(player, npc->x, npc->y) == 0) {
            // Npc no longer within range -- remove it
            eslist_remove(watched_npcs, npc);
        }
    }
    return 1;
}

int player_update_viewed_players(player_t *player)
{
    linkedlist_t visible_players;
    player_t *target;
    lnode_t *node;

    memset(&visible_players, 0, sizeof(linkedlist_t));
    player_get_players_in_view(player, &visible_players);

    LINKEDLIST_FOREACH(node, &visible_players) {
        target = (player_t *) node->data;
        if (player != target && (PF_LOGGED_IN &target->flags)) {
            inform_of_player(player, target);
            inform_of_player(target, player);
        }
    }
    linkedlist_clear(&visible_players);
    return 1;
}

int player_update_viewed_objects(player_t *player)
{
    eslist_t *watched_objects = &player->watched_objects;
    linkedlist_t visible_objects;
    gameobject_t *go;
    lnode_t *node;

    memset(&visible_objects, 0, sizeof(linkedlist_t));
    player_get_objects_in_view(player, &visible_objects);

    LINKEDLIST_FOREACH(node, &visible_objects) {
        go = (gameobject_t *) node->data;

        // If the object is being removed, skip it
        if (EF_REMOVING & go->flags) {
            continue;
        }

        // If the object is not within range, skip it
        if (within_range(player, go->x, go->y) == 0) {
            continue;
        }

        // If we're already aware of the object, skip it
        if (eslist_contains(watched_objects, go) == 1) {
            continue;
        }

        eslist_add(watched_objects, go);
    }
    linkedlist_clear(&visible_objects);
    return 1;
}

int player_update_viewed_items(player_t *player)
{
    eslist_t *watched_items = &player->watched_items;
    linkedlist_t visible_items;
    grounditem_t *item;
    lnode_t *node;

    memset(&visible_items, 0, sizeof(linkedlist_t));
    player_get_items_in_view(player, &visible_items);

    LINKEDLIST_FOREACH(node, &visible_items) {
        item = (grounditem_t *) node->data;

        // If the item is being removed, skip it
        if (EF_REMOVING & item->flags) {
            continue;
        }

        // If the item is not within range, skip it
        if (within_range(player, item->x, item->y) == 0) {
            continue;
        }

        // If we can't see the item, skip it
        if (can_see_item(player, item) == 0) {
            continue;
        }

        // If we're already aware of the item, skip it
        if (eslist_contains(watched_items, item) == 1) {
            continue;
        }

        eslist_add(watched_items, item);
    }
    linkedlist_clear(&visible_items);
    return 1;
}

int player_update_viewed_npcs(player_t *player)
{
    eslist_t *watched_npcs = &player->watched_npcs;
    linkedlist_t visible_npcs;
    npc_t *npc;
    lnode_t *node;

    memset(&visible_npcs, 0, sizeof(linkedlist_t));
    player_get_npcs_in_view(player, &visible_npcs);

    LINKEDLIST_FOREACH(node, &visible_npcs) {
        npc = (npc_t *) node->data;

        // If the npc is not within range, skip it
        if (within_range(player, npc->x, npc->y) == 0) {
            continue;
        }

        if (eslist_contains(watched_npcs, npc) == 0) {
            eslist_add(watched_npcs, npc);
        }
        else if (eslist_isremoving(watched_npcs, npc) == 1) {
            eslist_add(watched_npcs, npc);
        }
    }
    linkedlist_clear(&visible_npcs);
    return 0;
}

int player_get_players_in_view(player_t *player, linkedlist_t *player_list)
{
    linkedlist_t tile_list;
    lnode_t *tile_node, *player_node;
    player_t *target;
    tile_t *tile;
    int i = 0;

    memset(&tile_list, 0, sizeof(linkedlist_t));
    player_get_viewed_area(player, &tile_list, 15, 15, 16, 16);

    LINKEDLIST_FOREACH(tile_node, &tile_list) {
        tile = (tile_t *) tile_node->data;
        ++i;

        LINKEDLIST_FOREACH(player_node, &tile->players) {
            target = (player_t *) player_node->data;
            linkedlist_prepend(player_list, target);
        }
    }
    linkedlist_clear(&tile_list);
    return 1;
}

int player_get_objects_in_view(player_t *player, linkedlist_t *object_list)
{
    linkedlist_t tile_list;
    lnode_t *tile_node;
    tile_t *tile;

    memset(&tile_list, 0, sizeof(linkedlist_t));
    player_get_viewed_area(player, &tile_list, 21, 21, 21, 21);

    LINKEDLIST_FOREACH(tile_node, &tile_list) {
        tile = (tile_t *) tile_node->data;
        linkedlist_prepend(object_list, &tile->object);
    }
    linkedlist_clear(&tile_list);
    return 1;
}

int player_get_items_in_view(player_t *player, linkedlist_t *item_list)
{
    linkedlist_t tile_list;
    lnode_t *tile_node, *item_node;
    grounditem_t *item;
    tile_t *tile;

    memset(&tile_list, 0, sizeof(linkedlist_t));
    player_get_viewed_area(player, &tile_list, 21, 21, 21, 21);

    LINKEDLIST_FOREACH(tile_node, &tile_list) {
        tile = (tile_t *) tile_node->data;

        LINKEDLIST_FOREACH(item_node, &tile->items) {
            item = (grounditem_t *) item_node->data;
            linkedlist_prepend(item_list, item);
        }
    }
    linkedlist_clear(&tile_list);
    return 0;
}

int player_get_npcs_in_view(player_t *player, linkedlist_t *npc_list)
{
    linkedlist_t tile_list;
    lnode_t *tile_node, *npc_node;
    npc_t *npc;
    tile_t *tile;

    memset(&tile_list, 0, sizeof(linkedlist_t));
    player_get_viewed_area(player, &tile_list, 15, 15, 16, 16);

    LINKEDLIST_FOREACH(tile_node, &tile_list) {
        tile = (tile_t *) tile_node->data;

        LINKEDLIST_FOREACH(npc_node, &tile->npcs) {
            npc = (npc_t *) npc_node->data;
            linkedlist_prepend(npc_list, npc);
        }
    }
    linkedlist_clear(&tile_list);
    return 0;
}

int player_destroy(player_t *player)
{
    return 0;
}

static int
remove_known_appearance(player_t *player, player_t *target)
{
    linkedlist_t *known_app_ids = &player->known_appearance_ids;
    appearance_mapping_t *amapping;
    lnode_t *cur = known_app_ids->head;
    lnode_t *prev = known_app_ids->head;

    while (NULL != cur) {
        amapping = (appearance_mapping_t *) cur->data;
        if (target != amapping->player) {
            prev = cur;
            cur = cur->next;
            continue;
        }
        else {
            if (cur != known_app_ids->head) {
                prev->next = cur->next;
                lnode_t_free(cur);
                --known_app_ids->size;
            }
            else {
                known_app_ids->head = cur->next;
                lnode_t_free(cur);
                --known_app_ids->size;
            }
            return 1;
        }
    }
    return 0;
}

static int
within_range(player_t *player, int x, int y)
{
    int xdif = player->x - x;
    int ydif = player->y - y;
    return xdif <= 16 && xdif >= -15 &&
           ydif <= 16 && ydif >= -15;
}

static int
can_see_item(player_t *player, grounditem_t *item)
{
    if (0 == item->dropper ||
        player->username_hash == item->dropper) {
        return 1;
    }
    return (timestamp - item->spawn_time) > 60000;
}

static int
inform_of_player(player_t *player, player_t *target)
{
    eslist_t *watched_players = &player->watched_players;
    lnode_t *node = watched_players->known_entities.head;

    if ((!eslist_contains(watched_players, target) ||
         eslist_isremoving(watched_players, target)) &&
        within_range(player, target->x, target->y)) {
        eslist_add(watched_players, target);
    }
    return 0;
}

static int
player_get_viewed_area(player_t *player, linkedlist_t *tile_list,
                       int x1, int y1, int x2, int y2)
{
    int mob_x = player->x;
    int mob_y = player->y;
    int start_x, start_y, end_x, end_y;
    int x_width, y_width;
    int x, y;

    start_x = mob_x - x1;
    if (start_x < 0) {
        start_x = 0;
    }

    start_y = mob_y - y1;
    if (start_y < 0) {
        start_y = 0;
    }

    end_x = mob_x + x2;
    if (end_x >= 944) {
        end_x = 944 - 1;
    }

    end_y = mob_y + y2;
    if (end_y >= 3776) {
        end_y = 3776 - 1;
    }

    if (start_x > end_x) {
        x_width = start_x - end_x;
    }
    else {
        x_width = end_x - start_x;
    }

    if (start_y > end_y) {
        y_width = start_y - end_y;
    }
    else {
        y_width = end_y - start_y;
    }

    for (x = 0; (x + start_x) < end_x; ++x) {
        for (y = 0; (y + start_y) < end_y; ++y) {
            linkedlist_prepend(tile_list,
                               &tiles[x + start_x][y + start_y]);
        }
    }
}