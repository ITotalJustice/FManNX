#ifndef _DIR_H_
#define _DIR_H_

#define LIST_MAX    10
#define ROOT        "sdmc:/"

typedef struct node
{
    char file_name[256];
} node;

struct node *files;

int is_dir(char *folder_to_check);                                              // check if given path is a folder.
int scan_dir(char *directory);                                                  // scan entire dir, return number of files found.
void print_dir(int number_of_files, int list_move, int cursor, node *files);    // print all files in a dir.
void draw_menu(char *pwd);
void create_node(int number_of_files, char *folder_location);                   // create a node sizeof (node * number_of_files).
int file_stuff(int *cursor, int *number_of_files);
void free_node(node *head);                                                     // free the node (remember to set it to NULL after).

#endif