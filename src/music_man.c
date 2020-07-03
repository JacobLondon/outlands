#include <stdbool.h>
#include <raylib.h>
#include "music_man.h"
#include "globals.h"
#include "util.h"

#define MUSIC_REPEAT_COUNT 100
static bool initialized = false;
static float master_volume = DEFAULT_VOLUME;

typedef struct sound_def_tag {
	const char *name;
	const char *filename;
	Sound sound;
	bool loaded;
} sound_def;

typedef struct music_def_tag {
	const char *name;
	const char *filename;
	Music music;
	bool loaded;
} music_def;

static sound_def sound_definitions[] = {
	{ .name = "Thump", .filename = AUDIO_DIRECTORY "/sound_thump.mp3", .sound = { 0 }, .loaded = false},
	{ .name = NULL, .filename = NULL, .sound = { 0 } }
};

static music_def music_definitions[] = {
	{ .name = "Flow",     .filename = AUDIO_DIRECTORY "/song1.mp3", .music = { 0 }, .loaded = false },
	{ .name = "Floating", .filename = AUDIO_DIRECTORY "/song2.mp3", .music = { 0 }, .loaded = false },
	{ .name = NULL, .filename = NULL, .music = { 0 }, .loaded = false }
};

void music_man_init(void)
{
	if (initialized) {
		return;
	}

	InitAudioDevice();
	while (!IsAudioDeviceReady())
		;
	SetMasterVolume(master_volume);
	initialized = true;
}

void music_man_cleanup(void)
{
	if (!initialized) {
		return;
	}

	music_man_unload();
	CloseAudioDevice();
	initialized = false;
}

void music_man_unload(void)
{
	int i;

	for (i = 0; sound_definitions[i].name; i++) {
		if (sound_definitions[i].loaded) {
			sound_definitions[i].loaded = false;
			if (IsSoundPlaying(sound_definitions[i].sound)) {
				StopSound(sound_definitions[i].sound);
			}
			UnloadSound(sound_definitions[i].sound);
		}
	}

	for (i = 0; music_definitions[i].name; i++) {
		if (music_definitions[i].loaded) {
			music_definitions[i].loaded = false;
			if (IsMusicPlaying(music_definitions[i].music)) {
				StopMusicStream(music_definitions[i].music);
			}
			UnloadMusicStream(music_definitions[i].music);
		}
	}
}

void music_man_update(void)
{
	int i;
	for (i = 0; music_definitions[i].name; i++) {
		if (music_definitions[i].loaded) {
			UpdateMusicStream(music_definitions[i].music);
		}
	}
}

void music_man_set_volume(float volume)
{
	SetMasterVolume(volume);
	master_volume = volume;
}

float music_man_get_volume(void)
{
	return master_volume;
}

void music_man_load_sound(const char *name)
{
	int i;
	assert(name);
	for (i = 0; sound_definitions[i].name; i++) {
		if (streq((char *)sound_definitions[i].name, (char *)name)) {
			// don't reload
			if (sound_definitions[i].loaded) {
				return;
			}
			sound_definitions[i].sound = LoadSound(sound_definitions[i].filename);
			sound_definitions[i].loaded =  true;
			return;
		}
	}
	msg_assert(0, "Sound %s not found", name);
}

void music_man_play_sound(const char *name)
{
	int i;
	assert(name);
	for (i = 0; sound_definitions[i].name; i++) {
		if (streq((char *)sound_definitions[i].name, (char *)name)) {
			msg_assert(sound_definitions[i].loaded, "Sound %s not loaded", sound_definitions[i].filename);
			PlaySound(sound_definitions[i].sound);
			return;
		}
	}
	msg_assert(0, "Sound %s not found", name);
}

void music_man_load_music(const char *name)
{
	int i;
	assert(name);
	for (i = 0; music_definitions[i].name; i++) {
		if (streq((char *)music_definitions[i].name, (char *)name)) {
			msg_assert(!music_definitions[i].loaded, "Sound %s already loaded", music_definitions[i].filename);
			music_definitions[i].music = LoadMusicStream(music_definitions[i].filename);
			music_definitions[i].loaded =  true;
			SetMusicLoopCount(music_definitions[i].music, MUSIC_REPEAT_COUNT);
			PlayMusicStream(music_definitions[i].music);
			return;
		}
	}
	msg_assert(0, "Music %s not found", name);
}
