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

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_A) return YES;
        if (kDown & KEY_B) return NO;
    }
}

void error_box(int x, int y, const char *error_text)
{
    pop_up_box(x, y, white, error_text);
    SDL_UpdateRenderer();

    //sleep(3);
}

void draw_menu(char *pwd)
{
    SDL_ClearRenderer();

    SDL_DrawShape(dark_grey, 25, 20, 50, 5);
    SDL_DrawShape(dark_grey, 25, 30, 50, 5);
    SDL_DrawShape(dark_grey, 25, 40, 50, 5);

    /*char buffer[256];
    getcwd(buffer, sizeof(buffer));*/

    SDL_DrawText(fntMedium, 200, 25, white, "Dir: %s", pwd);
}

void draw_file_options(int cursor)
{
    char *options[] = { "edit", "cut", "copy", "move", "delete", "rename" };
    SDL_DrawShape(dark_grey, 350, 100, 250, 350);

    for (u_int16_t i = 0, nl = 120; i < 6; i++, nl += 55)
    {
        if (cursor == i)
            SDL_DrawText(fntSmall, 375, nl, grey, options[i]);
        else
            SDL_DrawText(fntSmall, 375, nl, white, options[i]);
    }
}