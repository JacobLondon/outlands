#ifndef OUTLANDS_GLOBALS_H
#define OUTLANDS_GLOBALS_H

#include <stdbool.h>

#define DEFAULT_TARGET_FPS 60
#define DEFAULT_SCREEN_WIDTH 1600
#define DEFAULT_SCREEN_HEIGHT 900

#define GRID_PIX_WIDTH 40
#define GRID_PIX_HEIGHT 40
#define GRIDS_WIDE (DEFAULT_SCREEN_WIDTH / GRID_PIX_WIDTH)
#define GRIDS_TALL (DEFAULT_SCREEN_HEIGHT / GRID_PIX_HEIGHT)

#define ASSET_DIRECTORY "assets"
#define AUDIO_DIRECTORY "audio"
#define DEFAULT_VOLUME 0.5

#define NETWORK_HOST "127.0.0.1"
#define NETWORK_PORT 7000
#define CLIENTS_MAX 8

extern unsigned char global_id;
extern bool global_connect_to_network;


#endif // OUTLANDS_GLOBALS_H
