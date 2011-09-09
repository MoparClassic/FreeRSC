/*
 * File:   entityutil.h
 * Author: lothy
 *
 * Created on 7 August 2011, 7:14 PM
 */

#ifndef ENTITYUTIL_H
#define	ENTITYUTIL_H

#include "common.h"
#include "linkedlist.h"

#define ENTITY_SET_FLAGS(e, f) (e)->flags |= (f);
#define ENTITY_UNSET_FLAGS(e, f) (e)->flags &= ~(f);

#define MOB_RESET_MOVED(e) (e)->flags &= ~EF_MOVED;

inline int within_range(int x, int y, int targ_x, int targ_y);
inline int get_height(int y);
int get_viewed_area(int mob_x, int mob_y, linkedlist_t *tile_list,
                    int x1, int y1, int x2, int y2);

#endif	/* ENTITYUTIL_H */

