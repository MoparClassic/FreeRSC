#include "packetsender.h"
#include "circularbuffer.h"
#include "player.h"

#include <assert.h>
#include <string.h>

int send_screenshot(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = client->out_buffer;
    cbuffer_create_frame(cbuf, 181);
    cbuffer_send(cbuf);
    return 1;
}

int send_combat_style(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;

    assert(client); /* client must not be a NULL pointer */

    cbuf = client->out_buffer;
    p = client->player;
    cbuffer_create_frame(cbuf, 129);
    cbuffer_write_byte(cbuf, p->combat_style);
    cbuffer_send(cbuf);
    return 1;
}

int send_fatigue(client_t *client)
{
    cbuffer_t *cbuf;
    player_t *p;

    assert(client); /* client must not be a NULL pointer */

    cbuf = client->out_buffer;
    p = client->player;
    cbuffer_create_frame(cbuf, 126);
    cbuffer_write_short(cbuf, p->fatigue);
    cbuffer_send(cbuf);
    return 1;
}

int hide_menu(client_t *client)
{
    cbuffer_t *cbuf;

    assert(client); /* client must not be a NULL pointer */

    cbuf = client->out_buffer;
    cbuffer_create_frame(cbuf, 127);
    cbuffer_send(cbuf);
    return 1;
}

int send_menu(client_t *client, const char *options[], int opt_count)
{
    cbuffer_t *cbuf;
    int i;
    int len;

    assert(client); /* client must not be a NULL pointer */

    cbuf = client->out_buffer;
    cbuffer_create_frame(cbuf, 223);
    cbuffer_write_byte(cbuf, opt_count);
    for (i = 0; i < opt_count; ++i) {
        len = strlen(options[i]);
        cbuffer_write_byte(cbuf, len);
        cbuffer_write_bytes(cbuf, options[i], len);
    }
}

int show_bank(client_t *client)
{

}

int hide_bank(client_t *client);
int update_bank_item(client_t *client, int slot, int new_id, int amount);
int show_shop(client_t *client, int shop_id); /* TODO: Implement shops */
int hide_shop(client_t *client);
int start_shutdown(client_t *client);
int send_alert(client_t *client, int is_big, const char *msg);
int send_sound(client_t *client, const char *sound_name);
int send_died(client_t *client);
int send_pm(client_t *client, uint64_t target_hash, uint8_t *msg);
int send_friend_update(client_t *client, uint64_t target_hash, int world);
int send_friend_list(client_t *client);
int send_ignore_list(client_t *client);
int send_trade_accept(client_t *client);
int send_trade_accept_update(client_t *client);
int send_trade_items(client_t *client);
int send_trade_window_open(client_t *client);
int send_trade_window_close(client_t *client);
int send_duel_accept(client_t *client);
int send_duel_accept_update(client_t *client);
int send_duel_setting_update(client_t *client);
int send_duel_items(client_t *client);
int send_duel_window_open(client_t *client);
int send_duel_window_close(client_t *client);
int send_appearance_screen(client_t *client);
int send_tele_bubble(client_t *client, int x, int y, int should_grab);
int send_message(client_t *client, const char *msg);
int send_remove_item(client_t *client, int slot);
int send_update_item(client_t *client, int slot);
int send_inventory(client_t *client);
int send_equipment_stats(client_t *client);
int send_stat(client_t *client, int stat);
int send_stats(client_t *client);
int send_world_info(client_t *client);
int send_prayers(client_t *client);
int send_game_settings(client_t *client);
int send_privacy_settings(client_t *client);
int send_logout(client_t *client);
int send_cant_logout(client_t *client);
int send_login_box(client_t *client);