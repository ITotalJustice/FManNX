#include <switch.h>

#include "sdl.h"
#include "dir.h"
#include "util.h"
#include "gfx_util.h"


void print_file_options(int cursor)
{
    char *options[] = 
    {
        "edit", 
        "cut", 
        "copy", 
        "move", 
        "delete", 
        "rename"
    };

    SDL_DrawShape(n_grey, 350, 100, 250, 350);

    for (u_int8_t i = 0, nl = 120; i < 6; i++, nl += 55)
    {
        if (cursor == i)
            SDL_DrawText(fntSmall, 375, nl, grey, options[i]);
        else
            SDL_DrawText(fntSmall, 375, nl, n_white, options[i]);
    }
}

void select_option(int cursor)
{
    switch (cursor)
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

void file_options_menu()
{
    uint8_t cursor = 0;

    while(appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        draw_menu("temp");
        print_dir();
        print_file_options(cursor);

        if (kDown & KEY_UP)
            cursor = move_cursor_up(cursor, 6);

        if (kDown & KEY_DOWN)
            cursor = move_cursor_down(cursor, 6);

        if (kDown & KEY_A)
            select_option(cursor);

        // exit out of menu.
        if (kDown & KEY_B || kDown & KEY_X)
            break;

        SDL_UpdateRenderer();
    }
}