#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <switch.h>

#include "dir.h"
#include "sdl.h"
#include "util.h"
#include "sdl_music.h"
#include "gfx_util.h"
#include "zip.h"
#include "file_option.h"
#include "reboot_payload.h"


typedef struct node
{
    char file_name[BUFFER_MAX];
    char ext[5];
    u_int8_t dir;
    u_int file_size;
} node;

typedef struct folder_node
{
    uint16_t total;
    uint16_t total_folders;
    uint16_t total_files;

    uint8_t total_payloads;
    uint8_t total_txt;
    uint8_t total_ini;

    uint8_t total_nro;
    uint8_t total_nsp;
    uint8_t total_xci;

    uint8_t total_mp3;
    uint8_t total_ogg;
    uint8_t total_wav;
    uint8_t total_flac;

    uint8_t total_zip;
    uint8_t total_7zip;
    uint8_t total_rar;

    uint8_t total_png;
    uint8_t total_jpg;
    uint8_t total_bmp;

    uint8_t total_mp4;
    uint8_t total_mkv;
} folder_node;

node *files = NULL;
folder_node *folder_info = NULL;

static int cursor = 0;
static int list_move = 0;
static int number_of_files = 0;


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

void scan_dir(char *directory)
{
    number_of_files = 0;

    // check if the dir is root, if not, +1 in order to display the ".."
    char buffer[BUFFER_MAX];
    if (strcmp(getcwd(buffer, sizeof(buffer)), ROOT)) number_of_files++;

    DIR *dir = opendir(directory);
    struct dirent *de;

    if (dir)
    {
        while ((de = readdir(dir)))
            number_of_files++;
        closedir(dir);
    }
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

    else if (!strcmp(file, ZIP_FILE))
    {
        folder_info->total_zip++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcmp(file, SEVZIP_FILE))
    {
        folder_info->total_7zip++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcmp(file, RAR_FILE))
    {
        folder_info->total_rar++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcmp(file, PNG_FILE))
    {
        folder_info->total_png++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcmp(file, JPG_FILE))
    {
        folder_info->total_jpg++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcmp(file, BITMAP_FILE))
    {
        folder_info->total_bmp++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcmp(file, MP4_FILE))
    {
        folder_info->total_mp4++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcmp(file, MKV_FILE))
    {
        folder_info->total_mkv++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else SDL_DrawShape(n_white, x, y, w, h); 
}

void free_nodes()
{
    if (files != NULL)
    {
        free(files);
        files = NULL;
    }

    if (folder_info != NULL)
    {
        free(folder_info);
        folder_info = NULL;
    }
}

void print_dir()
{
    int x = 150;
    int shape_x = 100;

    for (int i = 0, j = list_move, nl = 110; i < number_of_files && i < LIST_MAX; i++, j++, nl += 60)
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
            SDL_DrawText(fntSmall, x, nl, n_white, "%s", files[j].file_name);
    }
}

void create_node(char *folder_location)
{
    DIR *dir = opendir(folder_location);
    struct dirent *de;
    
    if (dir)
    {
        files = malloc(number_of_files * (sizeof(*files) + sizeof(*files)));
        folder_info = malloc(sizeof(*folder_info));
        /*memset(files, 0, sizeof(node));
        memset(folder_info, 0, sizeof(folder_node));*/

        folder_info->total = number_of_files;

        int n = 0;
        char buffer[BUFFER_MAX];
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

void file_stuff()
{
    // look at this mess...
    // basically concatenate 2 strings, with a '/' in the middle. Extremely messy.
    char buffer[BUFFER_MAX];
    char full_path[BUFFER_MAX + BUFFER_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", getcwd(buffer, sizeof(buffer)), files[cursor].file_name);

    chdir(full_path);
    cursor = 0;
    scan_dir(full_path);

    // freeeeeeeee...
    free_nodes();

    // create new node with the size of number_of_files.
    create_node(full_path);
}

void move_back_dir()
{
    if (!strcmp(files[0].file_name, ".."))
    {
        cursor = 0;
        list_move = 0;
        file_stuff();
    }
}

void file_select()
{
    if (files[cursor].dir == YES)
    {
        file_stuff();
        list_move = 0;
    }

    else if (!strcasecmp(files[cursor].ext, PAYLOAD))
        reboot_payload(files[cursor].file_name);

    //else if (!strcasecmp(files[cursor].ext, TXT_FILE))

    //else if (!strcasecmp(files[cursor].ext, INI_FILE))

    //else if (!strcasecmp(files[cursor].ext, NRO_FILE))

    //else if (!strcasecmp(files[cursor].ext, NSP_FILE))

    //else if (!strcasecmp(files[cursor].ext, XCI_FILE))
    
    else if (!strcasecmp(files[cursor].ext, MP3_FILE))
        SDL_PlayMusic(files[cursor].file_name);

    else if (!strcasecmp(files[cursor].ext, OGG_FILE))
        SDL_PlayMusic(files[cursor].file_name);

    //else if (!strcasecmp(files[cursor].ext, WAV_FILE))

    //else if (!strcasecmp(files[cursor].ext, FLAC_FILE))
}

void directory_menu()
{
    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        draw_menu("temp");
        print_dir();

        // scroll up...
        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, number_of_files);
            list_move = list_move_up(list_move, cursor, number_of_files, LIST_MAX);
        }

        // scroll down...
        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, number_of_files);
            list_move = list_move_down(list_move, cursor, LIST_MAX);
        }

        if (kDown & KEY_A)
        {
            file_select();
        }

        if (kDown & KEY_B)
        {

        }

        // file option menu.
        if (kDown & KEY_X)
        {
            file_options_menu();
        }

        // exit.
        if (kDown & KEY_PLUS) break;

        // render screen.
        SDL_UpdateRenderer();
    }
}