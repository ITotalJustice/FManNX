#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

#define SORT_SMALL  0
#define SORT_LARGE  1


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

void reset_values()
{
    folder_info->total = 0;
    folder_info->total_files = 0;
    folder_info->total_folders = 0;
    folder_info->total_flac = 0;
    folder_info->total_ini = 0;
    folder_info->total_jpg = 0;
    folder_info->total_mkv = 0;
    folder_info->total_mp3 = 0;
    folder_info->total_mp4 = 0;
    folder_info->total_nro = 0;
    folder_info->total_nca = 0;
    folder_info->total_nsp = 0;
    folder_info->total_xci = 0;
    folder_info->total_ogg = 0;
    folder_info->total_payloads = 0;
    folder_info->total_png = 0;
    folder_info->total_rar = 0;
    folder_info->total_txt = 0;
    folder_info->total_wav = 0;
    folder_info->total_zip = 0;
    folder_info->total_7zip = 0;
    folder_info->total_bmp = 0;
    folder_info->total_selected = 0;
}

void set_file_ext_colour(size_t file_in_array)
{
    if (!strcasecmp(file_info[file_in_array]->ext, PAYLOAD))
    {
		folder_info->total_payloads++;
        file_info[file_in_array]->ext_colour = brown;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, TXT_FILE))
    {
		folder_info->total_txt++;
        file_info[file_in_array]->ext_colour = green;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, INI_FILE))
    {
		folder_info->total_ini++;
        file_info[file_in_array]->ext_colour = orange;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, NRO_FILE))
    {
		folder_info->total_nro++;
        file_info[file_in_array]->ext_colour = yellow;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, NCA_FILE))
    {
		folder_info->total_nca++;
        file_info[file_in_array]->ext_colour = pink;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, NSP_FILE))
    {
		folder_info->total_nsp++;
        file_info[file_in_array]->ext_colour = pink;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, XCI_FILE))
    {
		folder_info->total_xci++;
        file_info[file_in_array]->ext_colour = red;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, MP3_FILE))
    {
		folder_info->total_mp3++;
        file_info[file_in_array]->ext_colour = indigo;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, OGG_FILE))
    {
		folder_info->total_ogg++;
        file_info[file_in_array]->ext_colour = indigo;
    }
	else if (!strcasecmp(file_info[file_in_array]->ext, OPUS_FILE))
	{
        folder_info->total_opus++;
        file_info[file_in_array]->ext_colour = indigo;
	}
    else if (!strcasecmp(file_info[file_in_array]->ext, WAV_FILE))
    {
		folder_info->total_wav++;
        file_info[file_in_array]->ext_colour = indigo;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, FLAC_FILE))
    {
		folder_info->total_flac++;
        file_info[file_in_array]->ext_colour = indigo;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, ZIP_FILE))
    {
		folder_info->total_zip++;
        file_info[file_in_array]->ext_colour = red;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, SEVZIP_FILE))
    {
		folder_info->total_7zip++;
        file_info[file_in_array]->ext_colour = red;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, RAR_FILE))
    {
		folder_info->total_rar++;
        file_info[file_in_array]->ext_colour = red;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, PNG_FILE))
    {
		folder_info->total_png++;
        file_info[file_in_array]->ext_colour = indigo;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, JPG_FILE))
    {
		folder_info->total_jpg++;
        file_info[file_in_array]->ext_colour = indigo;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, BITMAP_FILE))
    {
		folder_info->total_bmp++;
        file_info[file_in_array]->ext_colour = indigo;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, MP4_FILE))
    {
		folder_info->total_mp4++;
        file_info[file_in_array]->ext_colour = indigo;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, MKV_FILE))
    {
		folder_info->total_mkv++;
        file_info[file_in_array]->ext_colour = indigo;
    }
	else if (!strcasecmp(file_info[file_in_array]->ext, PDF_FILE))
    {
        folder_info->total_pdf++;
        file_info[file_in_array]->ext_colour = blue;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, EPUB_FILE))
    {
        folder_info->total_epub++;
        file_info[file_in_array]->ext_colour = blue;
    }
    else if (!strcasecmp(file_info[file_in_array]->ext, MOBI_FILE))
    {
        folder_info->total_mobi++;
        file_info[file_in_array]->ext_colour = blue;
    }
    else file_info[file_in_array]->ext_colour = n_white;
}

