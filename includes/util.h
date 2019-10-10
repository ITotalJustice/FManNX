#ifndef _UTIL_H_
#define _UTIL_H_

#define NO      0
#define YES     1

#define PAYLOAD     "bin"
#define TXT_FILE    "txt"
#define INI_FILE    "ini"
#define NRO_FILE    "nro"
#define NSP_FILE    "nsp"
#define XCI_FILE    "xci"

#define MP3_FILE    "mp3"
#define OGG_FILE    "ogg"
#define FLAC_FILE   "flac"
#define WAV_FILE    "wav"


int move_cursor_up(int cursor, int cursor_max);
int move_cursor_down(int cursor, int cursor_max);

int list_move_up(int list_move, int cursor, int number_of_files, int list_max);
int list_move_down(int list_move, int cursor, int list_max);

const char *get_filename_ext(const char *filename);

int file_exists(char *newfile_buffer, const char *src);
void create_dir(const char *dir);

void copy_file(const char *src, char *dest);
void copy_folder(const char *src, char *dest);

void move_file(const char *src, char *dest);
void move_folder(const char *src, char *dest);

#endif