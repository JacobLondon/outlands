#ifndef OUTLANDS_TEXTURE_MANAGER_H_
#define OUTLANDS_TEXTURE_MANAGER_H_

#include <raylib.h>

void texman_init(void);
void texman_cleanup(void);

void texman_load(char *png);
Texture2D *texman_get(char *png);

#endif // OUTLANDS_TEXTURE_MANAGER_H_
