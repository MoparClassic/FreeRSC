/*
 * File:   gameobject.h
 * Author: lothy
 *
 * Created on 22 July 2011, 10:15 AM
 */

#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#define MAX_GAMEOBJECT_DEFINITIONS 1000

typedef struct gameobjectdef gameobjectdef_t;
typedef struct gameobject gameobject_t;

struct gameobjectdef {
    char command1[40];
    char command2[40];
    unsigned short object_id;
    unsigned short type;
    unsigned short direction;
    unsigned short height;
    unsigned short width;
};

struct gameobject {
    unsigned short object_id;
    unsigned short x;
    unsigned short y;
    unsigned int flags;
};

extern gameobjectdef_t gameobject_definitions[MAX_GAMEOBJECT_DEFINITIONS];

int gameobject_isontile(gameobject_t *go, int x, int y);
int gameobject_get_type(int go_id);
int gameobject_get_dir(int go_id);

#endif	/* GAMEOBJECT_H */

