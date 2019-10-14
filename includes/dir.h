#ifndef _DIR_H_
#define _DIR_H_

#define LIST_MAX    9
#define ROOT        "sdmc:/"

#define BUFFER_MAX      512


typedef struct file_info
{
    char file_name[BUFFER_MAX];
    char ext[5];
    bool dir;
    bool selected;
    size_t file_size;

} file_info_t;

typedef struct folder_info
{
    size_t total;
    size_t total_folders;
    size_t total_files;
    size_t total_selected;

    size_t total_payloads;
    size_t total_txt;
    size_t total_ini;

    size_t total_nro;
    size_t total_nsp;
    size_t total_xci;

    size_t total_mp3;
    size_t total_ogg;
    size_t total_wav;
    size_t total_flac;

    size_t total_zip;
    size_t total_7zip;
    size_t total_rar;

    size_t total_png;
    size_t total_jpg;
    size_t total_bmp;

    size_t total_mp4;
    size_t total_mkv;

} folder_info_t;


void print_dir(void);                       // print all files in a dir.
void create_node(const char *folder_location);    // create a node sizeof (node * number_of_files).

int enter_directory(void);
void move_back_dir(void);
int file_select(void);
void free_nodes(void);

void directory_menu(void);


#endif