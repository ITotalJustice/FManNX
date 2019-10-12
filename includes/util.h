#ifndef _UTIL_H_
#define _UTIL_H_


#define APP_EXIT    1

#define NO          0
#define YES         1

#define PAYLOAD     "bin"
#define TXT_FILE    "txt"
#define INI_FILE    "ini"

#define NRO_FILE    "nro"
#define NSP_FILE    "nsp"
#define XCI_FILE    "xci"

#define MP3_FILE    "mp3"
#define OGG_FILE    "ogg"
#define WAV_FILE    "wav"
#define FLAC_FILE   "flac"

#define ZIP_FILE    "zip"
#define SEVZIP_FILE "7z"
#define RAR_FILE    "rar"

#define PNG_FILE    "png"
#define JPG_FILE    "jpg"
#define BITMAP_FILE "bmp"

#define MP4_FILE    "mp4"
#define MKV_FILE    "mkv"


//
int move_cursor_up(int cursor, int cursor_max);
int move_cursor_down(int cursor, int cursor_max);

//
int list_move_up(int list_move, int cursor, int number_of_files, int list_max);
int list_move_down(int list_move, int cursor, int list_max);

//
const char *get_filename_ext(const char *filename);

//
int is_dir(const char *folder_to_check);
int check_if_dir_exists(const char *folder);
int file_exists(char *newfile_buffer, const char *src);

// scan entire dir, return number of files found.
int scan_dir(char *directory);

//
void create_dir(const char *dir);

//
void copy_file(const char *src, char *dest);
void copy_folder(const char *src, char *dest);

//
void move_file(const char *src, char *dest);
void move_folder(const char *src, char *dest);

#endif