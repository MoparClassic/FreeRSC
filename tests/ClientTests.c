#include "ClientTests.h"

#ifdef STATIC_ALLOC_CLIENTS

static void
test_client_new(CuTest *tc)
{
    const char *err = "client.c client_new FAIL";
    client_t *c1, *c2;
    int max = STATIC_ALLOC_CLIENTS; /* For testing, use 10 is plenty */
    int middle = max / 2;
    int i;

    if (STATIC_ALLOC_CLIENTS % 2 == 1) {
        CuFail(tc, "client.c client_new FAIL -- "
               "rerun with even number of clients allocated");
    }

    for (i = 0; i < max; ++i) {
        c1 = client_new();
        if (i == middle - 1) {
            //printf("Freeing client %d\n", i);
            client_free(c1);
            c1 = NULL;
        }
    }

    c1 = client_new();
    client_free(c1);
    c2 = client_new();
    CuAssertPtrEquals_Msg(tc, err, c1, c2);

    c1 = client_new();
    CuAssertPtrEquals_Msg(tc, err, NULL, c1);

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("client.c client_new PASS\n");
#endif
}

static void
test_ev_watcher_new(CuTest *tc)
{
    const char *err = "client.c ev_watcher_new FAIL";
    struct ev_io *e1, *e2;
    int max = STATIC_ALLOC_CLIENTS;
    int middle = max / 2;
    int i, j;

    if (STATIC_ALLOC_CLIENTS % 2 == 1) {
        CuFail(tc, "client.c ev_watcher_new FAIL -- "
               "rerun with even number of clients allocated");
    }

    for (i = 0; i < max; ++i) {
        e1 = ev_watcher_new();
        if (i == middle - 1) {
            //printf("Freeing ev_io watcher %d\n", i);
            ev_watcher_free(e1);
            e1 = NULL;
        }
    }

    /*
     * Here we reacquire a pointer to the single ev_io struct that
     * was freed in the loop above.
     * We then free this again, and then acquire it again to make
     * sure that every single structure is checked before the
     * function returns NULL.
     * This means that e1 == e2, as we're reacquiring the same
     * ev_io struct.
     */
    e1 = ev_watcher_new();
    ev_watcher_free(e1);
    e2 = ev_watcher_new();
    CuAssertPtrEquals_Msg(tc, err, e1, e2);

    /*
     * At this stage, all of the ev_io structures have been
     * allocated.
     * The function should loop over everything, and then return
     * NULL to indicate that there is no memory left.
     */
    e1 = ev_watcher_new();
    CuAssertPtrEquals_Msg(tc, err, NULL, e1);

#if UNIT_TESTING_VERBOSITY_LEVEL >= 1
    printf("client.c ev_watcher_new PASS\n");
#endif
}

#endif

CuSuite *
get_client_test_suite()
{
    CuSuite *suite = CuSuiteNew();
#ifdef STATIC_ALLOC_CLIENTS
    SUITE_ADD_TEST(suite, &test_client_new);
    SUITE_ADD_TEST(suite, &test_ev_watcher_new);
#endif
    return suite;
}
