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

#define MAX_FILES   10000   //10,000


// GLOBALS
file_info_t *file_info[MAX_FILES];
folder_info_t *folder_info = NULL;

static char pwd[BUFFER_MAX];

static uint16_t cursor = 0;
static uint8_t list_move = 0;


void free_nodes()
{
    for (int i = 0; i < folder_info->total; i++)
    {
        free(file_info[i]);
        file_info[i] = NULL;
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

    for (uint16_t i = 0, j = list_move, nl = 110; i < folder_info->total && i < LIST_MAX; i++, j++, nl += 60)
    {
        if (file_info[j]->dir == YES)
        {
            SDL_DrawShape(purple, shape_x, nl, 20, 20);
            folder_info->total_folders++;
        }

        else draw_file_icon(file_info[j]->ext, shape_x, nl, 20, 20);
        
        if (j == cursor)
            SDL_DrawText(fntSmall, x, nl, n_cyan, "> %s", file_info[j]->file_name);
        else
            SDL_DrawText(fntSmall, x, nl, n_white, "%s", file_info[j]->file_name);
    }
}

void swap(int i, int j)
{
    // create a temporary file_info_t.
    file_info_t *temp;

    //swap...
    temp = file_info[i];
    file_info[i] = file_info[j];
    file_info[j] = temp;
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
        if (file_info[left]->dir == NO && file_info[i]->dir == YES)
            swap(++last, i);

        // swap if right side comes before left.
        else if (strcasecmp(file_info[i]->file_name, file_info[left]->file_name) < 0)
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
        folder_info = malloc(sizeof(folder_info_t));

        int n = 0;

        if (strcmp(pwd, ROOT))
        {
            // malloc the size of the node.
            file_info[n] = malloc(sizeof(file_info_t));

            // used to go back to the previous dir.
            strcpy(file_info[n]->file_name, "..");
            file_info[n]->dir = YES;

            n++;
        }

        // store all the information of each file in the directory.
        for ( ; (de = readdir(dir)); n++)
        {
            file_info[n] = malloc(sizeof(file_info_t));

            strcpy(file_info[n]->file_name, de->d_name);

            if (is_dir(de->d_name))
                file_info[n]->dir = YES;

            else
            {
                file_info[n]->dir = NO;
                strcpy(file_info[n]->ext, get_filename_ext(de->d_name));
            }
        }
        closedir(dir);

        // save the total number of files.
        folder_info->total = n;

        // sort if more than 1 file exists.
        if (folder_info->total > 1)
        {
            printf("sorting array...\n\n");
            struct_sort(0, folder_info->total - 1);
        }
    }
}

int enter_directory()
{
    // concatenate 2 strings, with a '/' in the middle.
    char full_path[BUFFER_MAX + BUFFER_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", pwd, file_info[cursor]->file_name);

    // change directory to the new path.
    chdir(full_path);

    // set new pwd.
    memset(pwd, '\0', sizeof(pwd));
    getcwd(pwd, sizeof(pwd));

    // reset the values.
    cursor = 0;
    list_move = 0;

    // freeeeeeeee...
    free_nodes();

    // create new file_info_t with the size of number_of_files.
    create_node(full_path);

    return 0;
}

void move_back_dir()
{
    if (!strcmp(file_info[0]->file_name, ".."))
    {
        cursor = 0;
        enter_directory();
    }
}

int file_select()
{
    // if selected file is a dir, enter it.
    if (file_info[cursor]->dir == YES)
        return enter_directory();

    else if (!strcasecmp(file_info[cursor]->ext, PAYLOAD))
        return reboot_payload(file_info[cursor]->file_name);

    //else if (!strcasecmp(file_info[cursor]->ext, TXT_FILE))

    //else if (!strcasecmp(file_info[cursor]->ext, INI_FILE))

    //else if (!strcasecmp(file_info[cursor]->ext, NRO_FILE))

    //else if (!strcasecmp(file_info[cursor]->ext, NSP_FILE))

    //else if (!strcasecmp(file_info[cursor]->ext, XCI_FILE))
    
    else if (!strcasecmp(file_info[cursor]->ext, MP3_FILE))
        return SDL_PlayMusic(file_info[cursor]->file_name);

    else if (!strcasecmp(file_info[cursor]->ext, OGG_FILE))
        return SDL_PlayMusic(file_info[cursor]->file_name);

    //else if (!strcasecmp(file_info[cursor]->ext, WAV_FILE))

    //else if (!strcasecmp(file_info[cursor]->ext, FLAC_FILE))

    else if (!strcasecmp(file_info[cursor]->ext, ZIP_FILE))
        return unzip_menu(pwd, file_info[cursor]->file_name);

    //else if (!strcasecmp(file_info[cursor]->ext, SEVZIP_FILE))

    //else if (!strcasecmp(file_info[cursor]->ext, RAR_FILE))

    else if (!strcasecmp(file_info[cursor]->ext, PNG_FILE) || !strcasecmp(file_info[cursor]->ext, JPG_FILE) || !strcasecmp(file_info[cursor]->ext, BITMAP_FILE))
        return image_menu(file_info[cursor]->file_name);

    return 0;
}

void directory_menu()
{
    getcwd(pwd, sizeof(pwd));

    create_node(pwd);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        draw_menu(pwd);
        print_dir();

        // scroll up...
        if (kDown & KEY_UP)
        {
            cursor = move_cursor_up(cursor, folder_info->total);
            list_move = list_move_up(list_move, cursor, folder_info->total, LIST_MAX);

            printf("cursor = %d\n", cursor);
            printf("list move = %d\n", list_move);
        }

        // scroll down...
        if (kDown & KEY_DOWN)
        {
            cursor = move_cursor_down(cursor, folder_info->total);
            list_move = list_move_down(list_move, cursor, LIST_MAX);

            printf("cursor = %d\n", cursor);
            printf("list move = %d\n", list_move);
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