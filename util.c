#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void *(*allocate)(size_t size) = malloc;
void (*freedom)(void *p) = free;

static size_t poolloc_index = 0;
static size_t poolloc_size = 0;
static void *poolloc_begin = NULL;

void pool_init(size_t kilobytes)
{
	assert(poolloc_begin == NULL);
	poolloc_begin = malloc(kilobytes * 1024);
	assert(poolloc_begin);
	poolloc_size = kilobytes * 1024;
	memset(poolloc_begin, 0, poolloc_size);
	allocate = poolloc;
	freedom = poolfree;
}

void pool_cleanup(void)
{
	assert(poolloc_begin);
	memset(poolloc_begin, 0, poolloc_size);
	free(poolloc_begin);
}

void pool_usage(void)
{
	printf("%.3f KB / %.3f KB", poolloc_index / 1000.0f, poolloc_size / 1000.0f);
}

void *poolloc(size_t size)
{
	void *addr;
	assert(("Not enough memory", poolloc_index + size < poolloc_size));
	addr = (void *)&((unsigned char *)poolloc_begin)[poolloc_index];
	poolloc_index += size;

	return addr;
}

void poolfree(void *p)
{
	(void)p;
	// It's fine, just drain the pool.
	return;
}

int rand_range(int min, int max)
{
	return rand() % (max - min) + min;
}

float rand_uniform(void)
{
	return (float)rand() / (float)RAND_MAX;
}
