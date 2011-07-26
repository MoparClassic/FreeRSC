#include "npc.h"

npcdef_t npc_definitions[MAX_NPC_DEFINITIONS];

int npc_set_flags(npc_t *npc, int flags)
{
    npc->flags |= flags;
    return 1;
}

int npc_unset_flags(npc_t *npc, int flags)
{
    npc->flags &= ~flags;
    return 1;
}

int npc_reset_moved(npc_t *npc)
{
    return npc_unset_flags(npc, EF_MOVED);
}

int npc_update_position(npc_t *npc)
{
    return 0;
}

int npc_update_appearance_id(npc_t *npc)
{
    if (npc->flags & EF_APPEARANCE_CHANGED) {
        ++npc->appearance_id;
    }
    return 1;
}

int npc_aggressive(npc_t *npc)
{
    return (npc->flags & NF_AGGRESSIVE) && (npc->flags & NF_ATTACKABLE);
}

int npc_set_sprite(npc_t *npc, int sprite)
{
    npc->sprite = sprite;
    npc_set_flags(npc, EF_SPRITE_CHANGED);
    return 1;
}

int npcdef_get_hitpoints(int npc_id)
{
    npcdef_t *ndef = &npc_definitions[npc_id];
    return ndef->hitpoints;
}

int npcdef_get_attack(int npc_id)
{
    npcdef_t *ndef = &npc_definitions[npc_id];
    return ndef->attack;
}

int npcdef_get_defense(int npc_id)
{
    npcdef_t *ndef = &npc_definitions[npc_id];
    return ndef->defense;
}

int npcdef_get_strength(int npc_id)
{
    npcdef_t *ndef = &npc_definitions[npc_id];
    return ndef->strength;
}

int npcdef_set_flags(int npc_id, int flags)
{
    npcdef_t *ndef = &npc_definitions[npc_id];
    ndef->flags |= flags;
    return 1;
}

int npcdef_unset_flags(int npc_id, int flags)
{
    npcdef_t *ndef = &npc_definitions[npc_id];
    ndef->flags &= ~flags;
    return 1;
}