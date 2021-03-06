#ifndef INCLUDED_PLAYER_H
#define	INCLUDED_PLAYER_H

#include "linkedlist.h"
#include "entitystatelist.h"

#include <stdint.h>

#define MAX_BANK_SIZE       192
#define MAX_INVENTORY_SIZE  30
#define MAX_FRIENDS         100
#define MAX_IGNORES         100

#define PF_WARNED_TO_MOVE          (1 << 31)
#define PF_LOGGED_IN               (1 << 30)
#define PF_REQUIRES_OFFER_UPDATE   (1 << 29)
#define PF_TRADING                 (1 << 28)
#define PF_DUELING                 (1 << 27)
#define PF_BLOCK_PUBLIC_CHAT       (1 << 26)
#define PF_BLOCK_PRIVATE_CHAT      (1 << 25)
#define PF_BLOCK_DUEL_REQUESTS     (1 << 24)
#define PF_BLOCK_TRADE_REQUESTS    (1 << 23)
#define PF_ADMIN_CROWN_ALLOWED     (1 << 22)
#define PF_MOD_CROWN_ALLOWED       (1 << 21)
#define PF_PMOD_CROWN_ALLOWED      (1 << 20)
#define PF_DISPLAY_CROWN           (1 << 19)
#define PF_ITEM_WIELDED            (1 << 18)

typedef struct player player_t;
typedef struct appearance_mapping appearance_mapping_t;
typedef struct invitem invitem_t;

struct invitem {
    uint16_t item_id;
    uint32_t amount;
    uint32_t flags;
};

struct player {
    const char *username;
    uint64_t username_hash;
    uint64_t server_key;
    uint32_t flags;
    uint32_t appearance_id;
    uint16_t index;

    uint64_t last_ping;
    uint64_t last_moved;

    uint16_t x;
    uint16_t y;
    uint16_t sprite;

    uint8_t max_stats[18];
    uint8_t cur_stats[18];
    uint32_t experience[18];
    uint8_t combat_style;
    uint16_t fatigue;
    uint16_t prayers;
    uint16_t game_settings;
    uint16_t privacy_settings;

    uint64_t friend_list[MAX_FRIENDS];
    uint64_t ignore_list[MAX_IGNORES];

    uint8_t hair_colour;
    uint8_t top_colour;
    uint8_t trouser_colour;
    uint8_t skin_colour;
    uint8_t combat_level;
    uint8_t skulled;
    uint8_t worn_items[12];

    uint16_t bank_item_ids[MAX_BANK_SIZE];
    uint32_t bank_item_amounts[MAX_BANK_SIZE];

    invitem_t inventory_items[MAX_INVENTORY_SIZE];

    player_t *trade_target;
    player_t *duel_target;

    linkedlist_t bubbles_to_display;
    linkedlist_t chat_messages_to_display;
    linkedlist_t hit_updates_to_display;
    linkedlist_t projectiles_to_display;
    linkedlist_t appearance_updates_to_display;
    linkedlist_t npc_hit_updates_to_display;
    linkedlist_t npc_chat_messages_to_display;

    linkedlist_t known_appearance_ids;
    eslist_t watched_players;
    eslist_t watched_objects;
    eslist_t watched_items;
    eslist_t watched_npcs;

    uint8_t last_damage;
};

struct appearance_mapping {
    player_t *player;
    uint32_t appearance_id;
};

int player_get_crown(player_t *player);
int player_update_position(player_t *player);
int player_update_appearance_id(player_t *player);

int player_revalidate_watched_players(player_t *player);
int player_update_viewed_players(player_t *player);

int player_destroy(player_t *player);

int player_prayer_enabled(player_t *player, int prayer);
int player_activate_prayer(player_t *player, int prayer);
int player_deactivate_prayer(player_t *player, int prayer);

int player_game_setting_enabled(player_t *player, int setting);
int player_set_game_setting(player_t *player, int setting);
int player_unset_game_setting(player_t *player, int setting);

int player_privacy_setting_enabled(player_t *player, int setting);
int player_set_privacy_setting(player_t *player, int setting);
int player_unset_privacy_setting(player_t *player, int setting);

#endif	/* INCLUDED_PLAYER_H */

