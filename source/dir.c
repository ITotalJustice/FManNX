#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <switch.h>

#include "dir.h"
#include "util.h"
#include "sdl.h"


int is_dir(char *folder_to_check)
{
    DIR *dir = opendir(folder_to_check);
    if (dir)
    {
        closedir(dir);
        return YES;
    }
    
    return NO;  
}

int scan_dir(char *directory)
{
    int files_found = 0;

    // check if the dir is root, if not, +1 in order to display the ".."
    char buffer[256];
    if (strcmp(getcwd(buffer, sizeof(buffer)), ROOT)) files_found++;

    DIR *dir = opendir(directory);
    struct dirent *de;

    if (dir)
    {
        while ((de = readdir(dir)))
            files_found++;
        closedir(dir);
    }
    
    return files_found;
}

void draw_file_icon(char *file, int x, int y, int w, int h)
{
    if (!strcmp(file, PAYLOAD))
    {
        folder_info->total_payloads++;
        SDL_DrawShape(brown, x, y, w, h);
    }

    else if (!strcmp(file, TXT_FILE))
    {
        folder_info->total_txt++;
        SDL_DrawShape(green, x, y, w, h);
    }

    else if (!strcmp(file, INI_FILE))
    {
        folder_info->total_ini++;
        SDL_DrawShape(orange, x, y, w, h);
    }

    else if (!strcmp(file, NRO_FILE))
    {
        folder_info->total_nro++;
        SDL_DrawShape(yellow, x, y, w, h);
    }

    else if (!strcmp(file, NSP_FILE))
    {
        folder_info->total_nsp++;
        SDL_DrawShape(pink, x, y, w, h);
    }

    else if (!strcmp(file, XCI_FILE))
    {
        folder_info->total_xci++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcmp(file, MP3_FILE))
    {
        folder_info->total_mp3++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcmp(file, OGG_FILE))
    {
        folder_info->total_ogg++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcmp(file, WAV_FILE))
    {
        folder_info->total_wav++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcmp(file, FLAC_FILE))
    {
        folder_info->total_flac++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else SDL_DrawShape(white, x, y, w, h); 
}

void print_dir(int number_of_files, int list_move, int cursor, node *files)
{
    int x = 150;
    int shape_x = 100;

    for (int i = 0, j = list_move, nl = 100; i < number_of_files && i < LIST_MAX; i++, j++, nl += 60)
    {
        if (files[j].dir == YES)
        {
            SDL_DrawShape(purple, shape_x, nl, 20, 20);
            folder_info->total_folders++;
        }

        else draw_file_icon(files[j].ext, shape_x, nl, 20, 20);
        
        if (j == cursor)
            SDL_DrawText(fntSmall, x, nl, grey, "> %s", files[j].file_name);
        else
            SDL_DrawText(fntSmall, x, nl, white, "%s", files[j].file_name);
    }
}

void create_node(int number_of_files, char *folder_location)
{
    DIR *dir = opendir(folder_location);
    struct dirent *de;
    
    if (dir)
    {
        files = malloc(number_of_files * (sizeof(*files) + sizeof(*files)));
        folder_info = malloc(sizeof(*folder_location));

        folder_info->total = number_of_files;

        int n = 0;
        char buffer[256];
        if (strcmp(getcwd(buffer, sizeof(buffer)), ROOT))
        {
            strcpy(files[n].file_name, "..");
            files[n].dir = 1;
            n++;
        }

        // store all the information of each file in the directory.
        for ( ; (de = readdir(dir)); n++)
        {
            strcpy(files[n].file_name, de->d_name);

            if (is_dir(de->d_name)) files[n].dir = YES;

            else
            {
                files[n].dir = NO;
                strcpy(files[n].ext, get_filename_ext(de->d_name));
            }
        }
        closedir(dir);
    }
}

void file_stuff(int *cursor, int *number_of_files)
{
    // look at this mess...
    // basically concatenate 2 strings, with a '/' in the middle. Extremely messy.
    char buffer[1024];
    char full_path[256 + sizeof(buffer)];
    snprintf(full_path, sizeof(full_path), "%s/%s", getcwd(buffer, sizeof(buffer)), files[*cursor].file_name);

    chdir(full_path);
    *cursor = 0;
    *number_of_files = scan_dir(full_path);

    // freeeeeeeee...
    free(files);
    free(folder_info);
    //always set to NULL after free.
    files = NULL;
    folder_info = NULL;
    // create new node with the size of number_of_files.
    create_node(*number_of_files, full_path);
}