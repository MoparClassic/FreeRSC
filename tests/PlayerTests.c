#include "PlayerTests.h"

static void
test_player_get_crown(CuTest *tc)
{
    const char *err = "player.c player_get_crown FAIL";

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_get_crown PASS\n");
#endif
}

static void
test_player_set_flags(CuTest *tc)
{
    const char *err = "player.c player_set_flags FAIL";
    player_t player;
    int expected;
    int i;

    memset(&player, 0, sizeof (player_t));

    for (i = 1; expected < 0xFFFFFFFF; i *= 2) {
        expected = (expected << 1) | 1;
        player_set_flags(&player, i);
        CuAssertIntEquals_Msg(tc, err, expected, player.flags);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_set_flags PASS\n");
#endif
}

static void
test_player_unset_flags(CuTest *tc)
{
    const char *err = "player.c player_unset_flags FAIL";
    player_t player;
    unsigned int expected;
    unsigned int i;

    memset(&player, 0, sizeof (player_t));
    player.flags = 0xFFFFFFFF;
    expected = 0xFFFFFFFF;
    for (i = 2147483648u; expected != 0; i -= i / 2) {
        expected >>= 1;
        player_unset_flags(&player, i);
        CuAssertIntEquals_Msg(tc, err, expected, player.flags);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_unset_flags PASS\n");
#endif
}

static void
test_player_reset_moved(CuTest *tc)
{
    const char *err = "player.c player_reset_moved FAIL";
    player_t player;

    memset(&player, 0, sizeof (player_t));
    player_set_flags(&player, EF_MOVED);

    player_reset_moved(&player);
    if (EF_MOVED & player.flags) {
        CuFail(tc, err);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_reset_moved PASS\n");
#endif
}

static void
test_player_update_position(CuTest *tc)
{
    const char *err = "player.c player_update_position FAIL";
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_update_position PASS\n");
#endif
}

static void
test_player_update_appearance_id(CuTest *tc)
{
    const char *err = "player.c player_update_appearance_id FAIL";
    player_t player;

    memset(&player, 0, sizeof (player_t));
    player_update_appearance_id(&player);
    CuAssertIntEquals_Msg(tc, err, 0, player.appearance_id);

    player_set_flags(&player, EF_APPEARANCE_CHANGED);
    player_update_appearance_id(&player);
    CuAssertIntEquals_Msg(tc, err, 1, player.appearance_id);
    player_unset_flags(&player, EF_APPEARANCE_CHANGED);
    player_update_appearance_id(&player);
    CuAssertIntEquals_Msg(tc, err, 1, player.appearance_id);
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_update_appearance_id PASS\n");
#endif
}

static void
test_player_revalidate_watched_players(CuTest *tc)
{
    const char *err = "player.c player_revalidate_watched_players FAIL";
    player_t plist[5];
    player_t player;
    player_t *target;
    eslist_t *watched_players = &player.watched_players;
    int i;

    // Initialise the players we're using for testing
    memset(&player, 0, sizeof (player_t));
    memset(plist, 0, sizeof (plist));
    player.x = 500;
    player.y = 500;
    for (i = 0; i < 5; ++i) {
        plist[i].x = 514 + i;
        plist[i].y = 514 + i;
    }

    /*
     * Test part one:
     * Everything should find its way into the list of players to
     * be removed, simply because none of the players have their
     * 'logged in' flag set.
     */
    for (i = 0; i < 5; ++i) {
        linkedlist_prepend(&watched_players->known_entities, &plist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_players->known_entities));

    player_revalidate_watched_players(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_players->known_entities));
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_players->entities_to_remove));
    for (i = 0; i < 5; ++i) {
        target = &plist[i];
        if (!eslist_isremoving(watched_players, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_players);
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_players->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_players->entities_to_remove));


    /*
     * Test part two:
     * All of the players are logged in this time.
     * Three of the players are outside of the view area,
     * and should be marked for removal.
     */
    for (i = 0; i < 5; ++i) {
        player_set_flags(&plist[i], PF_LOGGED_IN);
        linkedlist_prepend(&watched_players->known_entities, &plist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_players->known_entities));

    player_revalidate_watched_players(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_players->known_entities));
    CuAssertIntEquals_Msg(tc, err, 3,
                          linkedlist_size(&watched_players->entities_to_remove));
    for (i = 2; i < 5; ++i) {
        target = &plist[i];
        if (!eslist_isremoving(watched_players, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_players);
    CuAssertIntEquals_Msg(tc, err, 2,
                          linkedlist_size(&watched_players->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_players->entities_to_remove));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_revalidate_watched_players PASS\n");
#endif
}

static void
test_player_revalidate_watched_objects(CuTest *tc)
{
    const char *err = "player.c player_revalidate_watched_objects FAIL";
    gameobject_t olist[5];
    player_t player;
    gameobject_t *target;
    eslist_t *watched_objects = &player.watched_objects;
    int i;

    // Initialise the players we're using for testing
    memset(&player, 0, sizeof (player_t));
    memset(olist, 0, sizeof (olist));
    player.x = 500;
    player.y = 500;
    for (i = 0; i < 5; ++i) {
        olist[i].x = 514 + i;
        olist[i].y = 514 + i;
    }

    /*
     * Test part one:
     */
    for (i = 0; i < 5; ++i) {
        gameobject_set_flags(&olist[i], EF_REMOVING);
        linkedlist_prepend(&watched_objects->known_entities, &olist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_objects->known_entities));

    player_revalidate_watched_objects(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_objects->known_entities));
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_objects->entities_to_remove));
    for (i = 0; i < 5; ++i) {
        target = &olist[i];
        if (!eslist_isremoving(watched_objects, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_objects);
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_objects->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_objects->entities_to_remove));


    /*
     * Test part two:
     */
    for (i = 0; i < 5; ++i) {
        gameobject_unset_flags(&olist[i], EF_REMOVING);
        linkedlist_prepend(&watched_objects->known_entities, &olist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_objects->known_entities));

    player_revalidate_watched_objects(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_objects->known_entities));
    CuAssertIntEquals_Msg(tc, err, 3,
                          linkedlist_size(&watched_objects->entities_to_remove));
    for (i = 2; i < 5; ++i) {
        target = &olist[i];
        if (!eslist_isremoving(watched_objects, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_objects);
    CuAssertIntEquals_Msg(tc, err, 2,
                          linkedlist_size(&watched_objects->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_objects->entities_to_remove));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_revalidate_watched_objects PASS\n");
#endif

}

static void
test_player_revalidate_watched_items(CuTest *tc)
{
    const char *err = "player.c player_revalidate_watched_items FAIL";
    grounditem_t ilist[5];
    player_t player;
    grounditem_t *target;
    eslist_t *watched_items = &player.watched_items;
    int i;

    // Initialise the players we're using for testing
    memset(&player, 0, sizeof (player_t));
    memset(ilist, 0, sizeof (ilist));
    player.x = 500;
    player.y = 500;
    for (i = 0; i < 5; ++i) {
        ilist[i].x = 514 + i;
        ilist[i].y = 514 + i;
    }

    /*
     * Test part one:
     */
    for (i = 0; i < 5; ++i) {
        grounditem_set_flags(&ilist[i], EF_REMOVING);
        linkedlist_prepend(&watched_items->known_entities, &ilist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_items->known_entities));

    player_revalidate_watched_items(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_items->known_entities));
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_items->entities_to_remove));
    for (i = 0; i < 5; ++i) {
        target = &ilist[i];
        if (!eslist_isremoving(watched_items, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_items);
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_items->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_items->entities_to_remove));


    /*
     * Test part two:
     */
    for (i = 0; i < 5; ++i) {
        grounditem_unset_flags(&ilist[i], EF_REMOVING);
        linkedlist_prepend(&watched_items->known_entities, &ilist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_items->known_entities));

    player_revalidate_watched_items(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_items->known_entities));
    CuAssertIntEquals_Msg(tc, err, 3,
                          linkedlist_size(&watched_items->entities_to_remove));
    for (i = 2; i < 5; ++i) {
        target = &ilist[i];
        if (!eslist_isremoving(watched_items, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_items);
    CuAssertIntEquals_Msg(tc, err, 2,
                          linkedlist_size(&watched_items->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_items->entities_to_remove));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_revalidate_watched_items PASS\n");
#endif
}

static void
test_player_revalidate_watched_npcs(CuTest *tc)
{
    const char *err = "player.c player_revalidate_watched_items FAIL";
    npc_t nlist[5];
    player_t player;
    npc_t *target;
    eslist_t *watched_npcs = &player.watched_npcs;
    int i;

    // Initialise the players we're using for testing
    memset(&player, 0, sizeof (player_t));
    memset(nlist, 0, sizeof (nlist));
    player.x = 500;
    player.y = 500;
    for (i = 0; i < 5; ++i) {
        nlist[i].x = 514 + i;
        nlist[i].y = 514 + i;
    }

    /*
     * Test part one:
     */
    for (i = 0; i < 5; ++i) {
        npc_set_flags(&nlist[i], EF_REMOVING);
        linkedlist_prepend(&watched_npcs->known_entities, &nlist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_npcs->known_entities));

    player_revalidate_watched_npcs(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_npcs->known_entities));
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_npcs->entities_to_remove));
    for (i = 0; i < 5; ++i) {
        target = &nlist[i];
        if (!eslist_isremoving(watched_npcs, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_npcs);
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_npcs->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_npcs->entities_to_remove));


    /*
     * Test part two:
     */
    for (i = 0; i < 5; ++i) {
        npc_unset_flags(&nlist[i], EF_REMOVING);
        linkedlist_prepend(&watched_npcs->known_entities, &nlist[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_npcs->known_entities));

    player_revalidate_watched_npcs(&player);
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&watched_npcs->known_entities));
    CuAssertIntEquals_Msg(tc, err, 3,
                          linkedlist_size(&watched_npcs->entities_to_remove));
    for (i = 2; i < 5; ++i) {
        target = &nlist[i];
        if (!eslist_isremoving(watched_npcs, target)) {
            CuFail(tc, err);
        }
    }

    eslist_update(watched_npcs);
    CuAssertIntEquals_Msg(tc, err, 2,
                          linkedlist_size(&watched_npcs->known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&watched_npcs->entities_to_remove));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_revalidate_watched_npcs PASS\n");
