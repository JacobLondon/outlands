# Outlands
Consists of two programs: Outlands (lands.exe) and Ship Maker (maker.exe)

# Building
## Windows
```
make
make maker
```
## Other
Currently no plans for other OSes due to building difficulties.

# Dependencies
1. Go to [Raylib Releases](https://github.com/raysan5/raylib/releases) and download / install `raylib_installer_v300.tcc.exe`
2. Go to [Make for Windows](http://gnuwin32.sourceforge.net/packages/make.htm) and download the `Binaries Zip`
    * Unzip the file anywhere on your computer. Copy the path to the `bin` directory in the unzipped directory
    * Press the `Windows Key` and type `environment variables` + `Enter`
    * Click on `environment variables...`
    * On the bottom half, look for `Path`, click on it, then click `Edit...`
    * Click `New` and press `ctrl+v` to paste your path from `bin`
    * Click `Ok` then `Ok`
    * Open a new `cmd` in this project and type `make` to generate the executable
