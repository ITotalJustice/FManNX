int move_cursor_up(int cursor, int cursor_max)
{
    if (cursor == 0) cursor = cursor_max;
    else cursor--;
    return cursor;
}

int move_cursor_down(int cursor, int cursor_max)
{
    if (cursor == cursor_max) cursor = 0;
    else cursor++;
    return cursor;
}