#endif
}

static void
test_player_get_players_in_view(CuTest *tc)
{
    const char *err = "player.c player_update_viewed_players FAIL";
    player_t player;
    player_t plist[4];
    linkedlist_t llplayers;

    memset(&player, 0, sizeof(player_t));
    memset(&llplayers, 0, sizeof (linkedlist_t));
    memset(plist, 0, sizeof (plist));
    player.x = 500;
    player.y = 500;

    world_set_player_location(&plist[0], 0, 0, 485, 485);
    world_set_player_location(&plist[1], 0, 0, 485, 515);
    world_set_player_location(&plist[2], 0, 0, 515, 515);
    world_set_player_location(&plist[3], 0, 0, 515, 485);

    player_get_players_in_view(&player, &llplayers);
    CuAssertIntEquals_Msg(tc, err, 4, linkedlist_size(&llplayers));
    linkedlist_clear(&llplayers);

    world_set_player_location(&plist[0], 485, 485, 484, 484);
    world_set_player_location(&plist[1], 485, 515, 484, 516);
    world_set_player_location(&plist[2], 515, 515, 516, 516);
    world_set_player_location(&plist[3], 515, 485, 516, 484);
    player_get_players_in_view(&player, &llplayers);
    CuAssertIntEquals_Msg(tc, err, 0, linkedlist_size(&llplayers));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_get_players_in_view PASS\n");
#endif
}

