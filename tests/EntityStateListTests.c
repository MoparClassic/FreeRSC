#include "EntityStateListTests.h"

static void
test_eslist_add(CuTest *tc)
{
    const char *err = "entitystatelist.c eslist_add FAIL";
    eslist_t eslist;
    lnode_t *node;
    int vals[] = {1, 2, 3, 4, 5};
    int i;

    memset(&eslist, 0, sizeof(eslist_t));

    // Resulting list will be: 5, 4, 3, 2, 1
    for (i = 0; i < 5; ++i) {
        //printf("Adding value to entitystatelist: %d\n", vals[i]);
        eslist_add(&eslist, (void *) vals[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5, linkedlist_size(&eslist.new_entities));

    i = 4;

    LINKEDLIST_FOREACH(node, &eslist.new_entities) {
        //printf("Result: %d\n", res);
        CuAssertIntEquals_Msg(tc, err, vals[i--], (int) node->data);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("entitystatelist.c eslist_add PASS\n");
#endif
}

static void
test_eslist_add_list(CuTest *tc)
{
    const char *err = "entitystatelist.c eslist_add_list FAIL";
    eslist_t eslist;
    linkedlist_t list;
    lnode_t *node;
    int vals[] = {1, 2, 3, 4, 5};
    int i;

    memset(&eslist, 0, sizeof(eslist_t));
    memset(&list, 0, sizeof(linkedlist_t));

    // Resulting list will be: 5, 4, 3, 2, 1
    for (i = 0; i < 5; ++i) {
        linkedlist_prepend(&list, (void *) vals[i]);
    }

    // Resulting internal list will be: 1, 2, 3, 4, 5
    eslist_add_list(&eslist, &list);
    i = 0;
    LINKEDLIST_FOREACH(node, &eslist.new_entities) {
        CuAssertIntEquals_Msg(tc, err, vals[i++], (int) node->data);
    }
    CuAssertIntEquals_Msg(tc, err, 5, linkedlist_size(&eslist.new_entities));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("entitystatelist.c eslist_add_list PASS\n");
#endif
}

static void
test_eslist_contains(CuTest *tc)
{
    const char *err = "entitystatelist.c eslist_contains FAIL";
    eslist_t eslist;
    int vals[] = {1, 2, 3, 4, 5};
    int vals2[] = {6, 7, 8, 9, 10};
    int i;

    memset(&eslist, 0, sizeof(eslist_t));

    // At this stage, the list shouldn't contain anything
    for (i = 0; i < 5; ++i) {
        if (eslist_contains(&eslist, (void *) vals[i]) == 1) {
            CuFail(tc, err);
        }

        if (eslist_contains(&eslist, (void *) vals2[i]) == 1) {
            CuFail(tc, err);
        }
    }

    for (i = 0; i < 5; ++i) {
        linkedlist_prepend(&eslist.new_entities, (void *) vals[i]);
    }

    /*
     * This time, the entitystatelist should contain all of
     * the values that are in the vals array, and still none
     * of the values that are in the vals2 array.
     */
    for (i = 0; i < 5; ++i) {
        // If vals[i] is NOT in the list, fail
        if (eslist_contains(&eslist, (void *) vals[i]) == 0) {
            CuFail(tc, err);
        }

        // If vals2[i] is in the list, fail -- it shouldn't be
        if (eslist_contains(&eslist, (void *) vals2[i]) == 1) {
            CuFail(tc, err);
        }
    }
    CuAssertIntEquals_Msg(tc, err, 5, linkedlist_size(&eslist.new_entities));
    CuAssertIntEquals_Msg(tc, err, 0, linkedlist_size(&eslist.known_entities));

    // Add the vals2 array values to the list of already known entities
    for (i = 0; i < 5; ++i) {
        linkedlist_prepend(&eslist.known_entities, (void *) vals2[i]);
    }

    /*
     * This time, all values from both arrays should be present
     * within the entitystatelist.
     * We have added one of the array's values to the internal
     * list of 'new' entities, and the other array's values
     * to the internal list of 'already known' entities.
     * If an element is present in either list then eslist_contains
     * should return 1.
     */
    for (i = 0; i < 5; ++i) {
        if (eslist_contains(&eslist, (void *) vals[i]) == 0) {
            CuFail(tc, err);
        }

        if (eslist_contains(&eslist, (void *) vals2[i]) == 0) {
            CuFail(tc, err);
        }
    }
    CuAssertIntEquals_Msg(tc, err, 5, linkedlist_size(&eslist.new_entities));
    CuAssertIntEquals_Msg(tc, err, 5, linkedlist_size(&eslist.known_entities));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("entitystatelist.c eslist_contains PASS\n");
#endif
}

static void
test_eslist_remove(CuTest *tc)
{
    const char *err = "entitystatelist.c eslist_remove FAIL";
    eslist_t eslist;
    lnode_t *node;
    int vals[] = {1, 2, 3, 4, 5};
    int i;

    memset(&eslist, 0, sizeof(eslist_t));

    for (i = 0; i < 5; ++i) {
        eslist_remove(&eslist, (void *) vals[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&eslist.entities_to_remove));

    /*
     * Check that the values in the 'entities to remove' list
     * are actually the values that we with to remove.
     */
    i = 4;
    LINKEDLIST_FOREACH(node, &eslist.entities_to_remove) {
        CuAssertIntEquals_Msg(tc, err, vals[i--], (int) node->data);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("entitystatelist.c eslist_remove PASS\n");
#endif
}

static void
test_eslist_isremoving(CuTest *tc)
{
    const char *err = "entitystatelist.c eslist_isremoving FAIL";
    eslist_t eslist;
    lnode_t *node;
    int vals[] = {1, 2, 3, 4, 5};
    int i;

    memset(&eslist, 0, sizeof(eslist_t));

    for (i = 0; i < 5; ++i) {
        eslist_remove(&eslist, (void *) vals[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&eslist.entities_to_remove));

    /*
     * Check that each element of the vals array that has
     * been flagged for removal is within the list of
     * entities to be removed.
     * In other words, eslist_removing should return 1 for
     * each of the values in the vals array.
     */
    for (i = 0; i < 5; ++i) {
        if (eslist_isremoving(&eslist, (void *) vals[i]) == 0) {
            CuFail(tc, err);
        }
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("entitystatelist.c eslist_isremoving PASS\n");
#endif
}

static void
test_eslist_update(CuTest *tc)
{
    const char *err = "entitystatelist.c eslist_update FAIL";
    eslist_t eslist;
    lnode_t *node;
    int vals[] = {1, 2, 3, 4, 5};
    int vals2[] = {6, 7, 8, 9, 10};
    int i;

    memset(&eslist, 0, sizeof(eslist_t));

    // Add elements from both value arrays to the entitystatelist
    for (i = 0; i < 5; ++i) {
        eslist_add(&eslist, (void *) vals[i]);
        eslist_add(&eslist, (void *) vals2[i]);
    }

    /*
     * At this stage, we have added 10 elements to the entitystatelist.
     * This means that, prior to invoking the update function, the
     * following should be true:
     * 1) The new_entities linkedlist should now have 10 elements
     * 2) The known_entities linkedlist should have zero elements
     * 3) The entities_to_remove should have zero elements
     */
    CuAssertIntEquals_Msg(tc, err, 10, linkedlist_size(&eslist.new_entities));
    CuAssertIntEquals_Msg(tc, err, 0, linkedlist_size(&eslist.known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&eslist.entities_to_remove));

    eslist_update(&eslist);

    /*
     * The entitystatelist has now been updated.
     * This means that:
     * 1) All elements in the new_entities linkedlist should have been
     * added to the known_entities linkedlist.
     * 2) All elements in the entities_to_remove linkedlist should have
     * been removed from the known_entities linkedlist.
     * 3) The new_entities and entities_to_remove linkedlists should
     * have been cleared.
     *
     * Therefore, the sizes of these lists should be:
     * new_entities: zero
     * known_entities: 10
     * entities_to_remove: zero
     */
    CuAssertIntEquals_Msg(tc, err, 0, linkedlist_size(&eslist.new_entities));
    CuAssertIntEquals_Msg(tc, err, 10, linkedlist_size(&eslist.known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&eslist.entities_to_remove));

    for (i = 0; i < 5; ++i) {
        eslist_remove(&eslist, (void *) vals2[i]);
    }
    CuAssertIntEquals_Msg(tc, err, 5,
                          linkedlist_size(&eslist.entities_to_remove));

    eslist_update(&eslist);
    CuAssertIntEquals_Msg(tc, err, 0, linkedlist_size(&eslist.new_entities));
    CuAssertIntEquals_Msg(tc, err, 5, linkedlist_size(&eslist.known_entities));
    CuAssertIntEquals_Msg(tc, err, 0,
                          linkedlist_size(&eslist.entities_to_remove));
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("entitystatelist.c eslist_update PASS\n");
#endif
}

static void
test_eslist_changed(CuTest *tc)
{
    const char *err = "entitystatelist.c eslist_changed FAIL";
    eslist_t eslist;
    int vals[] = {1, 2, 3, 4, 5};
    int i;

    memset(&eslist, 0, sizeof(eslist_t));

    // List should not have changed at this stage
    if (eslist_changed(&eslist) != 0) {
        CuFail(tc, err);
    }
    eslist_add(&eslist, (void *) 1337);
    if (eslist_changed(&eslist) == 0) {
        CuFail(tc, err);
    }

    memset(&eslist, 0, sizeof(eslist_t));
    eslist_remove(&eslist, (void *) 1337);
    if (eslist_changed(&eslist) == 0) {
        CuFail(tc, err);
    }

    memset(&eslist, 0, sizeof(eslist_t));
    eslist_add(&eslist, (void *) 9001);
    eslist_remove(&eslist, (void *) 133337);
    if (eslist_changed(&eslist) == 0) {
        CuFail(tc, err);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("entitystatelist.c eslist_changed PASS\n");
#endif
}

CuSuite *
get_entitystatelist_test_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, &test_eslist_add);
    SUITE_ADD_TEST(suite, &test_eslist_add_list);
    SUITE_ADD_TEST(suite, &test_eslist_contains);
    SUITE_ADD_TEST(suite, &test_eslist_remove);
    SUITE_ADD_TEST(suite, &test_eslist_isremoving);
    SUITE_ADD_TEST(suite, &test_eslist_update);
    SUITE_ADD_TEST(suite, &test_eslist_changed);
    return suite;
}
