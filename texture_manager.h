#ifndef OUTLANDS_TEXTURE_MANAGER_H_
#define OUTLANDS_TEXTURE_MANAGER_H_

#include <raylib.h>

void texman_init(void);
void texman_cleanup(void);

/* Load a texture and return a pointer to it,
 * ignore the loading if the texture already exists */
Texture2D *texman_load(char *png);

/* Extract a texture. If it's not there, assertion */
Texture2D *texman_get(char *png);

#endif // OUTLANDS_TEXTURE_MANAGER_H_
