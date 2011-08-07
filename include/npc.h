/*
 * File:   npc.h
 * Author: lothy
 *
 * Created on 12 July 2011, 8:24 PM
 */

#ifndef NPC_H
#define	NPC_H

#include <stdint.h>

#define MAX_NPC_DEFINITIONS 1500

#define NF_ATTACKABLE       (1 << 31)
#define NF_AGGRESSIVE       (1 << 30)

typedef struct npcdef npcdef_t;
typedef struct npc npc_t;

struct npcdef {
    uint16_t npc_id;
    uint8_t hitpoints;
    uint8_t attack;
    uint8_t defense;
    uint8_t strength;
    uint32_t flags;
    uint32_t respawn_time;
};

struct npc {
    uint16_t index;
    uint16_t npc_id;
    uint32_t flags;
    uint32_t appearance_id;
    uint16_t x;
    uint16_t y;

    uint8_t last_damage;
    uint8_t cur_hitpoints;
    uint8_t max_hitpoints;
    uint8_t sprite;
};

extern npcdef_t npc_definitions[MAX_NPC_DEFINITIONS];

int npc_update_position(npc_t *npc);
int npc_update_appearance_id(npc_t *npc);
int npc_aggressive(npc_t *npc);
int npc_set_sprite(npc_t *npc, int sprite);

int npcdef_get_hitpoints(int npc_id);
int npcdef_get_attack(int npc_id);
int npcdef_get_defense(int npc_id);
int npcdef_get_strength(int npc_id);
int npcdef_set_flags(int npc_id, int flags);
int npcdef_unset_flags(int npc_id, int flags);

#endif	/* NPC_H */

