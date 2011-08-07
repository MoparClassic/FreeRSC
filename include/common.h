/*
 * File:   common.h
 * Author: lothy
 *
 * Created on 17 July 2011, 4:47 PM
 */

#ifndef COMMON_H
#define	COMMON_H

#define MAX_MAP_WIDTH    944
#define MAX_MAP_HEIGHT   3776

#define UNUSED 0
#define IN_USE -1

#define ENTITY_PLAYER 1
#define ENTITY_NPC    1

#define EF_MOVED               0x0000001
#define EF_SPRITE_CHANGED      0x0000002
#define EF_REMOVING            0x0000004
#define EF_INITIALISED         0x0000008
#define EF_APPEARANCE_CHANGED  0x0000010
#define EF_DESTROYED           0x0000020

#endif	/* COMMON_H */

