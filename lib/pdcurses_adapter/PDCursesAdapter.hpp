#pragma once

#ifndef PDCURSESADAPTER_H
#define PDCURSESADAPTER_H

#include "../pdcurses/curses.h"
#include "../pdcurses/panel.h"
#include "../mystring/MyString.h"

class PDCursesAdapter {
    public:
    PDCursesAdapter ();
    ~PDCursesAdapter ();

    int x_max = 0;
    int y_max = 0;

    void init_windows();
    void print_string(MyString&  text, int y, int x);
    void print_char(const char c, int y, int x);
    void print_status(MyString& text);
    void set_cursor (const int y, const int x);
    void del_char();
    void clear_main_window();

    void nav_edit_mode();
    void cmd_mode();

    int get_char() const;
    int get_cursor_x ();
    int get_cursor_y ();

    private:
    WINDOW* text_window;
    WINDOW* bottom_window;
};

#endif