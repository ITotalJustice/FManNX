#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <switch.h>

#include "dir.h"
#include "sdl.h"
#include "sdl_music.h"

#define DEBUG


void app_init()
{
    #ifdef DEBUG
    socketInitializeDefault();
    nxlinkStdio();
    #endif

    plInitialize();
    splInitialize();
    romfsInit();
    sdlInit();
    sdl_music_init();
    romfsExit();
}

void app_exit()
{
    #ifdef DEBUG
    socketExit();
    #endif
    plExit();
    splExit();
    sdl_music_exit();
    sdlExit();
}

int main(int argc, char **argv)
{
    // init stuff.
    app_init();
    chdir(ROOT);

    // the main menu loop.
    directory_menu();
    
    // clean then exit...
    app_exit();
    return 0;
}