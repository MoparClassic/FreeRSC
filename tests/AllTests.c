#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TestCommon.h"

#include "client.h"
#include "linkedlist.h"
#include "world.h"

int
run_all_tests()
{
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    /*
     * First we test various collections that the application
     * is largely dependent on.
     * This includes the rest of the testing codebase, so it
     * is crucial that these tests pass before we attempt to
     * test anything else using these collections.
     */
    CuSuiteAddSuite(suite, (CuSuite *) get_util_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_linked_list_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_entitystatelist_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_circular_buffer_test_suite());

    /*
     * Everything else gets tested here in an order that
     * makes sense.
     */
    CuSuiteAddSuite(suite, (CuSuite *) get_world_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_client_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_player_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_inventory_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_gameobject_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_item_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_npc_test_suite());
    CuSuiteAddSuite(suite, (CuSuite *) get_clientupdater_test_suite());

    CuSuiteAddSuite(suite, (CuSuite *) get_encryption_test_suite());


    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    return 0;
}

static void
testing_init()
{
#ifdef STATIC_ALLOC_LINKEDLIST_NODES
    printf("Only static memory to be used for linkedlist nodes.\n");
    linkedlist_init();
    printf("\n");
#else
    printf("Linkedlist nodes will be created with heap memory.\n");
#endif

#ifdef STATIC_ALLOC_CLIENTS
    printf("Only static memory to be used for client structures.\n");
    client_init();
    printf("\n");
#else
    printf("Client structures will be created with heap memory.\n");
#endif

    world_init();
}

#define TEST_MACRO(a, d) \
    do {  \
        printf("%d %d %d %d\n", (a)->index, (a)->rofl, (a)->test, d); \
    } while (0)

typedef struct a a_t;
typedef struct b b_t;

struct a {
    int index;
    int rofl;
    int test;
};

struct b {
    int test;
    int index;
    int rofl;
};

int
main(void)
{
    a_t a;
    b_t b;

    a.index = 111;
    a.rofl = 222;
    a.test = 333;

    b.test = 333;
    b.index = 111;
    b.rofl = 222;
    TEST_MACRO(&a, 1337);
    TEST_MACRO(&b, 7331);
    testing_init();
    return run_all_tests();
}
