#include "playerutil.h"
#include "common.h"
#include "entitystatelist.h"
#include "npc.h"
#include "tile.h"
#include "util.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int player_revalidate_watched_objects(player_t *player)
{
    eslist_t *watched_objects = &player->watched_objects;
    lnode_t *node = watched_objects->known_entities.head;
    gameobject_t *go;

    assert(player); /* player must not be a NULL pointer */

    for (; NULL != node; node = node->next) {
        go = (gameobject_t *) node->data;

        if (EF_REMOVING & go->flags) {
            /* Object is being removed from the game -- remove it */
            eslist_remove(watched_objects, go);
        }
        else if (within_range(player->x, player->y, go->x, go->y) == 0) {
            /* Object is no longer within range -- remove it */
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

    assert(player); /* player must not be a NULL pointer */

    for (; NULL != node; node = node->next) {
        item = (grounditem_t *) node->data;

        if (EF_REMOVING & item->flags) {
            /* Item removed from game world -- remove from view */
            eslist_remove(watched_items, item);
        }
        else if (within_range(player->x, player->y, item->x, item->y) == 0) {
            /* Item no longer in range -- remove it */
            eslist_remove(watched_items, item);
        }
        else if (player_can_see_item(player, item) == 0) {
            /* Item can not be seen -- remove it */
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

    assert(player); /* player must not be a NULL pointer */

    for (; NULL != node; node = node->next) {
        npc = (npc_t *) node->data;

        if (EF_REMOVING & npc->flags) {
            /* NPC removed from game world -- remove from view */
            eslist_remove(watched_npcs, npc);
        }
        else if (within_range(player->x, player->y, npc->x, npc->y) == 0) {
            /* Npc no longer within range -- remove it */
            eslist_remove(watched_npcs, npc);
        }
    }
    return 1;
}

int player_update_viewed_objects(player_t *player)
{
    eslist_t *watched_objects = &player->watched_objects;
    linkedlist_t visible_objects;
    gameobject_t *go;
    lnode_t *node;

    assert(player); /* player must not be a NULL pointer */

    memset(&visible_objects, 0, sizeof(linkedlist_t));
    player_get_objects_in_view(player, &visible_objects);

    LINKEDLIST_FOREACH(node, &visible_objects) {
        go = (gameobject_t *) node->data;

        /* If the object is being removed, skip it */
        if (EF_REMOVING & go->flags) {
            continue;
        }

        /* If the object is not within range, skip it */
        if (within_range(player->x, player->y, go->x, go->y) == 0) {
            continue;
        }

        /* If we're already aware of the object, skip it */
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

    assert(player); /* player must not be a NULL pointer */

    memset(&visible_items, 0, sizeof(linkedlist_t));
    player_get_items_in_view(player, &visible_items);

    LINKEDLIST_FOREACH(node, &visible_items) {
        item = (grounditem_t *) node->data;

        /* If the item is being removed, skip it */
        if (EF_REMOVING & item->flags) {
            continue;
        }

        /* If the item is not within range, skip it */
        if (within_range(player->x, player->y, item->x, item->y) == 0) {
            continue;
        }

        /* If we can't see the item, skip it */
        if (player_can_see_item(player, item) == 0) {
            continue;
        }

        /* If we're already aware of the item, skip it */
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

    assert(player); /* player must not be a NULL pointer */

    memset(&visible_npcs, 0, sizeof(linkedlist_t));
    player_get_npcs_in_view(player, &visible_npcs);

    LINKEDLIST_FOREACH(node, &visible_npcs) {
        npc = (npc_t *) node->data;

        /* If the npc is not within range, skip it */
        if (within_range(player->x, player->y, npc->x, npc->y) == 0) {
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

    assert(player); /* player must not be a NULL pointer */
    assert(player_list); /* player_list must not be a NULL pointer */

    memset(&tile_list, 0, sizeof(linkedlist_t));
    get_viewed_area(player->x, player->y, &tile_list, 15, 15, 16, 16);

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

    assert(player); /* player must not be a NULL pointer */
    assert(object_list); /* object_list must not be a NULL pointer */

    memset(&tile_list, 0, sizeof(linkedlist_t));
    get_viewed_area(player->x, player->y, &tile_list, 21, 21, 21, 21);

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

    assert(player); /* player must not be a NULL pointer */
    assert(item_list); /* item_list must not be a NULL pointer */

    memset(&tile_list, 0, sizeof(linkedlist_t));
    get_viewed_area(player->x, player->y, &tile_list, 21, 21, 21, 21);

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

    assert(player); /* player must not be a NULL pointer */
    assert(npc_list); /* npc_list must not be a NULL pointer */

    memset(&tile_list, 0, sizeof(linkedlist_t));
    get_viewed_area(player->x, player->y, &tile_list, 15, 15, 16, 16);

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

int
player_can_see_item(player_t *player, grounditem_t *item)
{
    assert(player); /* player must not be a NULL pointer */
    assert(item); /* item must not be a NULL pointer */

    if (0 == item->dropper ||
        player->username_hash == item->dropper) {
        return 1;
    }
    return (timestamp - item->spawn_time) > 60000;
}
