#include <switch.h>

#include "sdl.h"
#include "dir.h"
#include "util.h"
#include "gfx_util.h"

#define OPTION_LIST_MAX 6

#define EDIT    0
#define CUT     1
#define COPY    2
#define MOVE    3
#define DELETE  4
#define RENAME  5

// GLOBALS.
static char file_temp[FILENAME_MAX];

void store_strings()
{
}

void print_file_options(int cursor)
{
    char *options[OPTION_LIST_MAX] = 
    {
        "edit",
        "cut",
        "copy",
        "move",
        "delete",
        "rename"
    };


    SDL_DrawShape(n_grey, 350, 100, 250, 350);

    for (uint16_t i = 0, nl = 120; i < 6; i++, nl += 55)
    {
        if (cursor == i)
            SDL_DrawText(fntSmall, 375, nl, grey, options[i]);
        else
            SDL_DrawText(fntSmall, 375, nl, n_white, options[i]);
    }
}

void select_option(int cursor, const char *pwd, const char *file)
{
    switch (cursor)
    {
        case EDIT:
            break;

        case CUT:
            memset(file_temp, '\0', sizeof(file_temp));
            snprintf(file_temp, sizeof(file_temp), "%s/%s", pwd, file);
            break;

        case COPY:
            memset(file_temp, '\0', sizeof(file_temp));
            snprintf(file_temp, sizeof(file_temp), "%s/%s", pwd, file);
            break;

        case MOVE:
            memset(file_temp, '\0', sizeof(file_temp));
            snprintf(file_temp, sizeof(file_temp), "%s/%s", pwd, file);
            break;

        case DELETE:
            if (is_dir(file))
                delete_dir(file);

            else
                remove(file);

            free_nodes();
            create_node(pwd);
            break;

        case RENAME:
            printf("rename\n");
            char buffer[256];
            keyboard(buffer);
            
            rename(file, buffer);
            free_nodes();
            create_node(pwd);
            break;
    }
}

int file_options_menu(const char *pwd, const char *file, ...)
{
    uint8_t cursor = 0;

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        // draw graphics
        draw_menu(pwd);
        print_dir();
        print_file_options(cursor);

        if (kDown & KEY_UP)
            cursor = move_cursor_up(cursor, OPTION_LIST_MAX);

        if (kDown & KEY_DOWN)
            cursor = move_cursor_down(cursor, OPTION_LIST_MAX);

        if (kDown & KEY_A)
        {
            select_option(cursor, pwd, file);
            break;
        }

        // exit out of menu.
        if (kDown & KEY_B || kDown & KEY_X)
            break;

        // exit app.
        if (kDown & KEY_PLUS)
            return APP_EXIT;

        SDL_UpdateRenderer();
    }

    return 0;
}