#include "packetsender.h"
#include "circularbuffer.h"
#include "entityutil.h"
#include "item.h"
#include "player.h"

#include <assert.h>
#include <string.h>

int send_screenshot(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 181);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_combat_style(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;
    cbuffer_create_frame(cbuf, 129);
    cbuffer_write_byte(cbuf, p->combat_style);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_fatigue(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;
    cbuffer_create_frame(cbuf, 126);
    cbuffer_write_short(cbuf, p->fatigue);
    cbuffer_format_packet(cbuf);
    return 1;
}

int hide_menu(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 127);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_menu(client_t *client, const char *options[], int opt_count)
{
    cbuffer_t *cbuf;
    int i;
    int len;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 223);
    cbuffer_write_byte(cbuf, opt_count);
    for (i = 0; i < opt_count; ++i) {
        len = strlen(options[i]);
        cbuffer_write_byte(cbuf, len);
        cbuffer_write_bytes(cbuf, options[i], len);
    }

    cbuffer_format_packet(cbuf);
    return 1;
}

int show_bank(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    int i;
    int size;

    assert(client); /* client must not be a NULL pointer */

    p = client->player;
    cbuf = &client->out_buffer;

    /* Find the size of the bank */
    for (size = 0; p->bank_item_ids[size] != 0; ++size);

    cbuffer_create_frame(cbuf, 93);
    cbuffer_write_byte(cbuf, size);
    cbuffer_write_byte(cbuf, MAX_BANK_SIZE);
    for (i = 0; i < size; ++i) {
        cbuffer_write_short(cbuf, p->bank_item_ids[i]);
        cbuffer_write_int(cbuf, p->bank_item_amounts[i]);
    }

    cbuffer_format_packet(cbuf);
    return 1;
}

int hide_bank(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 171);
    cbuffer_format_packet(cbuf);
    return 1;
}

int update_bank_item(client_t *client, int slot, int new_id, int amount)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 139);
    cbuffer_write_byte(cbuf, slot);
    cbuffer_write_short(cbuf, new_id);
    cbuffer_write_int(cbuf, amount);
    cbuffer_format_packet(cbuf);
    return 1;
}

int show_shop(client_t *client, int shop_id)
{
    return 0; /* TODO: Implement show_shop */
}

int hide_shop(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 220);
    cbuffer_format_packet(cbuf);
    return 1;
}

