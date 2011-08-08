#include "listener.h"
#include "client.h"
#include "circularbuffer.h"
#include "packethandler.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

static int
setnonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

/* Accept client requests */
void
accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    struct sockaddr_in client_addr;
    struct ev_io *w_client;
    client_t *client;
    socklen_t client_len = sizeof (client_addr);
    int client_sd;

    assert(loop); /* loop must not be a NULL pointer */
    assert(watcher); /* watcher must not be a NULL pointer */

    if (EV_ERROR & revents) {
        perror("got invalid event");
        return;
    }

    /* Accept client request */
    client_sd = accept(watcher->fd,
                       (struct sockaddr *) & client_addr, &client_len);

    if (client_sd < 0) {
        perror("accept error");
        return;
    }
    /* Set the client socket fd to non-blocking mode */
    setnonblock(client_sd);

    printf("Successfully connected with client.\n");

    /* Initialize and start watcher to read client requests */
    client = client_new();
    client->in_buffer.fd = client_sd;
    client->out_buffer.fd = client_sd;
    w_client = ev_watcher_new();
    w_client->attachment = client;
    ev_io_init(w_client, &read_cb, client_sd, EV_READ);
    ev_io_start(loop, w_client);
}

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    ssize_t read;
    client_t *client = (client_t *) watcher->attachment;
    cbuffer_t *inbuf = &client->in_buffer;
    int buf_space = sizeof (inbuf->buffer) - inbuf->write_offset;
    int avail;
    int opcode;
    int plen;

    assert(loop); /* loop must not be a NULL pointer */
    assert(watcher); /* watcher must not be a NULl pointer */

    /* If the event is invalid then there's nothing to do */
    if (EV_ERROR & revents) {
        perror("Invalid read event");
        return;
    }

    /*
     * If the write pointer == BUFFER_SIZE then there is no
     * space left at the end of the buffer to write to.
     * We need to wrap around back to the beginning of the
     * buffer. ie, write_offset should point to index zero.
     */
    if (0 == buf_space) {
        inbuf->write_offset = 0;
        buf_space = sizeof (inbuf->buffer);
    }

    read = recv(watcher->fd, &inbuf->buffer[inbuf->write_offset], buf_space, 0);
    if (read < 0) {
        if (errno == EAGAIN) {
            return; /* Nothing to read */
        }

        /* More serious error */
        perror(strerror(errno));
        return;
    }
    if (0 == read) {
        disconnect(loop, watcher);
        return;
    }

    /* Some data was read into the buffer, so increase the write ptr */
    inbuf->write_offset += read;

    printf("Data received: %d\n", read);

    /*
     * If the first read used up all of the space that was
     * remaining in the circular buffer then there may still
     * be data that needs to be read.
     * We need to wrap back around to the start of the
     * circular buffer and try another call to recv.
     */
    if (read == buf_space) {
        inbuf->write_offset = 0;
        buf_space = sizeof (inbuf->buffer);
        read = recv(watcher->fd, &inbuf->buffer[inbuf->write_offset],
                    buf_space - read, 0);

        if (read < 0) {
            if (errno != EAGAIN) {
                perror(strerror(errno));
                return;
            }
        }
        if (0 == read) {
            disconnect(loop, watcher);
            return;
        }

        inbuf->write_offset += read;
    }

    printf("Handling packet\n");

    /*
     * Now that we've read as much as we can, it's time to
     * start handling the data that we have received.
     */
    while (1) {
        cbuffer_mark_read_position(inbuf);
        avail = cbuffer_available(inbuf);

        if (avail >= 2) {
            plen = cbuffer_read_short(inbuf);
            if (avail - 2 >= plen) {
                opcode = cbuffer_read_byte(inbuf);
                --plen; /* Minus one because we just read the opcode */
                (*packet_decoder_table[opcode])(client, opcode, plen);
                continue;
            }
            else {
                cbuffer_rewind_read_position(inbuf);
                return;
            }
        }
        else {
            cbuffer_rewind_read_position(inbuf);
            return;
        }
    }
}

void disconnect(struct ev_loop *loop, struct ev_io *watcher)
{
    assert(loop); /* loop must not be a NULL pointer */
    assert(watcher); /* watcher must not be a NULL pointer */

    ev_io_stop(loop, watcher);
    client_free((client_t *) watcher->attachment);
    ev_watcher_free(watcher);
    printf("Client disconnected\n");
}