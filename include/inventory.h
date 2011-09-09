/*
 * File:   inventory.h
 * Author: lothy
 *
 * Created on 10 August 2011, 2:47 PM
 */

#ifndef INVENTORY_H
#define	INVENTORY_H

#include "player.h"

int add_inventory_item(player_t *player, int item_id, int amount);
int remove_inventory_item(player_t *player, int item_id, int amount);
int add_bank_item(player_t *player, int item_id, int amount);
int remove_bank_item(player_t *player, int item_id, int amount);

#endif	/* INVENTORY_H */

