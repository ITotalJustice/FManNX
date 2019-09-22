#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <switch.h>

#include "dir.h"

#define ROOT "/"


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
            char *folder_location = files[cursor].file_name;

            if(!isDir(folder_location))
            {
                cursor = 0;
                number_of_files = scanDir(folder_location);

                freeNode(files);
                files = NULL;
                files = createNode(number_of_files, folder_location);

                printDir(number_of_files, cursor, files);
            }
        }

        if (kDown & KEY_B)
        {
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