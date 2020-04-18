#ifndef OUTLANDS_MAN_H_
#define OUTLANDS_MAN_H_

#include "so.h"

typedef struct man_tag man;

man *man_new(void );
void man_del(man *self);

/* Give ownership to the man */
void man_scene_load(man *self, so *s);
/* Free all scene objects */
void man_scene_clear(man *self);
/* Call draw on all scene objects */
void man_scene_draw(man *self);
/* Call move on all scene objects */
void man_scene_update(man *self);

#endif // OUTLANDS_MAN_H_