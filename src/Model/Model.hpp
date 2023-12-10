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
    void init();
    void update_line_idxy();

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

    std::vector<MyString> current_file_array; // contains file's lines
    int line_pointer; // pointer to vector at <lines_screen_positions> 
};

#endif