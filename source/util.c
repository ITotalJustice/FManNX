#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "util.h"


int move_cursor_up(int cursor, int cursor_max)
{
    if (cursor == 0) cursor = cursor_max - 1;
    else cursor--;
    return cursor;
}

int move_cursor_down(int cursor, int cursor_max)
{
    if (cursor == cursor_max - 1) cursor = 0;
    else cursor++;
    return cursor;
}

int list_move_up(int list_move, int cursor, int number_of_files, int list_max)
{
    if (cursor == list_move - 1) list_move--;
    else if (cursor == number_of_files - 1) list_move = cursor - (list_max - 1);
    return list_move;
}

int list_move_down(int list_move, int cursor, int list_max)
{
    if (cursor == (list_max - 1) + list_move + 1) list_move++;
    else if (cursor == 0) list_move = 0;
    return list_move;
}

const char *get_filename_ext(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

int check_if_dir_exists(const char *folder)
{
    DIR *dir = opendir(folder);
    if (!dir) return NO;

    closedir(dir);
    return YES;
}

int file_exists(char *newfile_buffer, const char *src)
{
    for (u_int8_t i = 1; i < 256; i++)
    {
        char buffer[520];
        snprintf(buffer, sizeof(buffer), "%s%s%d%s", src, "(", i, ")");

        FILE *fp = fopen(buffer, "r");
        if (!fp)
        {
            sprintf(newfile_buffer, buffer);
            fclose(fp);
            return 0;
        }
    }
    return 1;
}

void create_dir(const char *dir)
{
    mkdir(dir, 0777);
}

void copy_file(const char *src, char *dest)
{
    FILE *srcfile = fopen(src, "rb");
    FILE *newfile;

    // check if the file already exists
    FILE *fp = fopen(dest, "r");
    if (fp)
    {
        fclose(fp);
        char newfile_buffer[512];
        if (!file_exists(newfile_buffer, dest))
            newfile = fopen(newfile_buffer, "wb");
    }

    else newfile = fopen(dest, "wb");

    if (srcfile && newfile)
    {
        char buffer[10000]; // 10kb per write, which is fast
        size_t bytes;       // size of the file to write (10kb or filesize max)

        while (0 < (bytes = fread(buffer, 1, sizeof(buffer), srcfile)))
            fwrite(buffer, 1, bytes, newfile);
    }
    fclose(srcfile);
    fclose(newfile);
}

void copy_folder(const char *src, char *dest)
{
    DIR *dir = opendir(src);
    struct dirent *de;

    create_dir(dest);

    while ((de = readdir(dir)))
    {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "%s/%s", dest, de->d_name);

        // check if the file is a folder.
        if (de->d_name[strlen(de->d_name)] == '/')
            create_dir(buffer);
        else
            copy_file(de->d_name, buffer);
    }
    
    closedir(dir);
}

void move_file(const char *src, char *dest)
{
    // check if the file already exists.
    FILE *fp = fopen(src, "r");
    if (fp)
    {
        fclose(fp);
        char newfile_buffer[512];
        if (!file_exists(newfile_buffer, dest))
            rename(src, newfile_buffer);
    }

    else rename(src, dest);
}

void move_folder(const char *src, char *dest)
{
    DIR *dir = opendir(src);
    struct dirent *de;

    create_dir(dest);

    while ((de = readdir(dir)))
    {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "%s/%s", dest, de->d_name);

        // check if the file is a folder.
        if (de->d_name[strlen(de->d_name)] == '/')
            create_dir(buffer);
        else
            move_file(de->d_name, buffer);
    }
    
    closedir(dir);
}