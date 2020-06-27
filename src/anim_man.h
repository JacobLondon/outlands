#ifndef OUTLANDS_ANI_MAN_H
#define OUTLANDS_ANI_MAN_H

#include <raylib.h>

typedef struct anim_man_tag anim_man;
typedef struct anim_tag anim;

/* Manage the loading and freeing of animations
 * through a manager. This allows you to give
 * the name of an asset to load and use. This
 * manages the animations themselves, so when
 * update is called, all animations managed
 * by the animation manager are updated
 */

anim_man *anim_man_new(void);
void anim_man_del(anim_man *self);

/* Update all anim_man animations */
void anim_man_update(anim_man *self);

/* Get an image if it exists, assert failure if not! */
anim *anim_man_get(anim_man *self, Texture2D *texture);

/* Create a new animation for anim_man to manage, if a png is
 * already loaded, don't do anything.
 * 
 * Return a pointer to the animation
 */
anim *anim_man_load(anim_man *self, Texture2D *texture, int width, int height);

#endif // OUTLANDS_ANI_MAN_H
