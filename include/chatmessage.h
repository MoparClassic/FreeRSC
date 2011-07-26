/*
 * File:   chatmessage.h
 * Author: lothy
 *
 * Created on 21 July 2011, 10:56 PM
 */

#ifndef CHATMESSAGE_H
#define	CHATMESSAGE_H

#include "common.h"

#define MAX_MESSAGE_LEN 80

typedef struct chatmessage chatmessage_t;

struct chatmessage {
    uint16_t sender_index;
    uint16_t recipient_index;
    char message[MAX_MESSAGE_LEN + 1];
};

#endif	/* CHATMESSAGE_H */

