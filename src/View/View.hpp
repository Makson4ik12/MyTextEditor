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
     * @param vector_start_line vector position to print on screen
     * @param direction 0 - up, 1 - down
    */
    void update_screen(std::vector<MyString>& text, int vector_start_line, int direction);

    void update_console_info(MyString& mode_name, MyString& filename, int line_number, int lines_total);
    
    void update_console_info(MyString& mode_name, MyString& filename, MyString* cmd, int line_number, int lines_total);

    private:
    PDCursesAdapter* adapter;
};

#endif