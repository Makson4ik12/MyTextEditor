#pragma once

#ifndef PDCURSESADAPTER_H
#define PDCURSESADAPTER_H

#include "../pdcurses/curses.h"
#include "../pdcurses/panel.h"

class PDCursesAdapter {
    public:
    PDCursesAdapter ();
    ~PDCursesAdapter ();

    void init_windows();
    void print_string(const char* text, int y, int x);
    void print_char(const char c, int y, int x);
    void set_cursor (const int y, const int x);
    void resize_windows();
    void del_char();

    int get_char() const;
    int get_cursor_x ();
    int get_cursor_y ();

    private:
    WINDOW* text_window;
    WINDOW* bottom_window;
};

#endif