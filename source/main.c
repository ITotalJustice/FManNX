#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <switch.h>

#include "dir.h"
#include "sdl.h"
#include "util.h"

//#define DEBUG

void app_init()
{
    #ifdef DEBUG
    socketInitializeDefault();
    nxlinkStdio();
    #endif

    plInitialize();
    romfsInit();
    sdlInit();
    romfsExit();
}

void app_exit()
{
    #ifdef DEBUG
    socketExit();
    #endif
    plExit();
    sdlExit();
}

int main(int argc, char **argv)
{
    // init stuff
    app_init();
    chdir(ROOT);

    int cursor = 0;
    int list_move = 0;
    int number_of_files = scan_dir(ROOT);
    
    create_node(number_of_files, ROOT);

    // draw menu and print directory
    draw_menu(ROOT);
    print_dir(number_of_files, list_move, cursor, files);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        // scroll up...
        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, number_of_files -1);
            if (cursor == list_move-1) list_move--;
            else if (cursor == number_of_files-1) list_move = cursor - (LIST_MAX - 1);

            draw_menu("temp");
            print_dir(number_of_files, list_move, cursor, files);
        }

        // scroll down...
        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, number_of_files -1);
            if (cursor == (LIST_MAX - 1) + list_move + 1) list_move++;
            else if (cursor == 0) list_move = 0;

            draw_menu("temp");
            print_dir(number_of_files, list_move, cursor, files);
        }

        if (kDown & KEY_A)
        {
            if (!file_stuff(&cursor, &number_of_files))
            {
                list_move = 0;
                draw_menu("temp");
                print_dir(number_of_files, list_move, cursor, files);
            }
        }

        // jump back to root dir.
        if (kDown & KEY_B)
        {
            if (!strcmp(files[0].file_name, ".."))
            {
                cursor = 0;
                if (!file_stuff(&cursor, &number_of_files))
                {
                    list_move = 0;
                    draw_menu("temp");
                    print_dir(number_of_files, list_move, cursor, files);
                }
            }
        }

        if (kDown & KEY_PLUS) break;
    }
    
    // clean then exit...
    free_node(files);
    app_exit();
    return 0;
}