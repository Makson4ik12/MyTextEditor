#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "../../lib/pdcurses_adapter/PDCursesAdapter.hpp"
#include "../View/View.hpp"
#include <iostream>
#include <fstream>
#include <cctype>

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
    void find_word(MyString& line, int position, int& start, int& end);

    int parse_nav_edit_mode(const int c);
    int parse_text_input_mode(const int c);
    int parse_cmd_input_mode(const int c);
    int parse_search_mode(const int c);

    int read_file();
    int save_to_file(MyString& filename);


    // ****** NAVIGATION COMMANDS***** //

    void nav_cmd_move_start();
    void nav_cmd_move_end();
    void nav_cmd_first_page();
    void nav_cmd_last_page();

    void nav_cmd_x();
    void nav_cmd_p();
    void nav_cmd_dd();
    void nav_cmd_y();
    void nav_cmd_yw();
    void nav_cmd_w();
    void nav_cmd_diw();

    // ****** COMMAND COMMANDS ****** //

    void cmd_go_to_line(int line);

    private:
    PDCursesAdapter* adapter;
    View* view;
    MyString cmd_buffer;
    MyString clipboard;

    std::vector<MyString> current_file_array; // contains file's lines
    int line_pointer; // pointer to vector at <lines_screen_positions> 
};

#endif