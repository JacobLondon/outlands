#include <memory.h>
#include <stddef.h>
#include "anim_man.h"
#include "key_object.h"
#include "key_man.h"
#include "texture_man.h"
#include "util.h"

#define KEY_OBJECTS_MAX 32

typedef void (*load_cb)(ko *self);

/* definitions to look for when loading */
typedef struct key_definition_tag {
	char *name;
	load_cb load;
} key_definition;

static void load_cb_laser(ko *self);

static key_definition defs[] = {
	{ "Missile", load_cb_laser },
	{ NULL, NULL }
};

static bool initialized = false;
static ko *key_objects[KEY_OBJECTS_MAX] = { NULL };


void key_man_init(void)
{
	assert(initialized == false);
	memset(key_objects, 0, sizeof(key_objects));
	initialized = true;
}

void key_man_cleanup(void)
{
	int i;
	assert(initialized == true);
	for (i = 0; i < KEY_OBJECTS_MAX; i++) {
		if (key_objects[i]) {
			ko_del(key_objects[i]);
		}
	}
	memset(key_objects, 0, sizeof(key_objects));
	initialized = false;
}

void key_man_load(char **names)
{
	key_definition *d;
	int i;
	ko *tmp;
	assert(names != NULL);
	assert(initialized == true);

	for (i = 0; names[i] != NULL; i++) {
		for (d = defs; d->name; d++) {
			if (streq(names[i], d->name)) {
				tmp = ko_new();
				d->load(tmp);
				key_objects[i] = tmp;
				break;
			}
		}

		/* should not get to the end of defs */
		if (d->name == NULL) {
			msg_assert("Key definition not found", 0);
		}
	}
}

void key_man_update(void)
{
	int i;
	assert(initialized == true);
	
	// update all key objects that exist
	for (i = 0; i < KEY_OBJECTS_MAX; i++) {
		if (key_objects[i]) {
			ko_update(key_objects[i]);
		}
	}
}


/*****************************************************************************
 * 
 * Key Object Callback Definitions
 * 
 * No animation manager. This is because key objects taken ownership
 * of their animations, so passing an owner pointer with anim_new is
 * correct, while anim_man_load or something is wrong, as animation managers
 * own, and do not transfer ownership.
 * 
 ****************************************************************************/

static void ko_cb_laser_0(ko *self, so *object)
{
	//so_set_pos(object, GetScreenWidth(), GetScreenHeight() / 2);
	ko_set_pos(self, GetScreenWidth(), GetMouseY());
	// wait until the key press, then go!
	if (IsKeyPressed(KEY_J)) {
		ko_set_key(self, true);
	}
}

static void ko_cb_laser_1(ko *self, so *object)
{
	const Vector2 p = so_get_pos(object);
	const int x = GetMouseX();

	// once to the right of, close enough, next state
	if (p.x < x) {
		ko_set_key(self, true);
	}
}

static void ko_cb_laser_2(ko *self, so *object)
{
	const int w = anim_get_width(so_get_anim(object));
	const int h = anim_get_height(so_get_anim(object));
	so_set_pos(object, GetMouseX() - w / 2, GetMouseY() - h / 2);
	// just animate and stop near the end
	if (ko_get_frame(self) >= ko_get_max_frames(self)) {
		ko_set_key(self, true);
	}
}

static void load_cb_laser(ko *self)
{
	so *tmp;
	Texture2D *missile = texture_man_load("assets/missile1.png");
	Texture2D *explosion = texture_man_load("assets/explosion3.png");
	
	tmp = so_new_own(anim_new(missile, 1, 1));
	so_set_pos(tmp, GetScreenWidth(), GetScreenHeight() / 2);
	so_newmov(tmp, so_cb_left, 30, NULL);
	so_newmov(tmp, so_cb_trk_vrt, 2, NULL);
	ko_add(self, tmp, ko_cb_laser_0, NULL); // spawn on press
	ko_add_rate(self, tmp, ko_cb_laser_1, NULL, 8); // move to mouse
	tmp = so_new_own(anim_new(explosion, 4, 4));
	ko_add_rate(self, tmp, ko_cb_laser_2, NULL, 20); // explode
}
