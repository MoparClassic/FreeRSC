/*
 * File:   listener.h
 * Author: lothy
 *
 * Created on 12 July 2011, 8:23 PM
 */

#ifndef LISTENER_H
#define	LISTENER_H

#include <errno.h>
#include <ev.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

struct server_sock {
    ev_io io;
    int fd;
    struct sockaddr_un sock;
    int sock_len;
};

struct client_sock {
    ev_io io;
    int fd;
    int index;
    struct server_sock *server;
};

int setnonblock(int fd);
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void disconnect(struct ev_loop *loop, struct ev_io *watcher);

#endif	/* LISTENER_H */

