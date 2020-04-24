#ifndef OUTLANDS_ANIMANAGER_H_
#define OUTLANDS_ANIMANAGER_H_

typedef struct animan_tag animan;
typedef struct anim_tag anim;

animan *animan_new(void);
void animan_del(animan *self);

/* Update all animan animations */
void animan_update(animan *self);

/* Get an image if it exists, assert failure if not! */
anim *animan_get(animan *self, char *png);

/* Create a new animation for animan to manage */
void animan_load(animan *self, char *png, int width, int height);

#endif // OUTLANDS_ANIMANAGER_H_
