/*
 * File:   gameobject.h
 * Author: lothy
 *
 * Created on 22 July 2011, 10:15 AM
 */

#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#include "common.h"

#define MAX_GAMEOBJECT_DEFINITIONS 1000

typedef struct gameobjectdef gameobjectdef_t;
typedef struct gameobject gameobject_t;

struct gameobjectdef {
    char command1[40];
    char command2[40];
    uint16_t object_id;
    uint16_t type;
    uint16_t direction;
    uint16_t height;
    uint16_t width;
};

struct gameobject {
    uint16_t object_id;
    uint16_t x;
    uint16_t y;
    uint32_t flags;
};

extern gameobjectdef_t gameobject_definitions[MAX_GAMEOBJECT_DEFINITIONS];

int gameobject_set_flags(gameobject_t *go, int flags);
int gameobject_unset_flags(gameobject_t *go, int flags);
int gameobject_isontile(gameobject_t *go, int x, int y);
int gameobject_get_type(int go_id);
int gameobject_get_dir(int go_id);

#endif	/* GAMEOBJECT_H */

