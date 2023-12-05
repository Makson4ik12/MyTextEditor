#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "../../lib/pdcurses_adapter/PDCursesAdapter.hpp"
#include "../View/View.hpp"
#include <iostream>
#include <fstream>

class Model {
    public:
    Model(PDCursesAdapter* adapter);
    ~Model();

    MyString current_mode;
    MyString current_file;
    
    int current_line;
    int file_lines_count;

    int parse_input_char(const int c);

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

    int open_file();

    private:
    PDCursesAdapter* adapter;
    View* view;
    MyString* cmd_buffer;
    int mode = 0;

    // work with file
    std::vector<MyString> text_of_file;
    std::vector<std::pair<int, int>> lines_coordinats;
    int current_text_page;
};

#endif