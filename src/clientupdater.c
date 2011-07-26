#include "clientupdater.h"

#include "bubble.h"
#include "chatmessage.h"
#include "circularbuffer.h"
#include "client.h"
#include "common.h"
#include "gameobject.h"
#include "linkedlist.h"
#include "npc.h"
#include "player.h"
#include "projectile.h"
#include "util.h"
#include "world.h"

static void update_npc_positions();
static void update_player_positions();
static void update_message_queues();
static void update_offers();
static void update_collections();
static void update_timeouts(client_t *client);
static int send_player_position_packet(client_t *client);
static int send_npc_position_packet(client_t *client);
static int send_game_object_position_packet(client_t *client);
static int send_wall_object_position_packet(client_t *client);
static int send_item_position_packet(client_t *client);
static int send_player_update_packet(client_t *client);
static int send_npc_update_packet(client_t *client);

int send_client_updates()
{
    client_t *client;
    lnode_t *node;

    update_npc_positions();
    update_player_positions();
    update_message_queues();
    update_offers();

    LINKEDLIST_FOREACH(node, &client_list) {
        client = (client_t *) node->data;

        send_player_position_packet(client);
        send_npc_position_packet(client);
        send_game_object_position_packet(client);
        send_wall_object_position_packet(client);
        send_item_position_packet(client);

        send_player_update_packet(client);
        send_npc_update_packet(client);
    }
    return 0;
}

static void
update_npc_positions()
{
    npc_t *npc;

    NPC_LIST_FOREACH(npc) {
        npc_reset_moved(npc);
        npc_update_position(npc);
        npc_update_appearance_id(npc);
    }
}

static void
update_player_positions()
{
    player_t *player;
    lnode_t *node;

    LINKEDLIST_FOREACH(node, &client_list) {
        player = ((client_t *) node)->player;

        player_reset_moved(player);
        player_update_position(player);
        player_update_appearance_id(player);
    }

    LINKEDLIST_FOREACH(node, &client_list) {
        player = ((client_t *) node)->player;

        player_revalidate_watched_players(player);
        player_revalidate_watched_objects(player);
        player_revalidate_watched_items(player);
        player_revalidate_watched_npcs(player);
        player_update_viewed_players(player);
        player_update_viewed_objects(player);
        player_update_viewed_items(player);
        player_update_viewed_npcs(player);
    }
}

static void
update_message_queues()
{
    player_t *player;
    chatmessage_t *msg;

    PLAYER_LIST_FOREACH(player) {
        msg = linkedlist_poll(&player->chat_messages_to_display);
        if (msg == NULL || !(player->flags & PF_LOGGED_IN)) {
            continue;
        }


    }
}

static void
update_offers()
{
    player_t *player;
    player_t *target;

    PLAYER_LIST_FOREACH(player) {
        if (!(player->flags & PF_REQUIRES_OFFER_UPDATE)) {
            continue;
        }
        player_unset_flags(player, PF_REQUIRES_OFFER_UPDATE);

        if (player->flags & PF_TRADING) {
            target = player->trade_target;
            if (target == NULL) {
                continue;
            }

            // TODO: Send trade items to target player
        }
        else if (player->flags & PF_DUELING) {
            target = player->duel_target;
            if (target == NULL) {
                continue;
            }

            // TODO: Send duel settings to the player
            // TODO: Send duel settings, and offered items to target
        }
    }
}

static void
update_collections()
{
    player_t *p;
    npc_t *n;

    PLAYER_LIST_FOREACH(p) {
        if ((p->flags & EF_REMOVING) &&
            (p->flags & EF_INITIALISED)) {
            world_unregister_player(p);
        }
    }

    PLAYER_LIST_FOREACH(p) {
        eslist_update(&p->watched_players);
        eslist_update(&p->watched_objects);
        eslist_update(&p->watched_items);
        eslist_update(&p->watched_npcs);

        linkedlist_clear(&p->projectiles_to_display);
        linkedlist_clear(&p->hit_updates_to_display);
        linkedlist_clear(&p->npc_hit_updates_to_display);
        linkedlist_clear(&p->chat_messages_to_display);
        linkedlist_clear(&p->npc_chat_messages_to_display);
        linkedlist_clear(&p->bubbles_to_display);

        player_unset_flags(p, EF_SPRITE_CHANGED | EF_APPEARANCE_CHANGED);
    }

    NPC_LIST_FOREACH(n) {
        npc_unset_flags(n, EF_SPRITE_CHANGED | EF_APPEARANCE_CHANGED);
    }
}

