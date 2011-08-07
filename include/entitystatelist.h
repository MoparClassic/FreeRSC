/*
 * File:   entitystatelist.h
 * Author: lothy
 *
 * Created on 22 July 2011, 9:38 AM
 */

#ifndef ENTITYSTATELIST_H
#define	ENTITYSTATELIST_H

#include "linkedlist.h"

typedef struct statelist eslist_t;

struct statelist {
    linkedlist_t new_entities;
    linkedlist_t known_entities;
    linkedlist_t entities_to_remove;
};

int eslist_add(eslist_t *sl, void *entity);
int eslist_add_list(eslist_t *sl, linkedlist_t *l);
int eslist_contains(eslist_t *sl, void *entity);
int eslist_remove(eslist_t *sl, void *entity);
int eslist_isremoving(eslist_t *sl, void *entity);
int eslist_update(eslist_t *sl);
int eslist_changed(eslist_t *sl);
int eslist_clear(eslist_t *sl);

#endif	/* ENTITYSTATELIST_H */

