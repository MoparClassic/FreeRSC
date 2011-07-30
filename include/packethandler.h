/*
 * File:   packet_handler.h
 * Author: lothy
 *
 * Created on 12 July 2011, 8:24 PM
 */

#ifndef PACKET_HANDLER_H
#define	PACKET_HANDLER_H

#include "client.h"
#include "common.h"

extern int (*packet_decoder_table[256])(client_t *,
                                        uint32_t opcode, uint32_t psiz);

#endif	/* PACKET_HANDLER_H */

