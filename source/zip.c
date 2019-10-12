#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <minizip/unzip.h>
#include <switch.h>

#include "sdl.h"
#include "dir.h"
#include "util.h"
#include "gfx_util.h"

#define WRITEBUFFERSIZE 500000 // 500KB
#define MAXFILENAME     256

typedef struct node
{
    char name[512];
    uint16_t size;
    char folder[2];
}node;

unzFile zfile;
unz_global_info gi;
unz_file_info file_info;
unz_file_pos zpos;
node *zip_node;


int is_folder(char *file)
{
    // check if the string ends with a /, if so, then its a directory.
    if ((file[strlen(file) - 1]) == '/') return YES;
    return NO;
}

void print_zip_contents(int cursor, int list_move)
{
    int x = 150;

    for (int i = 0, j = list_move, nl = 110; i < gi.number_entry && i < LIST_MAX; i++, j++, nl += 60)
    {   
        if (j == cursor)
            SDL_DrawText(fntSmall, x, nl, n_cyan, "> %s", zip_node[j].name);
        else
            SDL_DrawText(fntSmall, x, nl, n_white, "%s", zip_node[j].name);
    }
}

void free_zip_node()
{
    if (zip_node != NULL)
    {
        free(zip_node);
        zip_node = NULL;
    }
}

void close_zip()
{
    free_zip_node();
    unzClose(zfile);
}

void unzip()
{
    char filename_inzip[MAXFILENAME];

    unzOpenCurrentFile(zfile);
    unzGetCurrentFileInfo(zfile, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

    if (is_folder(filename_inzip) == YES)
    {
        if (check_if_dir_exists(filename_inzip) == NO)
            mkdir(filename_inzip, 0777);
    }

    else
    {
        const char *write_filename = filename_inzip;
        char *buf = malloc(WRITEBUFFERSIZE);

        FILE *outfile = fopen(write_filename, "wb");

        for (uint16_t j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE); j > 0; j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE))
            fwrite(buf, 1, WRITEBUFFERSIZE, outfile);

        fclose(outfile);
        free(buf);
    }
}

void unzip_all(const char *file)
{
    zfile = unzOpen(file);
    unzGetGlobalInfo(zfile, &gi);

    for (uint16_t i = 0; i < gi.number_entry; i++)
    {
        unzip();

        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
    }

    close_zip();
}

void set_position_inzip(int cursor)
{
    unzGoToFirstFile(zfile);
    for (uint16_t i = 0; i < cursor; i++)
        unzGoToNextFile(zfile);
}

void open_zip(const char *file)
{
    zfile = unzOpen(file);
    unzGetGlobalInfo(zfile, &gi);

    zip_node = malloc(sizeof(*zip_node) * gi.number_entry);

    for (uint8_t i = 0; i < gi.number_entry; i++)
    {
        char filename_inzip[MAXFILENAME];

        unzOpenCurrentFile(zfile);
        unzGetCurrentFileInfo(zfile, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

        strcpy(zip_node[i].name, filename_inzip);
        zip_node[i].size = file_info.uncompressed_size;

        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
    }
}

void select_file_inzip(int cursor)
{

}

void compress_file(const char *file)
{

}

void compress_folder(const char *folder)
{

}

int unzip_menu(char *pwd, const char *file)
{
    uint16_t cursor = 0;
    uint8_t list_move = 0;

    char full_path[BUFFER_MAX + BUFFER_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", pwd, file);

    open_zip(file);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        draw_menu(full_path);
        print_zip_contents(cursor, list_move);

        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, gi.number_entry);
            list_move = list_move_up(list_move, cursor, gi.number_entry, LIST_MAX);
        }

        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, gi.number_entry);
            list_move = list_move_down(list_move, cursor, LIST_MAX);
        }

        if (kDown & KEY_A)
            select_file_inzip(cursor);

        // exit menu.
        if (kDown & KEY_B) break;

        // exit app.
        if (kDown & KEY_PLUS)
        {
            close_zip();
            return APP_EXIT;
        }

        SDL_UpdateRenderer();
    }

    close_zip();
    return 0;
}