static void
update_timeouts(client_t *client)
{
    player_t *p = client->player;

    if (p->flags & EF_DESTROYED) {
        return;
    }

    if (timestamp - p->last_ping > 30000) {
        player_destroy(p);
    }
    else if (p->flags & PF_WARNED_TO_MOVE) {
        if (timestamp - p->last_moved >= 960000 && p->flags & PF_LOGGED_IN) {
            player_destroy(p);
        }
    }
    else if (timestamp - p->last_moved >= 900000) {
        // TODO: Send message to player telling them to move
        player_set_flags(p, PF_WARNED_TO_MOVE);
    }
}

static int
send_player_position_packet(client_t *client)
{
    player_t *player = client->player;
    eslist_t *sl = &player->watched_players;
    linkedlist_t *known_players = &sl->known_entities;
    linkedlist_t *new_players = &sl->new_entities;
    cbuffer_t *cbuf = &client->out_buffer;
    lnode_t *node;
    player_t *target;
    int px = player->x;
    int py = player->y;

    cbuffer_create_frame(cbuf, 145);
    cbuffer_init_bit_access(cbuf);
    cbuffer_write_bits(cbuf, px, 11);
    cbuffer_write_bits(cbuf, py, 13);
    cbuffer_write_bits(cbuf, player->sprite, 4);
    cbuffer_write_bits(cbuf, linkedlist_size(known_players), 8);

    LINKEDLIST_FOREACH(node, known_players) {
        target = (player_t *) node->data;
        if (player == target) {
            continue;
        }

        if (eslist_isremoving(sl, target)) {
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 12, 4);
        }
        else if (target->flags & EF_MOVED) {
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 0, 1);
            cbuffer_write_bits(cbuf, target->sprite, 3);
        }
        else if (target->flags & EF_SPRITE_CHANGED) {
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, target->sprite, 4);
        }
        else {
            cbuffer_write_bits(cbuf, 0, 1);
        }
    }

    LINKEDLIST_FOREACH(node, new_players) {
        target = (player_t *) node->data;
        cbuffer_write_bits(cbuf, target->index, 16);
        cbuffer_write_bits(cbuf, target->x - px, 5);
        cbuffer_write_bits(cbuf, target->y - py, 5);
        cbuffer_write_bits(cbuf, target->sprite, 4);
        cbuffer_write_bits(cbuf, 0, 1);
    }

    cbuffer_finish_bit_access(cbuf);
    cbuffer_send(cbuf);
    return 0;
}

static int
send_npc_position_packet(client_t *client)
{
    player_t *player = client->player;
    eslist_t *sl = &player->watched_npcs;
    linkedlist_t *known_npcs = &sl->known_entities;
    linkedlist_t *new_npcs = &sl->new_entities;
    cbuffer_t *cbuf = &client->out_buffer;
    lnode_t *node;
    npc_t *npc;
    int px = player->x;
    int py = player->y;

    cbuffer_create_frame(cbuf, 77);
    cbuffer_init_bit_access(cbuf);
    cbuffer_write_bits(cbuf, linkedlist_size(known_npcs), 8);

    LINKEDLIST_FOREACH(node, known_npcs) {
        npc = (npc_t *) node->data;
        cbuffer_write_bits(cbuf, npc->npc_id, 16);
        if (eslist_isremoving(sl, npc)) {
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 12, 4);
        }
        else if (npc->flags & EF_MOVED) {
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 0, 1);
            cbuffer_write_bits(cbuf, npc->sprite, 3);
        }
        else if (npc->flags & EF_SPRITE_CHANGED) {
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, 1, 1);
            cbuffer_write_bits(cbuf, npc->sprite, 4);
        }
        else {
            cbuffer_write_bits(cbuf, 0, 1);
        }
    }

    LINKEDLIST_FOREACH(node, new_npcs) {
        cbuffer_write_bits(cbuf, npc->index, 16);
        cbuffer_write_bits(cbuf, npc->x - px, 5);
        cbuffer_write_bits(cbuf, npc->y - py, 5);
        cbuffer_write_bits(cbuf, npc->sprite, 4);
        cbuffer_write_bits(cbuf, npc->npc_id, 10);
    }

    cbuffer_finish_bit_access(cbuf);
    cbuffer_send(cbuf);
    return 0;
}

