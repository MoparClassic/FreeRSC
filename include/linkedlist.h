/*
 * File:   linkedlist.h
 * Author: lothy
 *
 * Created on 14 July 2011, 9:08 PM
 */

#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

#define LINKEDLIST_FOREACH(item, list) \
    for ((item) = (list)->head; (item); (item) = (item)->next)

typedef struct linkedlist linkedlist_t;
typedef struct list_node lnode_t;

struct list_node {
    void *next;
    void *data;
};

struct linkedlist {
    lnode_t *head;
    unsigned int size;
};

#ifdef STATIC_ALLOC_LINKEDLIST_NODES
void linkedlist_init();
#endif

linkedlist_t * linkedlist_new();
void linkedlist_free(linkedlist_t *list);
int linkedlist_clear(linkedlist_t *list);
lnode_t * linkedlist_append(linkedlist_t *list, void *value);
lnode_t * linkedlist_prepend(linkedlist_t *list, void *value);
int linkedlist_remove(linkedlist_t *list, void *value);
int linkedlist_empty(linkedlist_t *list);
int linkedlist_size(linkedlist_t *list);
void * linkedlist_poll(linkedlist_t *list);


#endif	/* LINKEDLIST_H */
