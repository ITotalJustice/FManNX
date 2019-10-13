#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <switch.h>

#include "dir.h"
#include "file_option.h"

#include "sdl.h"
#include "sdl_music.h"

#include "util.h"
#include "gfx_util.h"

#include "zip.h"
#include "image.h"
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

static uint16_t cursor = 0;
static uint8_t list_move = 0;
static uint16_t number_of_files = 0;

static char pwd[BUFFER_MAX];


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

void draw_file_icon(char *file, int x, int y, int w, int h)
{
    if (!strcasecmp(file, PAYLOAD))
    {
        folder_info->total_payloads++;
        SDL_DrawShape(brown, x, y, w, h);
    }

    else if (!strcasecmp(file, TXT_FILE))
    {
        folder_info->total_txt++;
        SDL_DrawShape(green, x, y, w, h);
    }

    else if (!strcasecmp(file, INI_FILE))
    {
        folder_info->total_ini++;
        SDL_DrawShape(orange, x, y, w, h);
    }

    else if (!strcasecmp(file, NRO_FILE))
    {
        folder_info->total_nro++;
        SDL_DrawShape(yellow, x, y, w, h);
    }

    else if (!strcasecmp(file, NSP_FILE))
    {
        folder_info->total_nsp++;
        SDL_DrawShape(pink, x, y, w, h);
    }

    else if (!strcasecmp(file, XCI_FILE))
    {
        folder_info->total_xci++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcasecmp(file, MP3_FILE))
    {
        folder_info->total_mp3++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, OGG_FILE))
    {
        folder_info->total_ogg++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, WAV_FILE))
    {
        folder_info->total_wav++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, FLAC_FILE))
    {
        folder_info->total_flac++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, ZIP_FILE))
    {
        folder_info->total_zip++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcasecmp(file, SEVZIP_FILE))
    {
        folder_info->total_7zip++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcasecmp(file, RAR_FILE))
    {
        folder_info->total_rar++;
        SDL_DrawShape(red, x, y, w, h);
    }

    else if (!strcasecmp(file, PNG_FILE))
    {
        folder_info->total_png++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, JPG_FILE))
    {
        folder_info->total_jpg++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, BITMAP_FILE))
    {
        folder_info->total_bmp++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, MP4_FILE))
    {
        folder_info->total_mp4++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else if (!strcasecmp(file, MKV_FILE))
    {
        folder_info->total_mkv++;
        SDL_DrawShape(indigo, x, y, w, h);
    }

    else SDL_DrawShape(n_white, x, y, w, h); 
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
            SDL_DrawText(fntSmall, x, nl, n_cyan, "> %s", files[j].file_name);
        else
            SDL_DrawText(fntSmall, x, nl, n_white, "%s", files[j].file_name);
    }
}

void swap(int i, int j)
{
    // create a temporary node.
    node *temp = malloc(sizeof(*files));

    //swap...
    *temp = files[i];
    files[i] = files[j];
    files[j] = *temp;

    //free temp.
    free(temp);
}

void struct_sort(int left, int right)
{
    if (left >= right)
        return;

    swap(left, (left + right)/2);
    int last = left;

    for (int i = left + 1; i <= right; i++)
    {
        // if right is a dir and the left side is not, swap.
        if (files[left].dir == NO && files[i].dir == YES)
            swap(++last, i);

        // swap if right side comes before left.
        else if (strcasecmp(files[i].file_name, files[left].file_name) < 0)
            swap(++last, i);
    }

    swap(left, last);
    struct_sort(left, last-1);
    struct_sort(last+1, right);
}

void create_node(char *folder_location)
{
    DIR *dir = opendir(folder_location);
    struct dirent *de;
    
    if (dir)
    {
        files = malloc(number_of_files * (sizeof(*files) + sizeof(*files)));
        folder_info = malloc(sizeof(*folder_info));

        int n = 0;
        char buffer[BUFFER_MAX];
        if (strcmp(getcwd(buffer, sizeof(buffer)), ROOT))
        {
            strcpy(files[n].file_name, "..");
            files[n].dir = 1;
            n++;
            number_of_files++;
        }

        folder_info->total = number_of_files;

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

        if (number_of_files > 2)
            struct_sort(0, number_of_files - 1);
    }
}

int enter_directory()
{
    // concatenate 2 strings, with a '/' in the middle.
    char full_path[BUFFER_MAX + BUFFER_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", pwd, files[cursor].file_name);

    // change directory to the new path.
    chdir(full_path);
    getcwd(pwd, sizeof(pwd));

    // reset the values.
    cursor = 0;
    list_move = 0;
    number_of_files = scan_dir(full_path);

    // freeeeeeeee...
    free_nodes();

    // create new node with the size of number_of_files.
    create_node(full_path);

    return 0;
}

void move_back_dir()
{
    if (!strcmp(files[0].file_name, ".."))
    {
        cursor = 0;
        enter_directory();
    }
}

int file_select()
{
    // if selected file is a dir, enter it.
    if (files[cursor].dir == YES)
        return enter_directory();

    else if (!strcasecmp(files[cursor].ext, PAYLOAD))
        return reboot_payload(files[cursor].file_name);

    //else if (!strcasecmp(files[cursor].ext, TXT_FILE))

    //else if (!strcasecmp(files[cursor].ext, INI_FILE))

    //else if (!strcasecmp(files[cursor].ext, NRO_FILE))

    //else if (!strcasecmp(files[cursor].ext, NSP_FILE))

    //else if (!strcasecmp(files[cursor].ext, XCI_FILE))
    
    else if (!strcasecmp(files[cursor].ext, MP3_FILE))
        return SDL_PlayMusic(files[cursor].file_name);

    else if (!strcasecmp(files[cursor].ext, OGG_FILE))
        return SDL_PlayMusic(files[cursor].file_name);

    //else if (!strcasecmp(files[cursor].ext, WAV_FILE))

    //else if (!strcasecmp(files[cursor].ext, FLAC_FILE))

    else if (!strcasecmp(files[cursor].ext, ZIP_FILE))
        return unzip_menu(pwd, files[cursor].file_name);

    //else if (!strcasecmp(files[cursor].ext, SEVZIP_FILE))

    //else if (!strcasecmp(files[cursor].ext, RAR_FILE))

    else if (!strcasecmp(files[cursor].ext, PNG_FILE))
        return image_menu(files[cursor].file_name);

    else if (!strcasecmp(files[cursor].ext, JPG_FILE))
        return image_menu(files[cursor].file_name);

    else if (!strcasecmp(files[cursor].ext, BITMAP_FILE))
        return image_menu(files[cursor].file_name);

    return 0;
}

void directory_menu()
{
    number_of_files = scan_dir(ROOT);
    create_node(ROOT);

    getcwd(pwd, sizeof(pwd));

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        draw_menu(pwd);
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
            if (file_select() == APP_EXIT)
                break;

        if (kDown & KEY_B)
            move_back_dir();

        // file option menu.
        if (kDown & KEY_X)
            if (file_options_menu(pwd) == APP_EXIT)
                break;

        // exit.
        if (kDown & KEY_PLUS)
            break;

        // render screen.
        SDL_UpdateRenderer();
    }

    free_nodes();
}