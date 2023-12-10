#pragma once

#ifndef VIEW_H
#define VIEW_H

#include "../../lib/pdcurses_adapter/PDCursesAdapter.hpp"
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
     * Update file's page on window, up or down
     * @param text link to vector of MyString
     * @param current_line curren line of text on screen
     * @param direction 0 - up, 1 - down
    */
    int update_screen(std::vector<MyString>& text, int current_line, int direction, int lines_total);

    void update_console_info(MyString& mode_name, MyString& filename, int line_number, int lines_total);

    void update_console_info(MyString& mode_name, MyString& filename, MyString& cmd, int line_number, int lines_total);

    void clear_screen();

    

    private:
    PDCursesAdapter* adapter;
};

#endif