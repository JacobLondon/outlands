#ifndef OUTLANDS_IMGER_H_
#define OUTLANDS_IMGER_H_

typedef struct imger_tag imger;
typedef struct Texture2D Texture2D;

imger *imger_new(void);
void imger_del(imger *self);
/* Malloc a texture into memory if it is not already
 * there, return a pointer to the texture whether it
 * was malloc'ed or found */
Texture2D *imger_get(imger *self, const char *png);

#endif // OUTLANDS_IMGER_H_
