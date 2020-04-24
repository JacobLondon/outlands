#ifndef OUTLANDS_UTIL_H_
#define OUTLANDS_UTIL_H_

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

#endif // OUTLANDS_UTIL_H_
