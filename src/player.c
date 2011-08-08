#include "player.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int remove_known_appearance(player_t *player, player_t *target);
static int inform_of_player(player_t *player, player_t *target);

int
player_get_crown(player_t *player)
{
    assert(player); /* player must not be a NULL pointer */

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
    return 0; /* No crown */
}

int player_update_position(player_t *player)
{
    assert(player); /* player must not be a NULL pointer */
    return 0;
}

int player_update_appearance_id(player_t *player)
{
    assert(player); /* player must not be a NULL pointer */

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

    assert(player); /* player must not be a NULL pointer */

    for (; NULL != node; node = node->next) {
        target = (player_t *) node->data;

        if (0 == (PF_LOGGED_IN & target->flags)) {
            /* Player is no longer logged in -- remove them */
            eslist_remove(watched_players, target);
            remove_known_appearance(player, target);
        }
        else if (within_range(player->x, player->y, target->x, target->y) == 0) {
            /* If the player is no longer within range, remove them */
            eslist_remove(watched_players, target);
            remove_known_appearance(player, target);
        }
    }
    return 1;
}

int player_update_viewed_players(player_t *player)
{
    linkedlist_t visible_players;
    player_t *target;
    lnode_t *node;

    assert(player); /* player must not be a NULL pointer */

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

int player_destroy(player_t *player)
{
    assert(player); /* player must not be a NULL pointer */
    return 0;
}

static int
remove_known_appearance(player_t *player, player_t *target)
{
    linkedlist_t *known_app_ids = &player->known_appearance_ids;
    appearance_mapping_t *amapping;
    lnode_t *cur = known_app_ids->head;
    lnode_t *prev = known_app_ids->head;

    assert(player); /* player must not be a NULL pointer */
    assert(target); /* target must not be a NULL pointer */

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
inform_of_player(player_t *player, player_t *target)
{
    eslist_t *watched_players = &player->watched_players;
    lnode_t *node = watched_players->known_entities.head;

    assert(player); /* player must not be a NULL pointer */
    assert(target); /* target must not be a NULL pointer */

    if ((!eslist_contains(watched_players, target) ||
         eslist_isremoving(watched_players, target)) &&
        within_range(player->x, player->y, target->x, target->y)) {
        eslist_add(watched_players, target);
    }
    return 0;
}
