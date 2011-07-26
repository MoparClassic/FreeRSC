/*
 * File:   LinkedListTests.h
 */

#include "LinkedListTests.h"

#ifdef STATIC_ALLOC_LINKEDLIST_NODES
static void
test_linkedlist_lnode_alloc(CuTest *tc)
{
    const char *err = "linkedlist.c lnode_t_alloc FAIL";
    lnode_t *n1, *n2;
    int max = STATIC_ALLOC_LINKEDLIST_NODES; /* Use about 50 for testing */
    int middle = max / 2;
    int i;

    if (STATIC_ALLOC_LINKEDLIST_NODES % 2 == 1) {
        CuFail(tc, "linkedlist.c lnode_t_alloc FAIL -- "
               "rerun with even number of nodes allocated");
    }

    for (i = 0; i < max; ++i) {
        n1 = (lnode_t *) lnode_t_alloc();
        if (i == middle - 1) {
            //printf("Freeing node %d\n", i);
            lnode_t_free(n1);
            n1 = NULL;
        }
    }

    n1 = (lnode_t *) lnode_t_alloc();
    lnode_t_free(n1);
    n2 = (lnode_t *) lnode_t_alloc();
    CuAssertPtrEquals_Msg(tc, err, n1, n2);

    n1 = (lnode_t *) lnode_t_alloc();
    CuAssertPtrEquals_Msg(tc, err, NULL, n1);

    linkedlist_init();

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c lnode_t_alloc PASS\n");
#endif
}
#endif

static void
test_linkedlist_prepend(CuTest *tc)
{
    const char *err = "linkedlist.c linkedlist_prepend FAIL";
    char *values[4] = {"A", "B", "C", "D"};
    linkedlist_t list;
    lnode_t *node;
    int score = 0;
    int i;

#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("linkedlist.c linkedlist_prepend:\n");
#endif
    memset(&list, 0, sizeof (linkedlist_t));
    for (i = 0; i < 4; ++i) {
        linkedlist_prepend(&list, values[i]);
    }

    for (node = list.head, i = 3; node != NULL; node = node->next, --i) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("\tExpected: %s \tActual: %s\n",
               values[i], (char *) node->data);
#endif
        CuAssertStrEquals_Msg(tc, err, values[i], (char *) node->data);
        ++score;
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c linkedlist_prepend PASS\n");
#endif

    if (score < 4) {
        CuFail(tc, err);
    }
}

static void
test_linkedlist_append(CuTest *tc)
{
    const char *err = "linkedlist.c linkedlist_append FAIL";
    char *values[4] = {"A", "B", "C", "D"};
    linkedlist_t list;
    lnode_t *node;
    int score = 0;
    int i;

#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("linkedlist.c linkedlist_append:\n");
#endif
    memset(&list, 0, sizeof (linkedlist_t));
    for (i = 0; i < 4; ++i) {
        linkedlist_append(&list, values[i]);
    }

    for (node = list.head, i = 0; node != NULL; node = node->next, ++i) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("\tExpected: %s \tActual: %s\n",
               values[i], (char *) node->data);
#endif
        CuAssertStrEquals_Msg(tc, err, values[i], (char *) node->data);
        ++score;
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c linkedlist_append PASS\n");
#endif

    if (score < 4) {
        CuFail(tc, err);
    }
}

static void
test_linkedlist_foreach(CuTest *tc)
{
    const char *err = "linkedlist.c LINKEDLIST_FORACH macro FAIL";
    char *values[] = {"A", "B", "C", "D", "E", "F"};
    linkedlist_t list;
    lnode_t *node;
    int score = 0;
    int i = 0;

#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
    printf("linkedlist.c foreach macro\n");
#endif
    memset(&list, 0, sizeof (linkedlist_t));
    for (i = 0; i < 6; ++i) {
        linkedlist_append(&list, values[i]);
    }

    i = 0;

    LINKEDLIST_FOREACH(node, &list) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
        printf("\tExpected: %s \tActual: %s\n",
               values[i], (char *) node->data);
#endif
        CuAssertStrEquals_Msg(tc, err, values[i++], (char *) node->data);
        ++score;
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c LINKEDLIST_FOREACH macro PASS\n");
#endif

    if (score < 6) {
        CuFail(tc, err);
    }
}

