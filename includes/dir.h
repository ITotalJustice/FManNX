#ifndef _DIR_H_
#define _DIR_H_

#define LIST_MAX    10
#define ROOT        "sdmc:/"


typedef struct node
{
    char file_name[256];
    char ext[5];
    u_int8_t dir;
    u_int file_size;
} node;

typedef struct folder_node
{
    uint16_t total;
    uint16_t total_folders;
    uint16_t total_files;
    uint16_t total_payloads;
    uint16_t total_txt;
    uint16_t total_ini;
    uint16_t total_nro;
    uint16_t total_nsp;
    uint16_t total_xci;
    uint16_t total_mp3;
    uint16_t total_ogg;
    uint16_t total_wav;
    uint16_t total_flac;
} folder_node;

node *files;
folder_node *folder_info;

int is_dir(char *folder_to_check);                                              // check if given path is a folder.
int scan_dir(char *directory);                                                  // scan entire dir, return number of files found.
void print_dir(int number_of_files, int list_move, int cursor, node *files);    // print all files in a dir.
void create_node(int number_of_files, char *folder_location);                   // create a node sizeof (node * number_of_files).
void file_stuff(int *cursor, int *number_of_files);

#endif