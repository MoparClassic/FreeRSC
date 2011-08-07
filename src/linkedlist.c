/*
 * File:   linkedlist.c
 */

#include "linkedlist.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifdef STATIC_ALLOC_LINKEDLIST_NODES

#if STATIC_ALLOC_LINKEDLIST_NODES > 1
#define MAX_LINKEDLIST_NODES STATIC_ALLOC_LINKEDLIST_NODES
#else
#define MAX_LINKEDLIST_NODES 5000000
#endif

static struct lnode_ref {
    unsigned int used;
    lnode_t node;
};

static struct lnode_ref lnode_ref_list[MAX_LINKEDLIST_NODES + 1];

void
linkedlist_init()
{
    memset(lnode_ref_list, 0, sizeof (lnode_ref_list));
}
#endif

lnode_t *
lnode_t_alloc()
{
#ifdef STATIC_ALLOC_LINKEDLIST_NODES
    struct lnode_ref *noderef = NULL;
    lnode_t *node = NULL;
    static int i = 0;
    int j;

    for (j = 0; j < MAX_LINKEDLIST_NODES; ++j, ++i) {
        i %= MAX_LINKEDLIST_NODES;
        noderef = &lnode_ref_list[i + 1];
        if (noderef->used == UNUSED) {
            noderef->used = IN_USE;
            node = &noderef->node;
            memset(node, 0, sizeof (node));
            break;
        }
    }
    i = (i + 1) % MAX_LINKEDLIST_NODES;
    return node;
#else
    lnode_t *node = (lnode_t *) malloc(sizeof (lnode_t));
    memset(node, 0, sizeof (lnode_t));
    return node;
#endif
}

void lnode_t_free(lnode_t *node)
{
    assert(node); /* node must not be a NULL pointer */
    
    if (NULL == node) {
        return;
    }
#ifdef STATIC_ALLOC_LINKEDLIST_NODES
    unsigned int *usedptr = (unsigned int *) (((unsigned char *) node) - sizeof (unsigned int));

    *usedptr = UNUSED;
    memset(node, 0, sizeof (lnode_t));
#else
    free(node);
#endif
}

linkedlist_t *
linkedlist_new()
{
    linkedlist_t *list = (linkedlist_t *) malloc(sizeof (linkedlist_t));
    memset(list, 0, sizeof (linkedlist_t));
    return list;
}

void
linkedlist_free(linkedlist_t *list)
{
    assert(list); /* list must not be a NULL pointer */
    
    linkedlist_clear(list);
    free(list);
}

int
linkedlist_clear(linkedlist_t *list)
{
    lnode_t *cur;
    lnode_t *next;
    int count = 0;

    assert(list); /* list must not be a NULL pointer */

    cur = list->head;
    while (NULL != cur) {
        next = cur->next;
        --list->size;
        lnode_t_free(cur);
        ++count;

        cur = next;
    }

    list->head = NULL;
    return count;
}

lnode_t *
linkedlist_append(linkedlist_t *list, void *value)
{
    lnode_t *node;
    lnode_t *cur;
    lnode_t *prev;

    assert(list); /* list must not be a NULL pointer */
    assert(value); /* value must not be a NULL pointer */

    node = lnode_t_alloc();
    memset(node, 0, sizeof (lnode_t));
    node->data = value;

    /* If there is currently no head node, make this node the head */
    if (NULL == list->head) {
        list->head = node;
        return node;
    }

    /* Find the last node in the list, and assign it to prev */
    for (cur = list->head; NULL != cur; prev = cur, cur = cur->next);

    prev->next = node; /* Set the next field in prev to the new node */
    ++list->size;
    return node; /* Will not reach this point */
}

lnode_t *
linkedlist_prepend(linkedlist_t *list, void *value)
{
    lnode_t *node;

    assert(list); /* list must not be a NULL pointer */
    assert(value); /* value must not be a NULL pointer */

    node = lnode_t_alloc();
    memset(node, 0, sizeof (lnode_t));
    node->data = value;
    node->next = list->head;
    list->head = node;
    ++list->size;
    return node;
}

int
linkedlist_remove(linkedlist_t *list, void *value)
{
    lnode_t *node = list->head;
    lnode_t *prev = list->head;

    assert(list); /* list must not be a NULL pointer */
    assert(value); /* value must not be a NULL pointer */

    if (NULL == node) {
        return 0;
    }

    for (; NULL != node && node->data != value; prev = node, node = node->next);

    if (NULL == node) {
        return 0;
    }

    if (node != list->head) {
        --list->size;
        prev->next = node->next;
        lnode_t_free(node);
        return 1;
    }
    else {
        --list->size;
        list->head = node->next;
        lnode_t_free(node);
        return 1;
    }

    return 0; /* Nothing removed */
}

int
linkedlist_empty(linkedlist_t *list)
{
    assert(list); /* list must not be a NULL pointer */
    return NULL == list->head;
}

int
linkedlist_size(linkedlist_t *list)
{
    /*lnode_t *node = list->head;
    int count = 0;

    for (; NULL != node; node = node->next, ++count);

    return count;*/

    assert(list); /* list must not be a NULL pointer */
    return list->size;
}

void *
linkedlist_poll(linkedlist_t *list)
{
    lnode_t *node = list->head;
    void *val;

    assert(list); /* list must not be a NULL pointer */

    if (NULL == node) {
        return NULL;
    }

    val = node->data;
    list->head = node->next;
    --list->size;
    lnode_t_free(node);
    return val;
}