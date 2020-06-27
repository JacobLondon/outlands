#ifndef OUTLANDS_CONTEXT_H
#define OUTLANDS_CONTEXT_H

void context_init(void);
void context_cleanup(void);
void context_reload(void);
void context_update(void);
void context_draw(void);
void context_set_scene(unsigned idx);

#endif // OUTLANDS_CONTEXT_H