static void
test_player_get_objects_in_view(CuTest *tc)
{
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_get_objects_in_view PASS\n");
#endif
}

static void
test_player_get_items_in_view(CuTest *tc)
{
    const char *err = "player.c player_update_viewed_items FAIL";
    player_t player;
    grounditem_t ilist[4];
    linkedlist_t llitems;

    memset(&player, 0, sizeof(player_t));
    memset(&llitems, 0, sizeof (linkedlist_t));
    memset(ilist, 0, sizeof (ilist));
    player.x = 500;
    player.y = 500;

    world_set_grounditem_location(&ilist[0], 0, 0, 485, 485);
    world_set_grounditem_location(&ilist[1], 0, 0, 485, 515);
    world_set_grounditem_location(&ilist[2], 0, 0, 515, 515);
    world_set_grounditem_location(&ilist[3], 0, 0, 515, 485);

    player_get_items_in_view(&player, &llitems);
    CuAssertIntEquals_Msg(tc, err, 4, linkedlist_size(&llitems));
    linkedlist_clear(&llitems);

    world_set_grounditem_location(&ilist[0], 485, 485, 478, 478);
    world_set_grounditem_location(&ilist[1], 485, 515, 478, 521);
    world_set_grounditem_location(&ilist[2], 515, 515, 521, 521);
    world_set_grounditem_location(&ilist[3], 515, 485, 521, 478);
    player_get_items_in_view(&player, &llitems);
    CuAssertIntEquals_Msg(tc, err, 0, linkedlist_size(&llitems));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_get_items_in_view PASS\n");
#endif
}