void print_dir()
{
    int x = 150;
    int shape_x = 100;

    for (size_t i = 0, j = list_move, nl = 110; i < folder_info->total && i < LIST_MAX; i++, j++, nl += 60)
    {
        if (file_info[j]->dir == YES)
            SDL_DrawShape(file_info[j]->ext_colour, shape_x, nl, 20, 20);
        else
        {
            SDL_DrawShape(file_info[j]->ext_colour, shape_x, nl, 20, 20);
            SDL_DrawText(fntSmall, 1000, nl, n_silver, "%.2fMB", file_info[j]->file_size);
        }

        if (file_info[j]->selected == true)
            SDL_DrawShape(red, shape_x, nl, 20, 20);

        if (j == cursor)
            SDL_DrawText(fntSmall, x, nl, n_cyan, "> %s", file_info[j]->file_name);
        else
            SDL_DrawText(fntSmall, x, nl, n_white, "%s", file_info[j]->file_name);
    }
    SDL_DrawText(fntSmall, 25, 680, n_silver, "files: %ld    folders: %ld    selected: %ld    nca: %ld    nsp: %ld    xci: %ld    nro: %ld    mp3: %ld", folder_info->total_files, folder_info->total_folders, folder_info->total_selected, folder_info->total_nca, folder_info->total_nsp, folder_info->total_xci, folder_info->total_nro, folder_info->total_mp3);
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

void size_sort(int left, int right, int order)
{
    if (left >= right)
        return;

    swap(left, (left + right)/2);
    int last = left;

    for (int i = left + 1; i <= right; i++)
    {
        // swap if right side comes before left.
        if (order == SORT_SMALL && file_info[i]->file_size < file_info[left]->file_size)
            swap(++last, i);

        else if (file_info[i]->file_size > file_info[left]->file_size)
            swap(++last, i);
    }

    swap(left, last);
    size_sort(left, last-1, order);
    size_sort(last+1, right, order);
}

void alphabetical_sort(int left, int right)
{
    if (left >= right)
        return;

    swap(left, (left + right)/2);
    int last = left;

    for (int i = left + 1; i <= right; i++)
    {
        if (file_info[left]->dir == NO && file_info[i]->dir == YES)
            swap(++last, i);
        //else if (sort_num_string(file_info[i]->file_name, file_info[left]->file_name))
        else if (strcasecmp(file_info[i]->file_name, file_info[left]->file_name) < 0)
            swap(++last, i);
    }

    swap(left, last);
    alphabetical_sort(left, last-1);
    alphabetical_sort(last+1, right);
}

void create_node(const char *folder_location)
{
    DIR *dir = opendir(folder_location);
    struct dirent *de;
    
    if (dir)
    {
        folder_info = malloc(sizeof(folder_info_t));
        reset_values();

        size_t n = 0;

        if (strcmp(pwd, ROOT))
        {
            // malloc the size of the node.
            file_info[n] = malloc(sizeof(file_info_t));

            // used to go back to the previous dir.
            strcpy(file_info[n]->file_name, "..");
            file_info[n]->dir = YES;
            file_info[n]->ext_colour = purple;
            file_info[n]->selected = false;
            n++;
        }

        // store all the information of each file in the directory.
        for ( ; (de = readdir(dir)); n++)
        {
            file_info[n] = malloc(sizeof(file_info_t));

            strcpy(file_info[n]->file_name, de->d_name);
            file_info[n]->selected = false;

            if (is_dir(de->d_name))
            {
                folder_info->total_folders++;
                file_info[n]->dir = true;
                file_info[n]->ext_colour = purple;
                file_info[n]->file_size = 0;
            }
            else
            {
                folder_info->total_files++;
                file_info[n]->dir = false;
                strcpy(file_info[n]->ext, get_filename_ext(de->d_name));
                set_file_ext_colour(n);
                file_info[n]->file_size = get_filesize(de->d_name);
            }
        }
        closedir(dir);

        // save the total number of files.
        folder_info->total = n;

        // sort if more than 1 file exists.
        if (folder_info->total > 1)
        {
            printf("sorting array...\n\n");
            alphabetical_sort(0, folder_info->total - 1);
            //size_sort(0, folder_info->total - 1, SORT_LARGE);
        }

        // reset the values.
        cursor = 0;
        list_move = 0;
    }
}

int enter_directory()
{
    // concatenate 2 strings, with a '/' in the middle.
    char *full_path;
    if (!asiprintf(&full_path, "%s/%s", pwd, file_info[cursor]->file_name))
        return 1;

    // change directory to the new path.
    chdir(full_path);

    // set new pwd.
    memset(pwd, '\0', sizeof(pwd));
    getcwd(pwd, sizeof(pwd));

    // freeeeeeeee...
    free_nodes();

    // create new file_info_t with the size of number_of_files.
    create_node(full_path);
    free(full_path);

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

void clear_multi_select()
{
    for (size_t i = 0; folder_info->total_selected > 0; i++)
        if (file_info[i]->selected)
        {
            file_info[i]->selected = false;
            folder_info->total_selected--;
        }
}

void select_all()
{
    for (size_t i = 0; i < folder_info->total; i++)
    {
        file_info[i]->selected = true;
        folder_info->total_selected++;
    }
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
        {
            if (folder_info->total_selected > 0)
                clear_multi_select();
            else
                move_back_dir();
        }

        // file option menu.
        if (kDown & KEY_X)
            if (file_options_menu(pwd, file_info[cursor]->file_name) == APP_EXIT)
                break;

        // multi-select
        if (kDown & KEY_Y)
        {
            if (file_info[cursor]->selected)
            {
                folder_info->total_selected--;
                file_info[cursor]->selected = false;
            }
            else
            {
                folder_info->total_selected++;
                file_info[cursor]->selected = true;
            }
        }

        // exit app.
        if (kDown & KEY_PLUS)
            break;

        // render screen.
        SDL_UpdateRenderer();
    }

    free_nodes();
}