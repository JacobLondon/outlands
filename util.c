#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void *(*allocate)(size_t size) = malloc;
void (*freedom)(void *p) = free;

static size_t pool_index = 0;
static size_t pool_size = 0;
static void *pool_begin = NULL;

void pool_init(size_t kilobytes)
{
	assert(pool_begin == NULL);
	pool_begin = malloc(kilobytes * 1024);
	assert(pool_begin);
	pool_size = kilobytes * 1024;
	memset(pool_begin, 0, pool_size);
	allocate = poolloc;
	freedom = poolfree;
}

void pool_cleanup(void)
{
	assert(pool_begin);
	memset(pool_begin, 0, pool_size);
	free(pool_begin);
}

void pool_usage(void)
{
	printf("%.3f KB / %.3f KB\n", pool_index / 1000.0f, pool_size / 1000.0f);
}

void *poolloc(size_t size)
{
	void *addr;
	assert(("Not enough memory", pool_index + size < pool_size));
	addr = (void *)&((unsigned char *)pool_begin)[pool_index];
	pool_index += size;

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

int streq(char *s1, char *s2)
{
	for (; *s1 || *s2; s1++, s2++) {
		if (*s1 != *s2) {
			return 0;
		}
	}
	return 1;
}