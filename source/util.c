#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "util.h"


int move_cursor_up(int cursor, int cursor_max)
{
    if (cursor == 0)
        cursor = cursor_max - 1;
    else
        cursor--;
    
    return cursor;
}

int move_cursor_down(int cursor, int cursor_max)
{
    if (cursor == cursor_max - 1)
        cursor = 0;
    else
        cursor++;

    return cursor;
}

int list_move_up(int list_move, int cursor, int number_of_files, int list_max)
{
    if (cursor == list_move - 1)
        list_move--;
    else if (cursor == number_of_files - 1 && list_max <= number_of_files)
        list_move = cursor - (list_max - 1);

    return list_move;
}

int list_move_down(int list_move, int cursor, int list_max)
{
    if (cursor == (list_max - 1) + (list_move + 1))
        list_move++;
    else if (cursor == 0)
        list_move = 0;

    return list_move;
}

const char *get_filename_ext(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

bool sort_num_string(const char *str1, const char *str2)
{
   for (int i = 0; str1[i] && str2[i]; i++)
        if (str1[i] < str2[i])
            return 1;
    return 0;
}

bool is_dir(const char *folder_to_check)
{
    DIR *dir = opendir(folder_to_check);
    if (!dir)
        return false;

    closedir(dir);
    return true;
}

bool check_if_dir_exists(const char *directory)
{
    DIR *dir = opendir(directory);
    if (!dir)
        return false;

    closedir(dir);
    return true;
}

void file_exists(char **full_path, const char *src)
{
    for (u_int8_t i = 1; i < 256; i++)
    {
        char *buffer;
        if (!asiprintf(&buffer, "%s%c%d%c", src, '(', i, ')'))
            return;

        FILE *f = fopen(buffer, "r");
        if (!f)
        {
            fclose(f);
            full_path = &buffer;
            return;
        }
        free(buffer);
    }
}

size_t scan_dir(const char *directory)
{
    size_t number_of_files = 0;
    DIR *dir = opendir(directory);
    struct dirent *de;

    if (!dir)
        return number_of_files;

    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;
        number_of_files++;
    }

    closedir(dir);
    return number_of_files;
}

size_t scan_dir_recursive(const char *directory)
{
    size_t num = 0;
    DIR *dir = opendir(directory);
    struct dirent *de;

    if (!dir)
        return num;
        
    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char *full_path;
        if (!asiprintf(&full_path, "%s/%s", directory, de->d_name))
            return num;

        if (is_dir(de->d_name))
            num += get_foldersize(de->d_name);

        num++;
        free(full_path);
    }
    closedir(dir);
    return num;
}

size_t get_filesize(const char *file)
{
    size_t size = 0;
    FILE *f = fopen(file, "r");
    if (!f)
        return size;

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fclose(f);
    return size;
}

size_t get_foldersize(const char *directory)
{
    size_t size = 0;
    DIR *dir = opendir(directory);
    struct dirent *de;
    if (!dir)
        return size;

    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char *full_path;
        if (!asiprintf(&full_path, "%s/%s", directory, de->d_name))
            return size;

        if (is_dir(full_path))
            size += get_foldersize(full_path);
        else
            size += get_filesize(full_path);

        free(full_path);
    }
    return size;
}

void create_dir(const char *dir)
{
    mkdir(dir, 0777);
}

void delete_dir(const char *directory)
{
    DIR *dir = opendir(directory);
    struct dirent *de;

    if (!dir)
        return;

    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char *full_path;
        if (!asiprintf(&full_path, "%s/%s", directory, de->d_name))
            return;

        if (is_dir(full_path))
            delete_dir(full_path);
        else
            remove(full_path);

        free(full_path);
    }
    closedir(dir);
    rmdir(directory);
}

void copy_file(const char *src, char *dest)
{
    FILE *srcfile = fopen(src, "rb");
    FILE *newfile;

    // check if the file already exists
    FILE *f = fopen(dest, "r");
    if (f)
    {
        fclose(f);
        char *full_path;
        file_exists(&full_path, dest);
        newfile = fopen(full_path, "wb");
        free(full_path);
    }
    else
        newfile = fopen(dest, "wb");

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

        // check if the file is a directory.
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
    FILE *f = fopen(dest, "r");
    if (!f)
        rename(src, dest);
    else
    {
        fclose(f);
        char *full_path;
        file_exists(&full_path, dest);
        rename(src, full_path);
        free(full_path);
    }
}

void move_folder(const char *src, char *dest)
{
    DIR *dir = opendir(src);
    struct dirent *de;

    create_dir(dest);

    while ((de = readdir(dir)))
    {
        char *full_path;
        if (!asiprintf(&full_path, "%s/%s", src, de->d_name))
            return;

        // check if the file is a directory.
        if (de->d_name[strlen(de->d_name)] == '/')
            create_dir(full_path);
        else
            move_file(de->d_name, full_path);

        free(full_path);
    }
    closedir(dir);
}