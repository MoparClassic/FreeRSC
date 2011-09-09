/*
 * File:   packetsender.h
 * Author: lothy
 *
 * Created on 8 August 2011, 3:27 PM
 */

#ifndef PACKETSENDER_H
#define	PACKETSENDER_H

#include "client.h"

#include <stdint.h>
#include <stdlib.h>

int send_screenshot(client_t *client);
int send_combat_style(client_t *client);
int send_fatigue(client_t *client);
int hide_menu(client_t *client);
int send_menu(client_t *client, const char *options[], int opt_count);
int show_bank(client_t *client);
int hide_bank(client_t *client);
int update_bank_item(client_t *client, int slot, int new_id, int amount);
int show_shop(client_t *client, int shop_id); /* TODO: Implement shops */
int hide_shop(client_t *client);
int start_shutdown(client_t *client, int seconds);
int send_alert(client_t *client, int is_big, const char *msg);
int send_sound(client_t *client, const char *sound_name);
int send_died(client_t *client);
int send_pm(client_t *client, uint64_t target_hash, uint8_t *msg, size_t sz);
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
int send_server_info(client_t *client);
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

#endif	/* PACKETSENDER_H */

