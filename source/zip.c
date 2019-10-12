#include <stdio.h>
#include <minizip/unzip.h>
#include <string.h>
#include <dirent.h>
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

void print_list(int cursor, int list_move)
{

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

    for (uint8_t i = 0; i < gi.number_entry; i++)
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
    for (uint8_t i = 0; i < cursor; i++)
        unzGoToNextFile(zfile);
}

void open_zip(const char *file)
{
    free_zip_node();

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

void unzip_menu(const char *file)
{
    uint8_t cursor = 0;
    uint8_t list_move = 0;

    open_zip(file);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        draw_menu("temp");
        print_list(cursor, list_move);

        if (kDown & KEY_UP)
            cursor = move_cursor_up(cursor, gi.number_entry);

        if (kDown & KEY_DOWN)
            cursor = move_cursor_down(cursor, gi.number_entry);

        if (kDown & KEY_A)
            select_file_inzip(cursor);

        // exit.
        if (kDown & KEY_B) break;

        SDL_UpdateRenderer();
    }

    close_zip();
    free_zip_node();
}