static int
send_game_object_position_packet(client_t *client)
{
    player_t *player = client->player;
    eslist_t *sl = &player->watched_objects;
    linkedlist_t *known_objects = &sl->known_entities;
    linkedlist_t *new_objects = &sl->new_entities;
    cbuffer_t *cbuf = &client->out_buffer;
    lnode_t *node;
    gameobject_t *go;
    int px = player->x;
    int py = player->y;

    cbuffer_create_frame(cbuf, 27);

    LINKEDLIST_FOREACH(node, known_objects) {
        go = (gameobject_t *) node->data;
        if (gameobject_get_type(go->object_id) != 0) {
            continue;
        }

        if (eslist_isremoving(sl, go)) {
            cbuffer_write_short(cbuf, 60000);
            cbuffer_write_byte(cbuf, go->x - px);
            cbuffer_write_byte(cbuf, go->y - py);
            cbuffer_write_byte(cbuf, gameobject_get_dir(go->object_id));
        }
    }

    LINKEDLIST_FOREACH(node, new_objects) {
        go = (gameobject_t *) node->data;
        if (gameobject_get_type(go->object_id) != 0) {
            continue;
        }

        cbuffer_write_short(cbuf, go->object_id);
        cbuffer_write_byte(cbuf, go->x - px);
        cbuffer_write_byte(cbuf, go->y - py);
        cbuffer_write_byte(cbuf, gameobject_get_dir(go->object_id));
    }

    cbuffer_send(cbuf);
    return 0;
}

static int
send_wall_object_position_packet(client_t *client)
{
    player_t *player = client->player;
    eslist_t *sl = &player->watched_objects;
    linkedlist_t *known_objects = &sl->known_entities;
    linkedlist_t *new_objects = &sl->new_entities;
    cbuffer_t *cbuf = &client->out_buffer;
    lnode_t *node;
    gameobject_t *go;
    int px = player->x;
    int py = player->y;

    cbuffer_create_frame(cbuf, 95);

    LINKEDLIST_FOREACH(node, known_objects) {
        go = (gameobject_t *) node->data;
        if (gameobject_get_type(go->object_id) != 1) {
            continue;
        }

        if (eslist_isremoving(sl, go)) {
            cbuffer_write_short(cbuf, 60000);
            cbuffer_write_byte(cbuf, go->x - px);
            cbuffer_write_byte(cbuf, go->y - py);
            cbuffer_write_byte(cbuf, gameobject_get_dir(go->object_id));
        }
    }

    LINKEDLIST_FOREACH(node, new_objects) {
        go = (gameobject_t *) node->data;
        if (gameobject_get_type(go->object_id) != 1) {
            continue;
        }

        cbuffer_write_short(cbuf, go->object_id);
        cbuffer_write_byte(cbuf, go->x - px);
        cbuffer_write_byte(cbuf, go->y - py);
        cbuffer_write_byte(cbuf, gameobject_get_dir(go->object_id));
    }

    cbuffer_send(cbuf);
    return 0;
}

static int
send_item_position_packet(client_t *client)
{
    player_t *player = client->player;
    eslist_t *sl = &player->watched_items;
    linkedlist_t *known_items = &sl->known_entities;
    linkedlist_t *new_items = &sl->new_entities;
    cbuffer_t *cbuf = &client->out_buffer;
    lnode_t *node;
    grounditem_t *item;
    int px = player->x;
    int py = player->y;

    cbuffer_create_frame(cbuf, 109);

    LINKEDLIST_FOREACH(node, known_items) {
        if (eslist_isremoving(sl, node->data)) {
            item = (grounditem_t *) node->data;
            cbuffer_write_short(cbuf, item->item_id + 32768);
            cbuffer_write_byte(cbuf, item->x - px);
            cbuffer_write_byte(cbuf, item->y - py);
        }
    }

    LINKEDLIST_FOREACH(node, new_items) {
        item = (grounditem_t *) node->data;
        cbuffer_write_short(cbuf, item->item_id);
        cbuffer_write_byte(cbuf, item->x - px);
        cbuffer_write_byte(cbuf, item->y - py);
    }

    cbuffer_send(cbuf);
    return 0;
}