static void
test_player_get_npcs_in_view(CuTest *tc)
{
    const char *err = "player.c player_update_viewed_items FAIL";
    player_t player;
    npc_t nlist[4];
    linkedlist_t llnpcs;

    memset(&player, 0, sizeof(player_t));
    memset(&llnpcs, 0, sizeof (linkedlist_t));
    memset(nlist, 0, sizeof (nlist));
    player.x = 500;
    player.y = 500;

    world_set_npc_location(&nlist[0], 0, 0, 485, 485);
    world_set_npc_location(&nlist[1], 0, 0, 485, 515);
    world_set_npc_location(&nlist[2], 0, 0, 515, 515);
    world_set_npc_location(&nlist[3], 0, 0, 515, 485);

    player_get_npcs_in_view(&player, &llnpcs);
    CuAssertIntEquals_Msg(tc, err, 4, linkedlist_size(&llnpcs));
    linkedlist_clear(&llnpcs);

    world_set_npc_location(&nlist[0], 485, 485, 484, 484);
    world_set_npc_location(&nlist[1], 485, 515, 484, 516);
    world_set_npc_location(&nlist[2], 515, 515, 516, 516);
    world_set_npc_location(&nlist[3], 515, 485, 516, 484);
    player_get_npcs_in_view(&player, &llnpcs);
    CuAssertIntEquals_Msg(tc, err, 0, linkedlist_size(&llnpcs));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_get_npcs_in_view PASS\n");
#endif
}

static void
test_player_update_viewed_players(CuTest *tc)
{
    const char *err = "player.c player_update_viewed_players FAIL";
    player_t player, *target;
    player_t plist[4];
    lnode_t *node;
    int i;

    memset(&player, 0, sizeof(player_t));
    memset(plist, 0, sizeof(plist));
    player.x = 500;
    player.y = 500;
    for (i = 0; i < 4; ++i) {
        player_set_flags(&plist[i], PF_LOGGED_IN);
    }

    world_set_player_location(&plist[0], 0, 0, 485, 485);
    world_set_player_location(&plist[1], 0, 0, 485, 515);
    world_set_player_location(&plist[2], 0, 0, 515, 515);
    world_set_player_location(&plist[3], 0, 0, 515, 485);
    player_update_viewed_players(&player);
    CuAssertIntEquals_Msg(tc, err, 4,
                          linkedlist_size(&player.watched_players.new_entities));

    LINKEDLIST_FOREACH(node, &player.watched_players.new_entities) {
        target = (player_t *) node->data;
        for (i = 0; i < 4; ++i) {
            if (&plist[i] == target) {
                i = 10;
                break;
            }
        }

        if (i != 10) {
            CuFail(tc, err);
        }
    }

    linkedlist_clear(&player.watched_players.new_entities);
    world_set_player_location(&plist[0], 485, 485, 484, 484);
    world_set_player_location(&plist[1], 485, 515, 484, 516);
    world_set_player_location(&plist[2], 515, 515, 516, 516);
    world_set_player_location(&plist[3], 515, 485, 516, 484);
    player_update_viewed_players(&player);
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&player.watched_players.new_entities));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_update_viewed_players PASS\n");
#endif
}

static void
test_player_update_viewed_objects(CuTest *tc)
{
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_update_viewed_objects PASS\n");
#endif
}

static void
test_player_update_viewed_items(CuTest *tc)
{
    const char *err = "player.c player_update_viewed_items FAIL";
    player_t player;
    grounditem_t ilist[4], *target;
    lnode_t *node;
    int i;

    memset(&player, 0, sizeof(player_t));
    memset(ilist, 0, sizeof(ilist));
    player.x = 500;
    player.y = 500;

    world_set_grounditem_location(&ilist[0], 0, 0, 485, 485);
    world_set_grounditem_location(&ilist[1], 0, 0, 485, 515);
    world_set_grounditem_location(&ilist[2], 0, 0, 515, 515);
    world_set_grounditem_location(&ilist[3], 0, 0, 515, 485);
    player_update_viewed_items(&player);
    CuAssertIntEquals_Msg(tc, err, 4,
                          linkedlist_size(&player.watched_items.new_entities));

    LINKEDLIST_FOREACH(node, &player.watched_items.new_entities) {
        target = (grounditem_t *) node->data;
        for (i = 0; i < 4; ++i) {
            if (&ilist[i] == target) {
                i = 10;
                break;
            }
        }

        if (i != 10) {
            CuFail(tc, err);
        }
    }

    linkedlist_clear(&player.watched_items.new_entities);
    world_set_grounditem_location(&ilist[0], 485, 485, 483, 483);
    world_set_grounditem_location(&ilist[1], 485, 515, 484, 516);
    world_set_grounditem_location(&ilist[2], 515, 515, 516, 516);
    world_set_grounditem_location(&ilist[3], 515, 485, 516, 484);
    player_update_viewed_items(&player);
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&player.watched_items.new_entities));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_update_viewed_items PASS\n");
#endif
}

