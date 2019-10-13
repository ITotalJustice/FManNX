#ifndef _DIR_H_
#define _DIR_H_

#define LIST_MAX    9
#define ROOT        "sdmc:/"

#define BUFFER_MAX  512


typedef struct file_info
{
    char file_name[BUFFER_MAX];
    char ext[5];
    u_int8_t dir;
    u_int file_size;

} file_info_t;

typedef struct folder_info
{
    uint16_t total;
    uint16_t total_folders;
    uint16_t total_files;

    uint8_t total_payloads;
    uint8_t total_txt;
    uint8_t total_ini;

    uint8_t total_nro;
    uint8_t total_nsp;
    uint8_t total_xci;

    uint8_t total_mp3;
    uint8_t total_ogg;
    uint8_t total_wav;
    uint8_t total_flac;

    uint8_t total_zip;
    uint8_t total_7zip;
    uint8_t total_rar;

    uint8_t total_png;
    uint8_t total_jpg;
    uint8_t total_bmp;

    uint8_t total_mp4;
    uint8_t total_mkv;

} folder_info_t;


void print_dir(void);                       // print all files in a dir.
void create_node(char *folder_location);    // create a node sizeof (node * number_of_files).

int enter_directory(void);
void move_back_dir(void);
int file_select(void);
void free_nodes(void);

void directory_menu(void);


#endif