#include <unistd.h>
#include <switch.h>

#include "sdl.h"
#include "util.h"


void empty_pop_up_box()
{
    // outline. box
    SDL_DrawShape(black, (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box.
    SDL_DrawShape(dark_blue, SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
}

void pop_up_box(int x, int y, int colour, const char *text)
{
    // outline. box
    SDL_DrawShape(black, (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box.
    SDL_DrawShape(dark_blue, SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
    // text to draw.
    SDL_DrawText(fntSmall, x, y, colour, text);
}

int yes_no_box(int x, int y, const char *question)
{
    pop_up_box(x, y, white, question);

    // highlight box.
    SDL_DrawShape(faint_blue, 380, 410, 175, 65);
    SDL_DrawShape(faint_blue, 700, 410, 190, 65);
    // option text.
    SDL_DrawButton(fntButtonBig, BUTTON_B, 410, 425, white);
    SDL_DrawText(fntMedium, 455, 425, white, "No");
    SDL_DrawButton(fntButtonBig, BUTTON_A, 725, 425, white);
    SDL_DrawText(fntMedium, 770, 425, white, "Yes");

    SDL_UpdateRenderer();

    while (appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_A) return YES;
        if (kDown & KEY_B) return NO;
    }

    return 0;
}

void error_box(int x, int y, const char *error_text)
{
    pop_up_box(x, y, white, error_text);
    SDL_UpdateRenderer();

    //sleep(3);
}

void draw_menu(const char *pwd)
{
    SDL_ClearRenderer();

    SDL_DrawShape(n_silver, 25, 20, 50, 5);
    SDL_DrawShape(n_silver, 25, 35, 50, 5);
    SDL_DrawShape(n_silver, 25, 50, 50, 5);

    SDL_DrawText(fntMedium, 150, 25, white, "Dir: %s", pwd);
}