int start_shutdown(client_t *client, int seconds)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 172);
    cbuffer_write_short(cbuf, (int) (((double) seconds / 32) * 50));
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_alert(client_t *client, int is_big, const char *msg)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, is_big ? 64 : 148);
    cbuffer_write_bytes(cbuf, msg, strlen(msg));
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_sound(client_t *client, const char *sound_name)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 11);
    cbuffer_write_bytes(cbuf, sound_name, strlen(sound_name));
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_died(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 165);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_pm(client_t *client, uint64_t target_hash, uint8_t *msg, size_t sz)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 170);
    cbuffer_write_long(cbuf, target_hash);
    cbuffer_write_bytes(cbuf, msg, sz);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_friend_update(client_t *client, uint64_t target_hash, int world)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 25);
    cbuffer_write_long(cbuf, target_hash);
    cbuffer_write_byte(cbuf, world);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_friend_list(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    int size;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    for (size = 0; p->friend_list[size] != 0LL; ++size);

    cbuffer_create_frame(cbuf, 249);
    cbuffer_write_byte(cbuf, size);
    for (i = 0; i < size; ++i) {
        cbuffer_write_long(cbuf, p->friend_list[i]);
        cbuffer_write_byte(cbuf, 99); /* The world the friend is on */
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_ignore_list(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    int size;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    for (size = 0; p->ignore_list[size] != 0LL; ++size);

    cbuffer_create_frame(cbuf, 2);
    cbuffer_write_byte(cbuf, size);
    for (i = 0; i < size; ++i) {
        cbuffer_write_long(cbuf, p->ignore_list[i]);
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_trade_accept(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    player_t *t;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;
    t = p->trade_target;
    if (t == NULL) {
        return;
    }

    cbuffer_create_frame(cbuf, 251);
    cbuffer_write_long(cbuf, t->username_hash);

    /* TODO: Implement trading inventory, and fix this */
    cbuffer_write_byte(cbuf, 0); /* Size of target's trade offer */
    for (i = 0; i < 0; ++i) {
        cbuffer_write_short(cbuf, 0); /* Item id */
        cbuffer_write_int(cbuf, 0); /* Amount */
    }

    cbuffer_write_byte(cbuf, 0); /* Size of this player's trade offer */
    for (i = 0; i < 0; ++i) {
        cbuffer_write_short(cbuf, 0); /* Item id */
        cbuffer_write_int(cbuf, 0); /* Amount */
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_trade_accept_update(client_t *client)
{
    return 0; /* TODO: Implement send_trade_accept_update */
}

int send_trade_items(client_t *client)
{
    return 0; /* TODO: Implement send_trade_items */
}

int send_trade_window_open(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    player_t *t;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;
    t = p->trade_target;
    if (t == NULL) {
        return;
    }

    cbuffer_create_frame(cbuf, 4);
    cbuffer_write_short(cbuf, t->index);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_trade_window_close(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 187);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_duel_accept(client_t *client)
{
    return 0; /* TODO: Implement send_duel_accept */
}

int send_duel_accept_update(client_t *client)
{
    return 0; /* TODO: Implement send_duel_accept_update */
}

int send_duel_setting_update(client_t *client)
{
    return 0; /* TODO: Implement send_duel_setting_update */
}

int send_duel_items(client_t *client)
{
    return 0; /* TODO: Implement send_duel_items */
}

int send_duel_window_open(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    player_t *t;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;
    t = p->duel_target;
    if (t == NULL) {
        return;
    }

    cbuffer_create_frame(cbuf, 229);
    cbuffer_write_short(cbuf, t->index);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_duel_window_close(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 160);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_appearance_screen(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 207);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_server_info(client_t *client)
{
    cbuffer_t *cbuf;
    const char *location = "Australia";

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 110);
    cbuffer_write_long(cbuf, 0LL); /* The server's start time */
    cbuffer_write_bytes(cbuf, location, strlen(location));
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_tele_bubble(client_t *client, int x, int y, int should_grab)
{
    cbuffer_t *cbuf;
    player_t *p;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    cbuffer_create_frame(cbuf, 23);
    cbuffer_write_byte(cbuf, should_grab ? 1 : 0);
    cbuffer_write_byte(cbuf, x - p->x);
    cbuffer_write_byte(cbuf, y - p->y);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_message(client_t *client, const char *msg)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 48);
    cbuffer_write_bytes(cbuf, msg, strlen(msg));
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_remove_item(client_t *client, int slot)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 191);
    cbuffer_write_byte(cbuf, slot);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_update_item(client_t *client, int slot)
{
    cbuffer_t *cbuf;
    invitem_t *item;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    item = &client->player->inventory_items[slot];

    cbuffer_create_frame(cbuf, 228);
    cbuffer_write_byte(cbuf, slot);
    cbuffer_write_short(cbuf, item->item_id +
                        (item->flags & PF_ITEM_WIELDED ? 32768 : 0));

    if (item_stackable(item->item_id)) {
        cbuffer_write_int(cbuf, item->amount);
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_inventory(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    invitem_t *item;
    int size;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    for (size = 0; p->inventory_items[size].item_id != 0; ++size);

    cbuffer_create_frame(cbuf, 114);
    cbuffer_write_byte(cbuf, size);
    for (i = 0; i < size; ++i) {
        item = &p->inventory_items[i];
        cbuffer_write_short(cbuf, item->item_id +
                            (item->flags & PF_ITEM_WIELDED ? 32768 : 0));
        if (item_stackable(item->item_id)) {
            cbuffer_write_int(cbuf, item->amount);
        }
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_equipment_stats(client_t *client)
{
    return 0; /* TODO: Implement send_equipment_stats(client_t *) */
}

int send_stat(client_t *client, int stat)
{
    cbuffer_t *cbuf;
    player_t *p;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    cbuffer_create_frame(cbuf, 208);
    cbuffer_write_byte(cbuf, stat);
    cbuffer_write_byte(cbuf, p->cur_stats[stat]);
    cbuffer_write_byte(cbuf, p->max_stats[stat]);
    cbuffer_write_int(cbuf, p->experience[stat]);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_stats(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    cbuffer_create_frame(cbuf, 180);
    for (i = 0; i < 18; ++i) {
        cbuffer_write_byte(cbuf, p->cur_stats[i]);
    }
    for (i = 0; i < 18; ++i) {
        cbuffer_write_byte(cbuf, p->max_stats[i]);
    }
    for (i = 0; i < 18; ++i) {
        cbuffer_write_int(cbuf, p->experience[i]);
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_world_info(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    cbuffer_create_frame(cbuf, 131);
    cbuffer_write_short(cbuf, p->index);
    cbuffer_write_short(cbuf, 2304);
    cbuffer_write_short(cbuf, 1776);
    cbuffer_write_short(cbuf, get_height(p->y));
    cbuffer_write_short(cbuf, 944);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_prayers(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    cbuffer_create_frame(cbuf, 209);
    for (i = 0; i < 14; ++i) {
        cbuffer_write_byte(cbuf, player_prayer_enabled(p, i) ? 1 : 0);
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_game_settings(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    cbuffer_create_frame(cbuf, 152);
    cbuffer_write_byte(cbuf, player_game_setting_enabled(p, 0) ? 1 : 0);
    for (i = 2; i < 7; ++i) {
        cbuffer_write_byte(cbuf, player_game_setting_enabled(p, i) ? 1 : 0);
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_privacy_settings(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;
    int i;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    p = client->player;

    cbuffer_create_frame(cbuf, 158);
    for (i = 0; i < 4; ++i) {
        cbuffer_write_byte(cbuf, player_privacy_setting_enabled(p, i) ? 1 : 0);
    }
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_logout(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 222);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_cant_logout(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 136);
    cbuffer_format_packet(cbuf);
    return 1;
}

int send_login_box(client_t *client)
{
    cbuffer_t *cbuf;
    const char *last_ip = "133.33.33.37";

    assert(client); /* client must not be a NULL pointer */

    cbuf = &client->out_buffer;
    cbuffer_create_frame(cbuf, 248);
    cbuffer_write_short(cbuf, 1337); /* Days since last login */
    cbuffer_write_short(cbuf, 0); /* Days of subscription left */
    cbuffer_write_bytes(cbuf, last_ip, strlen(last_ip));
    cbuffer_format_packet(cbuf);
    return 1;
}