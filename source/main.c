#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <switch.h>

#include "dir.h"
#include "sdl.h"
#include "util.h"
#include "gfx_util.h"

//#define DEBUG
#define DIR_BROWSER     0
#define FILE_OPTION     1
#define MUSIC_PLAYER    2


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

    int option = 0;

    int cursor = 0;
    int file_option_cursor = 0;
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
            switch (option)
            {
                case DIR_BROWSER:
                    cursor = move_cursor_up(cursor, number_of_files -1);
                    list_move = list_move_up(list_move, cursor, number_of_files, LIST_MAX);

                    draw_menu("temp");
                    print_dir(number_of_files, list_move, cursor, files);
                    break;

                case FILE_OPTION:
                    file_option_cursor = move_cursor_up(file_option_cursor, number_of_files -1);

                    draw_menu("temp");
                    print_dir(number_of_files, list_move, cursor, files);
                break;
            }
        }

        // scroll down...
        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, number_of_files -1);
            list_move = list_move_down(list_move, cursor, LIST_MAX);

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

        if (kDown & KEY_X)
        {
            draw_menu("temp");
            print_dir(number_of_files, list_move, cursor, files);

            SDL_DrawShape(dark_grey, 350, 100, 250, 350);

            SDL_DrawText(fntSmall, 375, 120, white, "%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s", "edit", "cut", "copy", "move", "delete", "rename");
        }

        if (kDown & KEY_PLUS) break;

        SDL_UpdateRenderer();
    }
    
    // clean then exit...
    free_node(files);
    app_exit();
    return 0;
}