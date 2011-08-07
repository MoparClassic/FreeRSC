/*
 * File:   projectile.h
 * Author: lothy
 *
 * Created on 17 July 2011, 4:31 PM
 */

#ifndef PROJECTILE_H
#define	PROJECTILE_H

typedef struct projectile projectile_t;

struct projectile {
    uint16_t caster_index;
    uint16_t victim_index;
    uint8_t victim_type;
    uint8_t projectile_type;
};

#endif	/* PROJECTILE_H */

