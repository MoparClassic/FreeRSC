/*
 * File:   packet_handler.h
 * Author: lothy
 *
 * Created on 12 July 2011, 8:24 PM
 */

#ifndef PACKET_HANDLER_H
#define	PACKET_HANDLER_H

#include "client.h"

extern int (*packet_decoder_table[256])(client_t *,
                                        unsigned int opcode, int packet_size);

int discard(client_t *, unsigned int opcode, int packet_size);
int login(client_t *, unsigned int opcode, int packet_size);
int logout(client_t *, unsigned int opcode, int packet_size);


#endif	/* PACKET_HANDLER_H */

