#pragma once

#ifndef VIEW_H
#define VIEW_H

#include "PDCursesAdapter.hpp"
#include "../../lib/mystring/MyString.h"
#include "../Log/Log.hpp"
#include <iostream>
#include <vector>

class View {
    public:
    View(PDCursesAdapter* adapter);
    ~View();

    Log* log;

    /**
     * @param mode
     *      0 - navigation and editing,
     *      1 - text input,
     *      2 - command input,
     *      3 - search
    */
    void set_screen_mode(int mode);

    void update_screen();

    /**
     * @return mode
     *      0 - navigation and editing,
     *      1 - text input,
     *      2 - command input,
     *      3 - search
    */
    int get_screen_mode();

    private:
    PDCursesAdapter* adapter;
    std::vector<MyString>* buffer;
    int screen_mode = 0;
};

#endif