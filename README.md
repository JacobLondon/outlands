# Outlands
Personal project consisting of two programs: Outlands outlands(.exe) and Ship Editor editor(.exe). As I definitely don't own any of the assets used, they will not be available for download.

![Screenshot](https://cdn.discordapp.com/attachments/174372275133480960/748985374596005948/unknown.png)

# Building
```
make
make rule_editor
```

# Animation Library
Built in animation library, at some point I might make it more standalone than it is now. It will only allocate memory upon initialization and works as follows:
```C
// Prototype the initializers
static void init_cb_background1(scene *self);
static void init_cb_background2(scene *self);
static void init_cb_midground1(scene *self);
static void init_cb_midground1(scene *self);
static void init_cb_foreground1(scene *self);
static void init_cb_foreground2(scene *self);

// Create a scene_definition array
static scene_definition defs[] = {
    // name, max # of scene objects, init callback
    { "Background1", 1, init_cb_background1 },
    { "Background2", 1, init_cb_background2 },
    { "Midground1", 1, init_cb_midground1 },
    { "Midground2", 1, init_cb_midground1 },
    { "Foreground1", 1, init_cb_foreground1 },
    { "Foreground2", 1, init_cb_foreground2 },
};

// The scenes can be loaded into sets (assuming each scene has transparency)
// and can be loaded on top of each other, from left to right, back to front
// respectively. These sets can be loaded with scene_man_load_... functions.

static scene_set set_defs[] = {
    { "Cool Scene 1", { "Background1", "Midground1", "Foreground2", NULL } },
    { "Cool Scene 2", { "Background2", "Midground1", "Foreground1", NULL } },
    { "Cool Scene 3", { "Background1", "Midground2", "Foreground1", NULL } },
    { "Cool Scene 4", { "Background2", "Midground1", "Foreground2", NULL } },
}

// For details/examples on how to create the callbacks, see scene_man.c
```

# Todo
* VS integration (at some point)
* Input tasks for sending / tracking every necessary data point 
* Exponential movement / scaling

# Windows Setup
1. Go to [Raylib Releases](https://github.com/raysan5/raylib/releases) and download / install `raylib_installer_v300.tcc.exe`
2. Go to [Make for Windows](http://gnuwin32.sourceforge.net/packages/make.htm) and download the `Binaries Zip`
3. Unzip the file anywhere on your computer. Copy the path to the `bin` directory in the unzipped directory
4. Press the `Windows Key` and type `environment variables` + `Enter`
5. Click on `environment variables...`
6. On the bottom half, look for `Path`, click on it, then click `Edit...`
7. Click `New` and press `ctrl+v` to paste your path from `bin`
8. Click `Ok` then `Ok`
9. Open a new `cmd` in this project and type `make` to generate the executable

# Linux (Ubuntu) Setup
```bash
sudo apt update
# requirements
sudo apt install make gcc git
# dependencies
cat requirements.txt | xargs sudo apt install -y
# cd somewhere meaningful...
git clone https://github.com/raysan5/raylib
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED
sudo ln libraylib.so.3.0.0 /usr/lib/libraylib.so
sudo ldconfig
```
