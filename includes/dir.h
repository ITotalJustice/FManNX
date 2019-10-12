#ifndef _DIR_H_
#define _DIR_H_

#define LIST_MAX    9
#define ROOT        "sdmc:/"

#define BUFFER_MAX  512


void print_dir(void);                       // print all files in a dir.
void create_node(char *folder_location);    // create a node sizeof (node * number_of_files).

int enter_directory(void);
void move_back_dir(void);
int file_select(void);
void free_nodes(void);

void directory_menu(void);


#endif