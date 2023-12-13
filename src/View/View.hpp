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
     * @param direction 0 - up, 1 - down
    */
    int update_screen(std::vector<MyString>& text, int current_line, int direction, int lines_total);
    int page_up(int line_pointer, std::vector<MyString>& current_file_array);
    int page_down(int line_pointer, std::vector<MyString>& current_file_array);

    void update_console_info(MyString& mode_name, MyString& filename, int line_number, int lines_total);
    void update_console_info(MyString& mode_name, MyString& filename, MyString& cmd, int line_number, int lines_total);
    void update_line(MyString& line, int line_number, int cursor_idx);
    void clear_screen();

    private:
    PDCursesAdapter* adapter;
};

#endif