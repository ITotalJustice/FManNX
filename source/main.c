#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <switch.h>

#include "dir.h"
#include "sdl.h"
#include "util.h"
#include "sdl_music.h"
#include "gfx_util.h"
#include "zip.h"
#include "file_option.h"

//#define DEBUG


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

    free_nodes();
}

int main(int argc, char **argv)
{
    // init stuff
    app_init();
    chdir(ROOT);

    scan_dir(ROOT);
    
    create_node(ROOT);
    
    // clean then exit...
    app_exit();
    return 0;
}