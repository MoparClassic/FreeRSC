/*
 * File:   chatmessage.h
 * Author: lothy
 *
 * Created on 21 July 2011, 10:56 PM
 */

#ifndef INCLUDED_CHATMESSAGE_H
#define	INCLUDED_CHATMESSAGE_H

#define MAX_MESSAGE_LEN 80

typedef struct chatmessage chatmessage_t;

struct chatmessage {
    unsigned int sender_index;
    unsigned int recipient_index;
    char message[MAX_MESSAGE_LEN + 1];
};

#endif	/* INCLUDED_CHATMESSAGE_H */

