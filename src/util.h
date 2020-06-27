#ifndef OUTLANDS_UTIL_H
#define OUTLANDS_UTIL_H

#include <assert.h>
#include <stddef.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#ifndef NDEBUG
	#include <stdio.h>
	#define msg_assert(Msg, Expr) do { \
		if (!!(Expr)) {} \
		else { \
			fprintf(stderr, "assert: " Msg "\n"); \
			assert(Expr); \
		} \
	} while (0)
#else
	#define msg_assert(Msg, Expr) ((void)0)
#endif // NDEBUG

/* definitely not thread safe lol */
void pool_init(size_t kilobytes);
void pool_cleanup(void);
void pool_usage(void);
void *poolloc(size_t size);
void poolfree(void *p);

/* Use allocate and dealloc for dynamic
 * memory allocation. If pool isn't
 * initialized, it just used malloc and
 * free. But if pool is initialized, then
 * poolloc and poolfree is used instad.
 * On pool cleanup, malloc and free are
 * retied to these pointers.
 */
extern void *(*allocate)(size_t size);
extern void (*dealloc)(void *p);

int rand_range(int min, int max);
float rand_uniform(void);

/* 0 -> false, 1-> true */
int streq(char *s1, char *s2);

char *file_read(const char *fname, size_t *size);
int file_write(const char *fname, const char *str);
int file_append(const char *fname, const char *str);

int arg_check(int argc, char **argv, const char *arg);
char *arg_get(int argc, char **argv, const char *arg);

float fast_sqrtf(float number);

#endif // OUTLANDS_UTIL_H
