#include <switch.h>

#include "util.h"
#include "sdl.h"

void image_refresh_screen(double rotate)
{
    SDL_SetRenderDrawColor(SDL_GetRenderer(SDL_GetWindow()), 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(SDL_GetRenderer(SDL_GetWindow()));
    SDL_DrawImageRotate(loaded_image_temp, 0, 0, rotate);
}

int image_menu(const char *file)
{
    // load the image.
    SDL_ImageLoad(&loaded_image_temp, file);

    double rotate = 0;

    while (appletMainLoop())
    {
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidScanInput();

        image_refresh_screen(rotate);

        // rotate image clockwise.
        if (kDown & KEY_R)
        {
            rotate+=90;
            if (rotate == 360) rotate = 0;
        }

        // rotate image anti-clockwise.
        if (kDown & KEY_L)
        {
            if (rotate == 0) rotate = 360;
            rotate-=90;
        }

        // exit.
        if (kDown & KEY_B) break;

        if (kDown & KEY_PLUS) return APP_EXIT;

        SDL_UpdateRenderer();
    }

    return 0;
}