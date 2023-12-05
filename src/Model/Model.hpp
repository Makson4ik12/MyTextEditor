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

    int parse_input_char(const int c);

    int open_file();
    int save_to_file(MyString filename);

    private:
    PDCursesAdapter* adapter;
    View* view;
    MyString* cmd_buffer;
    int mode = 0;

    int file_lines_count; // count of line of current file/terminal file
    std::vector<MyString> text_of_file; // contains file's lines
    int current_text_page; // number of current text page on screen
    int __line; // pointer to lines_positions <array with lines names>
    MyString tmp_line; // if last line was too long and she was at bottom that it contains a unprinted part to print on next page
    std::vector<int> lines_positions; // contains numbers of all lines (because few lines can be printing at as a many lines)
};

#endif