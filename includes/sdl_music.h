#ifndef _SDL_MUSIC_H_
#define _SDL_MUSIC_H

int SDL_PlayMusic(char *file);
void SDL_SkipNextMusic(char **array, int *file, int files_max);
void SDL_SkipBackMusic(char **array, int *file, int files_max);


void sdl_music_init(void);
void sdl_music_exit(void);

#endif