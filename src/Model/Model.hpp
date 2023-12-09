#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "../../lib/pdcurses_adapter/PDCursesAdapter.hpp"
#include "../View/View.hpp"
#include <iostream>
#include <fstream>
#include <set>

class Model {
    public:
    Model(PDCursesAdapter* adapter);
    ~Model();

    MyString current_mode;
    MyString current_file;
    int mode = 0;

    /**
     * @param mode
     *      0 - navigation and editing,
     *      1 - text input,
     *      2 - command input,
     *      3 - search
    */
    void change_mode(int mode);

    int parse_nav_edit_mode(const int c);
    int parse_text_input_mode(const int c);
    int parse_cmd_input_mode(const int c);
    int parse_search_mode(const int c);

    int read_file();
    int save_to_file(MyString filename);

    private:
    PDCursesAdapter* adapter;
    View* view;
    MyString cmd_buffer;

    std::vector<MyString> text_of_file; // contains file's lines
    int file_lines_count; // count of line of current file/terminal file
    MyString tmp_line; // if last line was too long and she was at bottom that it contains a unprinted part to print on next page
    std::vector<int> lines_screen_positions; // contains screen positions of all lines: [line_idy_in_text_file] = line_idy_on_screen
    int line_pointer; // pointer to lines_screen_positions <array with lines names>
};

#endif