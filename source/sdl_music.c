#include <SDL2/SDL_mixer.h>

#define BITRATE 48000

static Mix_Music *music = NULL;


int SDL_PlayMusic(const char *file)
{
    if (music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }

    music = Mix_LoadMUS(file);
    Mix_PlayMusic(music, 0);

    return 0;
}

void SDL_SkipNextMusic(char **array, int *file, int files_max)
{
    if (*file + 1 <= files_max) *++file;
    else *file = 0;

    SDL_PlayMusic(array[*file]);
}

void SDL_SkipBackMusic(char **array, int *file, int files_max)
{
    if (*file - 1 >= 0) *--file;
    else *file = files_max;

    SDL_PlayMusic(array[*file]);
}

void sdl_music_init()
{
    Mix_Init(MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC);
    Mix_OpenAudio(BITRATE, AUDIO_S32LSB, 2, 1024);
    Mix_VolumeMusic(64); //TODO: volume slider
}

void sdl_music_exit()
{
    Mix_HaltChannel(-1);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
}