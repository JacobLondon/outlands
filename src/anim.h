#ifndef OUTLANDS_ANIM_H
#define OUTLANDS_ANIM_H

#include <raylib.h>

/**
 * Manage drawing Texture2D's from raylib. This
 * includes drawing animated textures for when
 * there are multiple frames to draw for an
 * individual Texture2D.
 */

typedef struct anim_tag anim;

anim *anim_new(Texture2D *texture, int width, int height);
void anim_del(anim *self);
anim *anim_copy(anim *self);

/* reset the animation to the beginning */
void anim_reset(anim *self);
void anim_update(anim *self);
void anim_draw(anim *self, Vector2 pos);

int anim_get_width(anim *self);
int anim_get_height(anim *self);

/* get the width * height for the number of animated frames */
int anim_get_frames(anim *self);

#endif // OUTLANDS_ANIM_H
