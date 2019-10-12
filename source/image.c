#include <switch.h>

#include "util.h"
#include "sdl.h"

void image_refresh_screen()
{
    SDL_RenderClear(SDL_GetRenderer(SDL_GetWindow()));
    SDL_DrawShape(black, 0, 0, SCREEN_W, SCREEN_W);
    SDL_DrawImage(loaded_image_temp, 0, 0);
}

int image_menu(const char *file)
{
    // load the image.
    SDL_ImageLoad(&loaded_image_temp, file);

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        image_refresh_screen();

        // exit.
        if (kDown & KEY_B) break;

        if (kDown & KEY_PLUS) return APP_EXIT;

        SDL_UpdateRenderer();
    }

    return 0;
}