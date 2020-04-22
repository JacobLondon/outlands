#ifndef OUTLANDS_IMGER_H_
#define OUTLANDS_IMGER_H_

typedef struct imger_tag imger;
typedef struct anim_tag anim;

imger *imger_new(void);
void imger_del(imger *self);

/* Update all imger animations */
void imger_update(imger *self);

/* Get an image if it exists, assert failure if not! */
anim *imger_get(imger *self, char *png);

/* Create a new animation for imger to manage */
void imger_load(imger *self, char *png, int width, int height);

#endif // OUTLANDS_IMGER_H_
