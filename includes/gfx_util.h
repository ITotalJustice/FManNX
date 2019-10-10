#ifndef _GFX_UTIL_H_
#define _GFX_UTIL_H_

void empty_pop_up_box();
void pop_up_box(int x, int y, int colour, const char *text);
int yes_no_box(int x, int y, const char *question);
void error_box(int x, int y, const char *error_text);

void draw_menu(char *pwd);

#endif