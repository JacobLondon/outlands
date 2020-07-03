#ifndef OUTLANDS_TEXTURE_MAN_H
#define OUTLANDS_TEXTURE_MAN_H

#include <raylib.h>

void texture_man_init(void);
void texture_man_cleanup(void);

/**
 * Load a texture and return a pointer to it,
 * ignore the loading if the texture already exists
 */
Texture2D *texture_man_load(char *png);

/* Extract a texture. If it's not there, assertion */
Texture2D *texture_man_get(char *png);

/**
 *Test drawing routine
 * Only use for test drawing or if animation will not
 * be necessary.
*/
void texture_man_test_draw(Texture2D *texture, int x, int y);

#endif // OUTLANDS_TEXTURE_MAN_H
