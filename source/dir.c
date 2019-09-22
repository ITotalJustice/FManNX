#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <switch.h>

#include "dir.h"


int isDir(char *folder_to_check)
{
    DIR *dir = opendir(folder_to_check);
    if (dir)
    {
        closedir(dir);
        return 0;
    }
    
    return 1;  
}

int scanDir(char *directory)
{
    int files_found = 0;

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

void printDir(int number_of_files, int cursor, node *files)
{
    consoleClear();

    for (int i = 0; i < number_of_files; i++)
    {
        if (cursor == i) printf("> %s\n\n", files[i].file_name);
        else printf("%s\n\n", files[i].file_name);
    }

    consoleUpdate(NULL);
}

node* createNode(int number_of_files, char *folder_location)
{
    DIR *dir = opendir(folder_location);
    struct dirent *de;
    
    struct node *files =  NULL;
    
    if (dir)
    {
        files = malloc(number_of_files * sizeof(*files));

        for (int i = 0; (de = readdir(dir)); i++)
        {
            strcpy(files[i].file_name, de->d_name);
        }
        closedir(dir);
    }

    return files;
}

void freeNode(node *head)
{
    if (head == NULL) return;

	free(head);
}