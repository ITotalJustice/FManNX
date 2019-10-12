#ifndef _DIR_H_
#define _DIR_H_

#define LIST_MAX    9
#define ROOT        "sdmc:/"

#define BUFFER_MAX  512


int is_dir(char *folder_to_check);          // check if given path is a folder.
void scan_dir(char *directory);             // scan entire dir, return number of files found.
void print_dir(void);                       // print all files in a dir.
void create_node(char *folder_location);    // create a node sizeof (node * number_of_files).

void file_stuff(void);
void move_back_dir(void);
void file_select(void);
void free_nodes(void);
void up(void);
void down(void);


#endif