static void
test_player_update_viewed_npcs(CuTest *tc)
{
    const char *err = "player.c player_update_viewed_npcs FAIL";
    player_t player;
    npc_t nlist[4], *target;
    lnode_t *node;
    int i;

    memset(&player, 0, sizeof(player_t));
    memset(nlist, 0, sizeof(nlist));
    player.x = 500;
    player.y = 500;

    world_set_npc_location(&nlist[0], 0, 0, 485, 485);
    world_set_npc_location(&nlist[1], 0, 0, 485, 515);
    world_set_npc_location(&nlist[2], 0, 0, 515, 515);
    world_set_npc_location(&nlist[3], 0, 0, 515, 485);
    player_update_viewed_npcs(&player);
    CuAssertIntEquals_Msg(tc, err, 4,
                          linkedlist_size(&player.watched_npcs.new_entities));

    LINKEDLIST_FOREACH(node, &player.watched_npcs.new_entities) {
        target = (npc_t *) node->data;
        for (i = 0; i < 4; ++i) {
            if (&nlist[i] == target) {
                i = 10;
                break;
            }
        }

        if (i != 10) {
            CuFail(tc, err);
        }
    }

    memset(&player, 0, sizeof(player_t));
    world_set_npc_location(&nlist[0], 485, 485, 484, 484);
    world_set_npc_location(&nlist[1], 485, 515, 484, 516);
    world_set_npc_location(&nlist[2], 515, 515, 516, 516);
    world_set_npc_location(&nlist[3], 515, 485, 516, 484);
    player_update_viewed_players(&player);
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&player.watched_npcs.new_entities));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_update_viewed_npcs PASS\n");
#endif
}

static void
test_player_destroy(CuTest *tc)
{
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("player.c player_destroy PASS\n");
#endif
}

static void
test_remove_known_appearance(CuTest *tc)
{

}

static void
test_within_range(CuTest *tc)
{

}

static void
test_can_see_item(CuTest *tc)
{

}

static void
test_inform_of_player(CuTest *tc)
{

}

static void
test_player_get_viewed_area(CuTest *tc)
{

}

CuSuite *
get_player_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, &test_player_get_crown);
    SUITE_ADD_TEST(suite, &test_player_set_flags);
    SUITE_ADD_TEST(suite, &test_player_unset_flags);
    SUITE_ADD_TEST(suite, &test_player_reset_moved);
    SUITE_ADD_TEST(suite, &test_player_update_position);
    SUITE_ADD_TEST(suite, &test_player_update_appearance_id);

    SUITE_ADD_TEST(suite, &test_player_revalidate_watched_players);
    SUITE_ADD_TEST(suite, &test_player_revalidate_watched_objects);
    SUITE_ADD_TEST(suite, &test_player_revalidate_watched_items);
    SUITE_ADD_TEST(suite, &test_player_revalidate_watched_npcs);

    SUITE_ADD_TEST(suite, &test_player_get_players_in_view);
    SUITE_ADD_TEST(suite, &test_player_get_objects_in_view);
    SUITE_ADD_TEST(suite, &test_player_get_items_in_view);
    SUITE_ADD_TEST(suite, &test_player_get_npcs_in_view);

    SUITE_ADD_TEST(suite, &test_player_update_viewed_players);
    SUITE_ADD_TEST(suite, &test_player_update_viewed_objects);
    SUITE_ADD_TEST(suite, &test_player_update_viewed_items);
    SUITE_ADD_TEST(suite, &test_player_update_viewed_npcs);

    /*
        SUITE_ADD_TEST(suite, &test_player_destroy);

        // The following are static functions in player.c
        SUITE_ADD_TEST(suite, &test_remove_known_appearance);
        SUITE_ADD_TEST(suite, &test_within_range);
        SUITE_ADD_TEST(suite, &test_can_see_item);
        SUITE_ADD_TEST(suite, &test_inform_of_player);
        SUITE_ADD_TEST(suite, &test_player_get_viewed_area);
    */
    return suite;
}