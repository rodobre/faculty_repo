#ifndef _UTIL_H
#define _UTIL_H
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

#define SAFE_FREE(x) { free(x); x = NULL; }
size_t get_time_difference(struct timeval x, struct timeval y);
size_t get_rand(size_t min, size_t max);
#endif