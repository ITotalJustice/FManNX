#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <switch.h>
#include <string.h>

#include "dir.h"


void appInit()
{
    consoleInit(NULL);
}

void appExit()
{
    consoleExit(NULL);
}

int main(int argc, char **argv)
{
    // init stuff
    appInit();
    chdir(ROOT);

    int number_of_files = scanDir(ROOT);
    int cursor = 0;
    
    struct node *files = createNode(number_of_files, ROOT);

    printDir(number_of_files, cursor, files);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        // scroll up...
        if (kDown & KEY_UP)
        {
            if (cursor == 0) cursor = number_of_files - 1;
            else cursor--;
            printDir(number_of_files, cursor, files);
        }

        // scroll down...
        if (kDown & KEY_DOWN)
        {
            if (cursor == number_of_files - 1) cursor = 0;
            else cursor++;
            printDir(number_of_files, cursor, files);
        }

        if (kDown & KEY_A)
        {
            // look at this mess...
            // basically concatenate 2 strings, with a '/' in the middle. Extremely messy.
            char buffer[512];
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "%s/%s", getcwd(buffer, sizeof(buffer)), files[cursor].file_name);

            // if this is a directory, the code inside executes.
            if(!isDir(full_path))
            {
                chdir(full_path);
                cursor = 0;
                number_of_files = scanDir(full_path);

                // hacky way to show ".." even in an empty directory
                if (number_of_files == 0) number_of_files++;

                // freeeeeeeee...
                freeNode(files);
                //always set to NULL after free.
                files = NULL;
                // create new node with the size of number_of_files.
                files = createNode(number_of_files, full_path);

                // print out the new list of files.
                printDir(number_of_files, cursor, files);
            }
        }

        // jump back to root dir.
        if (kDown & KEY_B)
        {
            chdir(ROOT);
            cursor = 0;
            number_of_files = scanDir(ROOT);

            freeNode(files);
            files = NULL;
            files = createNode(number_of_files, ROOT);

            printDir(number_of_files, cursor, files);
        }

        if (kDown & KEY_PLUS) break;
    }
    
    // clean then exit...
    freeNode(files);
    appExit();
    return 0;
}