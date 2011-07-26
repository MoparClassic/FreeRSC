#include "client.h"

#include <stdio.h>

#ifdef STATIC_ALLOC_CLIENTS

#if STATIC_ALLOC_CLIENTS > 1
#define MAX_CLIENTS STATIC_ALLOC_CLIENTS
#else
#define MAX_CLIENTS 1000
#endif

static struct client_ref {
    uint32_t used;
    client_t client;
};

static struct ev_io_ref {
    uint32_t used;
    struct ev_io watcher;
};

static struct client_ref client_ref_list[MAX_CLIENTS + 1];
static struct ev_io_ref watcher_ref_list[MAX_CLIENTS + 1];

void client_init()
{
    memset(client_ref_list, 0, sizeof (client_ref_list));
    memset(watcher_ref_list, 0, sizeof (watcher_ref_list));
    printf("Client structures available: %d\n", MAX_CLIENTS);
}
#endif

client_t *
client_new()
{
#ifdef STATIC_ALLOC_CLIENTS
    struct client_ref *cref = NULL;
    client_t *client = NULL;
    static int i = 0;
    int j;

    for (j = 0; j < MAX_CLIENTS; ++j, ++i) {
        i %= MAX_CLIENTS;
        cref = &client_ref_list[i + 1];
        if (cref->used == UNUSED) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
            printf("Client found! ID=%d\n", i + 1);
#endif
            cref->used = IN_USE;
            client = &cref->client;
            memset(client, 0, sizeof (client_t));
            break;
        }
    }
    i = (i + 1) % MAX_CLIENTS;
    return client;
#else
    client_t *client = (client_t *) malloc(sizeof (client_t));
    memset(client, 0, sizeof (client_t));
    return client;
#endif
}

void client_free(client_t *client)
{
#ifdef STATIC_ALLOC_CLIENTS
    uint32_t *usedptr = (uint32_t *) (((uint8_t *) client) - sizeof (uint32_t));

    *usedptr = UNUSED;
    memset(client, 0, sizeof (client_t));
#else
    free(client);
#endif
}

struct ev_io *
ev_watcher_new() {
#ifdef STATIC_ALLOC_CLIENTS
    struct ev_io_ref *ref = NULL;
    struct ev_io *watcher = NULL;
    static int i = 0;
    int j;

    for (j = 0; j < MAX_CLIENTS; ++j, ++i) {
        i %= MAX_CLIENTS;
        ref = &watcher_ref_list[i + 1];
        if (ref->used == UNUSED) {
#if UNIT_TESTING_VERBOSITY_LEVEL >= 2
            printf("ev_watcher found! ID=%d\n", i + 1);
#endif
            ref->used = IN_USE;
            watcher = &ref->watcher;
            memset(watcher, 0, sizeof (struct ev_io));
            break;
        }
    }
    i = (i + 1) % MAX_CLIENTS;
    return watcher;
#else
    struct ev_io *watcher = (struct ev_io *) malloc(sizeof (struct ev_io));
    memset(watcher, 0, sizeof (struct ev_io));
    return watcher;
#endif
}

void ev_watcher_free(struct ev_io *watcher)
{
#ifdef STATIC_ALLOC_CLIENTS
    uint32_t *usedptr = (uint32_t *) (((uint8_t *) watcher) - sizeof (uint32_t));

    *usedptr = UNUSED;
    memset(watcher, 0, sizeof (struct ev_io));
#else
    free(watcher);
#endif
}