#include "WorldTests.h"

static void
test_world_player_foreach(CuTest *tc)
{
    const char *err = "world.c PLAYER_FOREACH macro FAIL";
    const char *username = "Testname";
    int max = MAX_PLAYERS;
    int mid = max / 2;
    player_t *player;
    player_t *plist[max];
    int i;

    if (max % 2 == 1) {
        CuFail(tc, "world.c PLAYER_FOREACH macro FAIL -- "
               "Rerun tests with even number of max players");
    }

    for (i = 0; i < max; ++i) {
        player = get_unused_player_slot();
        CuAssertIntEquals_Msg(tc, err, i + 1, player->index);
        player->username = username;
        if (i == mid - 1) {
            remove_player(player);
            continue;
        }

        plist[i] = player;
    }

    i = 0;
    player = NULL;
    PLAYER_LIST_FOREACH(player) {
        if (player->index == UNUSED) {
            ++i;
            continue;
        }

        CuAssertPtrEquals_Msg(tc, err, plist[i++], player);
    }

    player = get_unused_player_slot();
    CuAssertIntEquals_Msg(tc, err, mid, player->index);

    player = get_unused_player_slot();
    CuAssertPtrEquals_Msg(tc, err, NULL, player);

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("world.c PLAYER_FOREACH macro PASS\n");
#endif
}

static void
test_world_npc_foreach(CuTest *tc)
{
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("world.c NPC_FOREACH macro PASS\n");
#endif
}

CuSuite *
get_world_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, &test_world_player_foreach);
    SUITE_ADD_TEST(suite, &test_world_npc_foreach);
    return suite;
}