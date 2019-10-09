#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <switch.h>

#include "dir.h"
#include "sdl.h"


int is_dir(char *folder_to_check)
{
    DIR *dir = opendir(folder_to_check);
    if (dir)
    {
        closedir(dir);
        return 0;
    }
    
    return 1;  
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

    for (int i = 0, j = list_move, nl = 100; i < number_of_files && i < LIST_MAX; i++, j++, nl += 60)
    {
        if (j == cursor)
            SDL_DrawText(fntSmall, x, nl, grey, "> %s", files[j].file_name);
        else
            SDL_DrawText(fntSmall, x, nl, white, "%s", files[j].file_name);
    }
    SDL_UpdateRenderer();
}

void draw_menu(char *pwd)
{
    SDL_ClearRenderer();

    SDL_DrawShape(dark_grey, 25, 20, 50, 5);
    SDL_DrawShape(dark_grey, 25, 30, 50, 5);
    SDL_DrawShape(dark_grey, 25, 40, 50, 5);

    char buffer[256];
    getcwd(buffer, sizeof(buffer));

    SDL_DrawText(fntMedium, 200, 25, white, "Dir: %s", buffer);
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
            n++;
        }

        for (int i = n; (de = readdir(dir)); i++)
        {
            strcpy(files[i].file_name, de->d_name);
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
    if (!is_dir(full_path))
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