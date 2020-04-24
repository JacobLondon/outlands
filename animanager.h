#ifndef OUTLANDS_ANIMANAGER_H_
#define OUTLANDS_ANIMANAGER_H_

typedef struct animan_tag animan;
typedef struct anim_tag anim;

/* Manage the loading and freeing of animations
 * through a manager. This allows you to give
 * the name of an asset to load and use. This
 * manages the animations themselves, so when
 * update is called, all animations managed
 * by the animation manager are updated
 */

animan *animan_new(void);
void animan_del(animan *self);

/* Update all animan animations */
void animan_update(animan *self);

/* Get an image if it exists, assert failure if not! */
anim *animan_get(animan *self, char *png);

/* Create a new animation for animan to manage */
void animan_load(animan *self, char *png, int width, int height);

#endif // OUTLANDS_ANIMANAGER_H_
