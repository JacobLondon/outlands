#ifndef OUTLANDS_MUSIC_MAN_H
#define OUTLANDS_MUSIC_MAN_H

/**
 * initialize the music module
 */
void music_man_init(void);

/**
 * clean up the music module
 */
void music_man_cleanup(void);

/**
 * Stop all music and sounds and unload them
 */
void music_man_unload(void);

void music_man_update(void);


void music_man_set_volume(float volume);

float music_man_get_volume(void);

/**
 * Load a sound, it cannot be played until
 * this happens
 */
void music_man_load_sound(const char *name);

/**
 * Play a sound once, the sound must have been loaded
 * on init
 */
void music_man_play_sound(const char *name);

/**
 * Play music from a file on loop until cleanup
 */
void music_man_load_music(const char *name);

#endif // OUTLANDS_MUSIC_MAN_H