static void
test_linkedlist_clear(CuTest *tc)
{
    const char *err = "linkedlist.c linkedlist_clear FAIL";
    char *values[] = {"A", "B", "C", "D", "E", "F"};
    linkedlist_t list;
    int cleared = 0;
    int i = 0;

    memset(&list, 0, sizeof (linkedlist_t));
    for (i = 0; i < 6; ++i) {
        linkedlist_append(&list, values[i]);
    }

    cleared = linkedlist_clear(&list);
    if (cleared != 6) {
        CuFail(tc, err);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c linkedlist_clear PASS\n");
#endif
}

static void
test_linkedlist_empty(CuTest *tc)
{
    const char *err = "linkedlist.c linkedlist_empty FAIL";
    linkedlist_t list;

    memset(&list, 0, sizeof (linkedlist_t));
    linkedlist_prepend(&list, "Hi");

    // If the list is empty, fail
    if (linkedlist_empty(&list) == 1) {
        CuFail(tc, err);
    }

    linkedlist_clear(&list);

    // If the list is declared not empty, fail
    if (linkedlist_empty(&list) == 0) {
        CuFail(tc, err);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c linkedlist_empty PASS\n");
#endif
}

static void
test_linkedlist_size(CuTest *tc)
{
    const char *err = "linkedlist.c linkedlist_size FAIL";
    char *values[] = {"A", "B", "C", "D"};
    linkedlist_t list;
    int i;

    memset(&list, 0, sizeof (linkedlist_t));
    for (i = 0; i < 4; ++i) {
        linkedlist_prepend(&list, values[i]);
    }

    if (linkedlist_size(&list) != 4) {
        CuFail(tc, err);
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c linkedlist_size PASS\n");
#endif
}

static void test_linkedlist_remove(CuTest *tc)
{
    const char *err = "linkedlist.c linkedlist_remove FAIL";
    char *values[] = {"A", "B", "C", "D", "E", "F"};
    char *values2[] = {"A", "B", "D", "E", "F"};
    char *values3[] = {"B", "D", "E"};
    linkedlist_t list;
    lnode_t *node;
    int i;

    memset(&list, 0, sizeof (linkedlist_t));
    for (i = 0; i < 6; ++i) {
        linkedlist_append(&list, values[i]);
    }

    i = 0;
    linkedlist_remove(&list, "C"); // Test removal of element in middle

    LINKEDLIST_FOREACH(node, &list) {
        CuAssertStrEquals_Msg(tc, err, values2[i++], (char *) node->data);
    }

    i = 0;
    linkedlist_remove(&list, "A"); // Test removal of head
    linkedlist_remove(&list, "F"); // Test removal of tail
    linkedlist_remove(&list, "Z"); // Test removal of value not in list

    LINKEDLIST_FOREACH(node, &list) {
        CuAssertStrEquals_Msg(tc, err, values3[i++], (char *) node->data);
    }

    linkedlist_remove(&list, "B");
    linkedlist_remove(&list, "D");
    linkedlist_remove(&list, "E");
    if (!linkedlist_empty(&list)) {
        CuFail(tc, "Linked list remove function: list should be empty");
    }
#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("linkedlist.c linkedlist_remove PASS\n");
#endif
}

CuSuite *
get_linked_list_test_suite()
{
    CuSuite *suite = CuSuiteNew();
#ifdef STATIC_ALLOC_LINKEDLIST_NODES
    SUITE_ADD_TEST(suite, &test_linkedlist_lnode_alloc);
#endif
    SUITE_ADD_TEST(suite, &test_linkedlist_prepend);
    SUITE_ADD_TEST(suite, &test_linkedlist_append);
    SUITE_ADD_TEST(suite, &test_linkedlist_foreach);
    SUITE_ADD_TEST(suite, &test_linkedlist_clear);
    SUITE_ADD_TEST(suite, &test_linkedlist_empty);
    SUITE_ADD_TEST(suite, &test_linkedlist_size);
    SUITE_ADD_TEST(suite, &test_linkedlist_remove);
    return suite;
}