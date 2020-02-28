#include "util.h"

size_t get_time_difference(struct timeval x , struct timeval y)
{
	size_t x_ms , y_ms , diff;
	
	x_ms = (size_t)x.tv_sec*1000000 + (size_t)x.tv_usec;
	y_ms = (size_t)y.tv_sec*1000000 + (size_t)y.tv_usec;
	
	diff = (size_t)y_ms - (size_t)x_ms;
	
	return diff;
}

size_t get_rand(size_t min, size_t max)
{
	size_t num;
	getrandom(&num, sizeof(num), 0u);
	return min + (num % (max - min));
}