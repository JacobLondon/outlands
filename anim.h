#ifndef OUTLANDS_ANIM_H_
#define OUTLANDS_ANIM_H

#include <raylib.h>

typedef struct anim_tag anim;

anim *anim_new(char *png, int width, int height);
void anim_del(anim *self);

void anim_update(anim *self);
void anim_draw(anim *self, Vector2 pos);

int anim_get_width(anim *self);
int anim_get_height(anim *self);

#endif // OUTLANDS_ANIM_H_
