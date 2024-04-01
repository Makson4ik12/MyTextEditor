#pragma once

#ifndef VIEW_H
#define VIEW_H

#include "../../lib/pdcurses_adapter/PDCursesAdapter.hpp"
#include "../../lib/mystring/MyString.h"
#include <iostream>
#include <vector>

class View {
    public:
    View(PDCursesAdapter* adapter);
    ~View();

    int update_screen(std::vector<MyString>& text, int current_line, int direction, int lines_total);
    int page_up(int line_pointer, std::vector<MyString>& current_file_array);
    int page_down(int line_pointer, std::vector<MyString>& current_file_array);
    void update_console_info(MyString& mode_name, MyString& filename, int line_number, int lines_total, int column_num, int column_total);
    void update_console_info(MyString& mode_name, MyString& filename, MyString cmd_name, MyString& cmd, int line_number, int lines_total, int column_num, int column_total);
    void update_line(MyString& line, int line_number, int cursor_idx);
    void update_line(MyString& line, int line_number, int cursor_idx, int col_pointer);
    void clear_screen();
    void update_cursor(int cursor_idy, int cursor_idx);

    PDCursesAdapter* adapter;
};

#endif