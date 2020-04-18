#ifndef OUTLANDS_UTIL_H_
#define OUTLANDS_UTIL_H_

/* definitely not thread safe lol */
void pool_init(size_t kilobytes);
void pool_cleanup(void);
void pool_usage(void);
void *poolloc(size_t size);
void poolfree(void *p);

extern void *(*allocate)(size_t size);
extern void (*freedom)(void *p);

int rand_range(int min, int max);
float rand_uniform(void);

#endif // OUTLANDS_UTIL_H_
