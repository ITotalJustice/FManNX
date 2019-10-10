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

void print_dir(int number_of_files, int list_move, int cursor, node *files)
{
    int x = 150;
    int shape_x = 100;

    for (int i = 0, j = list_move, nl = 100; i < number_of_files && i < LIST_MAX; i++, j++, nl += 60)
    {
        if (files[j].dir == YES)
            SDL_DrawShape(purple, shape_x, nl, 20, 20);

        else if (!strcmp(files[j].ext, PAYLOAD))
            SDL_DrawShape(brown, shape_x, nl, 20, 20);

        else if (!strcmp(files[j].ext, TXT_FILE))
            SDL_DrawShape(green, shape_x, nl, 20, 20);

        else if (!strcmp(files[j].ext, INI_FILE))
            SDL_DrawShape(orange, shape_x, nl, 20, 20);

        else if (!strcmp(files[j].ext, NRO_FILE))
            SDL_DrawShape(yellow, shape_x, nl, 20, 20);

        else if (!strcmp(files[j].ext, NSP_FILE))
            SDL_DrawShape(pink, shape_x, nl, 20, 20);

        else if (!strcmp(files[j].ext, XCI_FILE))
            SDL_DrawShape(red, shape_x, nl, 20, 20);
        
        else if (!strcmp(files[j].ext, MP3_FILE))
            SDL_DrawShape(indigo, shape_x, nl, 20, 20);

        else SDL_DrawShape(white, shape_x, nl, 20, 20);  
        
        if (j == cursor)
            SDL_DrawText(fntSmall, x, nl, grey, "> %s", files[j].file_name);
        else
            SDL_DrawText(fntSmall, x, nl, white, "%s", files[j].file_name);

        printf("%s, %s\n", files[j].file_name, files[j].ext);
    }
}

void create_node(int number_of_files, char *folder_location)
{
    DIR *dir = opendir(folder_location);
    struct dirent *de;
    
    if (dir)
    {
        files = malloc(number_of_files * sizeof(*files) + sizeof(*files));

        int n = 0;
        char buffer[256];
        if (strcmp(getcwd(buffer, sizeof(buffer)), ROOT))
        {
            strcpy(files[n].file_name, "..");
            strcpy(files[n].ext, "..");
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

void free_node(node *head)
{
    // check if head is already NULL.
    if (head == NULL) return;

	free(head);
}

int file_stuff(int *cursor, int *number_of_files)
{
    // look at this mess...
    // basically concatenate 2 strings, with a '/' in the middle. Extremely messy.
    char buffer[1024];
    char full_path[256 + sizeof(buffer)];
    snprintf(full_path, sizeof(full_path), "%s/%s", getcwd(buffer, sizeof(buffer)), files[*cursor].file_name);

    // if this is a directory, the code inside executes.
    if (is_dir(full_path))
    {
        chdir(full_path);
        *cursor = 0;
        *number_of_files = scan_dir(full_path);

        // freeeeeeeee...
        free_node(files);
        //always set to NULL after free.
        files = NULL;
        // create new node with the size of number_of_files.
        create_node(*number_of_files, full_path);

        return 0;
    }

    // if selected file was not a dir.
    return 1;
}