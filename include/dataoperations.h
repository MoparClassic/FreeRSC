/*
 * File:   dataoperations.h
 * Author: lothy
 *
 * Created on 30 July 2011, 9:27 PM
 */

#ifndef DATAOPERATIONS_H
#define	DATAOPERATIONS_H

#include <stdint.h>
#include <stdlib.h>

uint8_t dops_read_byte(void *data, int ofs);
uint16_t dops_read_short(void *data, int ofs);
uint32_t dops_read_int(void *data, int ofs);
uint64_t dops_read_long(void *data, int ofs);
int dops_decrypt_data(void *data, size_t siz, int ofs);

#endif	/* DATAOPERATIONS_H */

