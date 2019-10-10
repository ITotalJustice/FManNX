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
#include "reboot_payload.h"

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

        draw_menu("temp");
        print_dir(number_of_files, list_move, cursor, files);

        if (option == FILE_OPTION) draw_file_options(file_option_cursor);

        // scroll up...
        if (kDown & KEY_UP)
        {
            switch (option)
            {
                case DIR_BROWSER:
                    cursor = move_cursor_up(cursor, number_of_files);
                    list_move = list_move_up(list_move, cursor, number_of_files, LIST_MAX);
                    break;

                case FILE_OPTION:
                    file_option_cursor = move_cursor_up(file_option_cursor, 6);
                break;
            }
        }

        // scroll down...
        if (kDown & KEY_DOWN)
        {
            switch (option)
            {
                case DIR_BROWSER:
                    cursor = move_cursor_down(cursor, number_of_files);
                    list_move = list_move_down(list_move, cursor, LIST_MAX);
                    break;

                case FILE_OPTION:
                    file_option_cursor = move_cursor_down(file_option_cursor, 6);
                break;
            }
        }

        if (kDown & KEY_A)
        {
            if (option == DIR_BROWSER)
            {
                if (files[cursor].dir == YES)
                {
                    file_stuff(&cursor, &number_of_files);
                    list_move = 0;
                }

                else if (!strcmp(files[cursor].ext, PAYLOAD))
                    reboot_payload(files[cursor].file_name);

                //else if (!strcmp(files[cursor].ext, TXT_FILE))

                //else if (!strcmp(files[cursor].ext, INI_FILE))

                //else if (!strcmp(files[cursor].ext, NRO_FILE))

                //else if (!strcmp(files[cursor].ext, NSP_FILE))

                //else if (!strcmp(files[cursor].ext, XCI_FILE))
                
                else if (!strcmp(files[cursor].ext, MP3_FILE))
                    SDL_PlayMusic(files[cursor].file_name);

                else if (!strcmp(files[cursor].ext, OGG_FILE))
                    SDL_PlayMusic(files[cursor].file_name);

                //else if (!strcmp(files[cursor].ext, WAV_FILE))

                //else if (!strcmp(files[cursor].ext, FLAC_FILE))
            }

            else if (option == FILE_OPTION)
            {
                switch (file_option_cursor)
                {
                    case 0:
                        // edit
                        break;

                    case 1:
                        //cut
                        break;

                    case 2:
                        //copy
                        break;

                    case 3:
                        //move
                        break;

                    case 4:
                        //delete
                        break;

                    case 5:
                        //rename
                        break;
                }
            }
        }

        // jump back to root dir.
        if (kDown & KEY_B)
        {
            if (option == DIR_BROWSER)
            {
                if (!strcmp(files[0].file_name, ".."))
                {
                    cursor = 0;
                    list_move = 0;
                    file_stuff(&cursor, &number_of_files);
                }
            }

            else option = DIR_BROWSER;
        }

        // file option menu.
        if (kDown & KEY_X)
        {
            if (option == DIR_BROWSER) option = FILE_OPTION;
            else option = DIR_BROWSER;
        }

        // exit.
        if (kDown & KEY_PLUS) break;

        // render screen.
        SDL_UpdateRenderer();
    }
    
    // clean then exit...
    free(files);
    free(folder_info);
    app_exit();
    return 0;
}