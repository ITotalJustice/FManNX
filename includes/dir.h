#ifndef _DIR_H_
#define _DIR_H_

typedef struct node
{
    char file_name[256];
    int position;
} node;

int isDir(char *folder_to_check);                                   // check if given path is a folder.
int scanDir(char *directory);                                       // scan entire dir, return number of files found.
void printDir(int number_of_files, int cursor, node *files);        // print all files in a dir.
node* createNode(int number_of_files, char *folder_location);       // create a node sizeof (node * number_of_files).
void freeNode(node *head);                                          // free the node (remember to set it to NULL after).

#endif