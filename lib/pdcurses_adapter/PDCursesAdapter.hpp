#pragma once

#ifndef PDCURSESADAPTER_H
#define PDCURSESADAPTER_H

#include "../pdcurses/curses.h"
#include "../pdcurses/panel.h"

class PDCursesAdapter {
    public:
    PDCursesAdapter ();
    ~PDCursesAdapter ();

    void print_message(const char* text);

    int get_char();
};

#endif