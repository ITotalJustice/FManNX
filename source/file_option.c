#include <switch.h>

#include "sdl.h"
#include "dir.h"
#include "util.h"
#include "gfx_util.h"

#define OPTION_LIST_MAX 7

#define EDIT        0
#define COPY        1
#define PASTE       2
#define MOVE        3
#define MOVE_HERE   4
#define DELETE      5
#define RENAME      6

// GLOBALS.
static char file_location[FILENAME_MAX];
static char file_name[BUFFER_MAX];

void clear_buffers()
{
    memset(file_location, '\0', sizeof(file_location));
    memset(file_name, '\0', sizeof(file_name));
}

void store_strings()
{
}

void print_file_options(int cursor)
{
    char *options[OPTION_LIST_MAX] = 
    {
        "edit",
        "copy",
        "paste",
        "move",
        "move here",
        "delete",
        "rename"
    };


    SDL_DrawShape(n_grey, 350, 100, 250, 400);

    for (uint16_t i = 0, nl = 120; i < OPTION_LIST_MAX; i++, nl += 55)
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

        case COPY:
            clear_buffers();
            strcpy(file_name, file);
            snprintf(file_location, sizeof(file_location), "%s/%s", pwd, file);
            break;

        case PASTE:
            if (is_dir(file_location))
                copy_folder(file_location, file_name);
            else
                copy_file(file_location, file_name);
            clear_buffers();
            free_nodes();
            create_node(pwd);
            break;

        case MOVE:
            clear_buffers();
            strcpy(file_name, file);
            snprintf(file_location, sizeof(file_location), "%s/%s", pwd, file);
            break;

        case MOVE_HERE:
            if (is_dir(file_location))
                move_folder(file_location, file_name);
            else
                move_file(file_location, file_name);
            clear_buffers();
            free_nodes();
            create_node(pwd);
            break;

        case DELETE:
            if (is_dir(file))
                delete_dir(file);
            else
                remove(file);
            clear_buffers();
            free_nodes();
            create_node(pwd);
            break;

        case RENAME:
            printf("rename\n");
            char buffer[256];
            keyboard(buffer);
            
            rename(file, buffer);
            clear_buffers();
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