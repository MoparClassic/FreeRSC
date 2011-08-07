/*
 * File:   util.h
 * Author: lothy
 *
 * Created on 11 July 2011, 8:45 PM
 */

#ifndef UTIL_H
#define	UTIL_H

#include "config.h"
#include <stdint.h>
#include <stdlib.h>

extern uint64_t timestamp;

#ifndef HAVE_STRDUP
char *strdup(const char *src);
#endif

#ifndef HAVE_STRLCAT
size_t strlcat(char *dst, const char *src, size_t siz);
#endif

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t siz);
#endif

uint64_t get_current_time_millis();

#endif	/* UTIL_H */

