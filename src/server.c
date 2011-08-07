#include "server.h"
#include "client.h"
#include "clientupdater.h"
#include "common.h"
#include "listener.h"
#include "util.h"

#include <assert.h>
#include <netinet/in.h>
#include <ev.h>
#include <stdlib.h>
#include <time.h>

#define PORT_NO 43594

static void sigint_cb(struct ev_loop *loop, ev_signal *w, int revents);
static void *game_loop(struct ev_timer *w, int revents);
static void server_init();

int main(int argc, char *argv[])
{
    struct ev_io w_accept;
    struct ev_loop *loop = ev_default_loop(0);
    struct ev_signal sig_watcher;
    struct ev_timer game_pulse_timer;
    struct sockaddr_in addr;
    int sd;

    srand(time(NULL));

    server_init();

    /* Create server socket */
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        return -1;
    }

    memset(&addr, 0, sizeof (struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NO);
    addr.sin_addr.s_addr = INADDR_ANY;

    /* Bind socket to address */
    if (bind(sd, (struct sockaddr*) & addr, sizeof (addr)) != 0) {
        perror("bind error");
    }

    /* Start listing on the socket */
    if (listen(sd, 2) < 0) {
        perror("listen error");
        return -1;
    }

    ev_signal_init(&sig_watcher, &sigint_cb, SIGINT);
    ev_signal_start(loop, &sig_watcher);

    /* Start a timer that calls the game_loop function every 600ms */
    ev_timer_init(&game_pulse_timer, &game_loop, 0, 0.6);
    ev_timer_start(loop, &game_pulse_timer);

    /* Initialize and start a watcher to accepts client requests */
    ev_io_init(&w_accept, accept_cb, sd, EV_READ);
    ev_io_start(loop, &w_accept);

    printf("Starting event loop and listening for connections\n");
    ev_loop(loop, 0);

    return 0;
}

/*
 * A major update is sent every 600ms.
 */
static void *
game_loop(struct ev_timer *w, int revents)
{
    assert(w); /* w must not be a NULL pointer */
    
    timestamp = get_current_time_millis();
    send_client_updates();
}

static void
sigint_cb(struct ev_loop *loop, ev_signal *w, int revents)
{
    assert(loop); /* loop must not be a NULL pointer */
    assert(w); /* w must not be a NULL pointer */
    
    printf("Received SIGINT -- shutting down\n");
    ev_break(loop, EVBREAK_ALL);
}

static void
server_init()
{
#ifdef STATIC_ALLOC_LINKEDLIST_NODES
    printf("Only static memory to be used for linkedlist nodes.\n");
    linkedlist_init();
#else
    printf("Linkedlist nodes will be created with heap memory.\n");
#endif

#ifdef STATIC_ALLOC_CLIENTS
    printf("Only static memory to be used for client structures.\n");
    client_init();
#else
    printf("Client structures will be created with heap memory.\n");
#endif

    world_init();
}
