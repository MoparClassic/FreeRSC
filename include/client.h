/*
 * File:   client.h
 * Author: lothy
 *
 * Created on 13 July 2011, 3:56 PM
 */

#ifndef INCLUDED_CLIENT_H
#define	INCLUDED_CLIENT_H

#include "circularbuffer.h"
#include "player.h"

typedef struct client client_t;

struct client {
    player_t *player;
    cbuffer_t in_buffer;
    cbuffer_t out_buffer;
};

#ifdef STATIC_ALLOC_CLIENTS
void client_init();
#endif

client_t *client_new();
void client_free(client_t *client);
struct ev_io *ev_watcher_new();
void ev_watcher_free(struct ev_io *watcher);

#endif	/* INCLUDED_CLIENT_H */

