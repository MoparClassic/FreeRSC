#include "npc.h"
#include "common.h"

#include <assert.h>

npcdef_t npc_definitions[MAX_NPC_DEFINITIONS];

int npc_update_position(npc_t *npc)
{
    assert(npc); /* npc_t must not be NULL */
    return 0;
}

int npc_update_appearance_id(npc_t *npc)
{
    assert(npc); /* npc_t must not be NULL */

    if (npc->flags & EF_APPEARANCE_CHANGED) {
        ++npc->appearance_id;
    }
    return 1;
}

int npc_aggressive(npc_t *npc)
{
    assert(npc); /* npc_t must not be NULL */
    return (npc->flags & NF_AGGRESSIVE) && (npc->flags & NF_ATTACKABLE);
}

int npc_set_sprite(npc_t *npc, int sprite)
{
    assert(npc); /* npc_t must not be NULL */
    assert(sprite > 0); /* Sprite must be a valid, positive integer */

    npc->sprite = sprite;
    npc->flags |= EF_SPRITE_CHANGED;
    return 1;
}

int npcdef_get_hitpoints(int npc_id)
{
    assert(npc_id >= 0 && npc_id < MAX_NPC_DEFINITIONS);
    return npc_definitions[npc_id].hitpoints;
}

int npcdef_get_attack(int npc_id)
{
    assert(npc_id >= 0 && npc_id < MAX_NPC_DEFINITIONS);
    return npc_definitions[npc_id].attack;
}

int npcdef_get_defense(int npc_id)
{
    assert(npc_id >= 0 && npc_id < MAX_NPC_DEFINITIONS);
    return npc_definitions[npc_id].defense;
}

int npcdef_get_strength(int npc_id)
{
    assert(npc_id >= 0 && npc_id < MAX_NPC_DEFINITIONS);
    return npc_definitions[npc_id].strength;
}

int npcdef_set_flags(int npc_id, int flags)
{
    assert(npc_id >= 0 && npc_id < MAX_NPC_DEFINITIONS);
    npc_definitions[npc_id].flags |= flags;
    return 1;
}

int npcdef_unset_flags(int npc_id, int flags)
{
    assert(npc_id >= 0 && npc_id < MAX_NPC_DEFINITIONS);
    npc_definitions[npc_id].flags &= ~flags;
    return 1;
}