static int
send_player_update_packet(client_t *client)
{
    int update_size;
    cbuffer_t *cbuf = &client->out_buffer;
    player_t *player = client->player;
    lnode_t *node;
    bubble_t *bubble;
    chatmessage_t *chatmsg;
    projectile_t *projectile;
    player_t *ptarget;
    size_t len;
    int i;

    update_size = linkedlist_size(&player->bubbles_to_display) +
                  linkedlist_size(&player->chat_messages_to_display) +
                  linkedlist_size(&player->hit_updates_to_display) +
                  linkedlist_size(&player->projectiles_to_display) +
                  linkedlist_size(&player->appearance_updates_to_display);

    if (update_size > 0) {
        cbuffer_create_frame(cbuf, 53);
        cbuffer_write_short(cbuf, update_size);

        // 1: Bubbles

        LINKEDLIST_FOREACH(node, &player->bubbles_to_display) {
            bubble = (bubble_t *) node->data;
            cbuffer_write_short(cbuf, bubble->player_index);
            cbuffer_write_byte(cbuf, 0);
            cbuffer_write_short(cbuf, bubble->item_id);
        }

        // 2: Chat messages

        LINKEDLIST_FOREACH(node, &player->chat_messages_to_display) {
            chatmsg = (chatmessage_t *) node->data;
            len = strlen(chatmsg->message);

            cbuffer_write_short(cbuf, chatmsg->sender_index);
            cbuffer_write_byte(cbuf, chatmsg->recipient_index == 0 ? 1 : 6);
            cbuffer_write_byte(cbuf, len);
            cbuffer_write_bytes(cbuf, chatmsg->message, len);
        }

        // 3: Player hit updates

        LINKEDLIST_FOREACH(node, &player->hit_updates_to_display) {
            ptarget = (player_t *) node->data;
            cbuffer_write_short(cbuf, ptarget->index);
            cbuffer_write_byte(cbuf, 2);
            cbuffer_write_byte(cbuf, ptarget->last_damage);
            cbuffer_write_byte(cbuf, ptarget->cur_stats[3]);
            cbuffer_write_byte(cbuf, ptarget->max_stats[3]);
        }

        // 4: Projectiles

        LINKEDLIST_FOREACH(node, &player->projectiles_to_display) {
            projectile = (projectile_t *) node->data;
            if (projectile->victim_type == ENTITY_NPC) { // Npc victim
                cbuffer_write_short(cbuf, projectile->caster_index);
                cbuffer_write_byte(cbuf, 3);
                cbuffer_write_short(cbuf, projectile->projectile_type);
                cbuffer_write_short(cbuf, projectile->victim_index);
            }
            else if (projectile->victim_type == ENTITY_PLAYER) { //Player victim
                cbuffer_write_short(cbuf, projectile->caster_index);
                cbuffer_write_byte(cbuf, 4);
                cbuffer_write_short(cbuf, projectile->projectile_type);
                cbuffer_write_short(cbuf, projectile->victim_index);
            }
        }

        // 5: Player appearance updates

        LINKEDLIST_FOREACH(node, &player->appearance_updates_to_display) {
            ptarget = (player_t *) node->data;
            cbuffer_write_short(cbuf, ptarget->index);
            cbuffer_write_byte(cbuf, 5);
            cbuffer_write_short(cbuf, 0); // Appearance id
            cbuffer_write_long(cbuf, ptarget->username_hash);
            cbuffer_write_byte(cbuf, 12); // Number of items worn
            for (i = 0; i < 12; ++i) {
                cbuffer_write_byte(cbuf, ptarget->worn_items[i]);
            }
            cbuffer_write_byte(cbuf, ptarget->hair_colour);
            cbuffer_write_byte(cbuf, ptarget->top_colour);
            cbuffer_write_byte(cbuf, ptarget->trouser_colour);
            cbuffer_write_byte(cbuf, ptarget->skin_colour);
            cbuffer_write_byte(cbuf, ptarget->combat_level);
            cbuffer_write_byte(cbuf, ptarget->skulled ? 1 : 0);
            cbuffer_write_byte(cbuf, player_get_crown(ptarget));
        }

        cbuffer_send(cbuf);
    }
    return 0;
}

static int
send_npc_update_packet(client_t *client)
{
    int update_size;
    player_t *player = client->player;
    cbuffer_t *cbuf = &client->out_buffer;
    lnode_t *node;
    chatmessage_t *chatmsg;
    npc_t *npc;
    size_t len;


    update_size = linkedlist_size(&player->npc_hit_updates_to_display) +
                  linkedlist_size(&player->npc_chat_messages_to_display);

    if (update_size > 0) {
        cbuffer_create_frame(cbuf, 190);
        cbuffer_write_short(cbuf, update_size);

        LINKEDLIST_FOREACH(node, &player->npc_chat_messages_to_display) {
            chatmsg = (chatmessage_t *) node->data;
            len = strlen(chatmsg->message);

            cbuffer_write_short(cbuf, chatmsg->sender_index);
            cbuffer_write_byte(cbuf, 1);
            cbuffer_write_short(cbuf, chatmsg->recipient_index);
            cbuffer_write_byte(cbuf, len);
            cbuffer_write_bytes(cbuf, chatmsg->message, len);
        }

        LINKEDLIST_FOREACH(node, &player->npc_hit_updates_to_display) {
            npc = (npc_t *) node->data;
            cbuffer_write_short(cbuf, npc->index);
            cbuffer_write_byte(cbuf, 2);
            cbuffer_write_byte(cbuf, npc->last_damage);
            cbuffer_write_byte(cbuf, npc->cur_hitpoints);
            cbuffer_write_byte(cbuf, npcdef_get_hitpoints(npc->npc_id));
        }

        cbuffer_send(cbuf);
    }
    return 0;
}