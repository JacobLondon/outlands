#ifndef OUTLANDS_CONTEXT_H_
#define OUTLANDS_CONTEXT_H_

void context_init(void);
void context_cleanup(void);
void context_reload(void);
void context_update(void);
void context_draw(void);
void context_set_scene(unsigned idx);

#endif // OUTLANDS_CONTEXT_H_
