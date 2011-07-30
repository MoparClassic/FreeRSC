#include "entitystatelist.h"

int eslist_add(eslist_t *sl, void *entity)
{
    linkedlist_prepend(&sl->new_entities, entity);
    return 1;
}

int eslist_add_list(eslist_t *sl, linkedlist_t *l)
{
    lnode_t *node;

    LINKEDLIST_FOREACH(node, l) {
        linkedlist_prepend(&sl->new_entities, node->data);
    }
    return 1;
}

int eslist_contains(eslist_t *sl, void *entity)
{
    lnode_t *node;

    LINKEDLIST_FOREACH(node, &sl->known_entities) {
        if (node->data == entity) {
            return 1;
        }
    }

    LINKEDLIST_FOREACH(node, &sl->new_entities) {
        if (node->data == entity) {
            return 1;
        }
    }

    return 0; /* Entity is not in the entitystatelist */
}

int eslist_remove(eslist_t *sl, void *entity)
{
    linkedlist_prepend(&sl->entities_to_remove, entity);
    return 1;
}

int eslist_isremoving(eslist_t *sl, void *entity)
{
    lnode_t *node;

    LINKEDLIST_FOREACH(node, &sl->entities_to_remove) {
        if (node->data == entity) {
            return 1;
        }
    }

    return 0;
}

int eslist_update(eslist_t *sl)
{
    lnode_t *node;

    LINKEDLIST_FOREACH(node, &sl->entities_to_remove) {
        linkedlist_remove(&sl->known_entities, node->data);
    }

    LINKEDLIST_FOREACH(node, &sl->new_entities) {
        linkedlist_prepend(&sl->known_entities, node->data);
    }

    linkedlist_clear(&sl->new_entities);
    linkedlist_clear(&sl->entities_to_remove);
    return 1;
}

int eslist_changed(eslist_t *sl)
{
    return linkedlist_size(&sl->entities_to_remove) ||
           linkedlist_size(&sl->new_entities);
}

int eslist_clear(eslist_t *sl)
{
    linkedlist_clear(&sl->entities_to_remove);
    linkedlist_clear(&sl->known_entities);
    linkedlist_clear(&sl->new_entities);
    return 1;
}