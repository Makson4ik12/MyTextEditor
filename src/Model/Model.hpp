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

    int mode;
    void change_mode(int mode);

    // ****** NAVIGATION MODE COMMANDS***** //
    void nav_cmd_key_up();
    void nav_cmd_key_down();
    void nav_cmd_key_left();
    void nav_cmd_key_right();

    void nav_cmd_move_start();
    void nav_cmd_move_end();
    void nav_cmd_first_page();
    void nav_cmd_last_page();
    void nav_cmd_page_up();
    void nav_cmd_page_down();
    void nav_cmd_default(char c);

    void nav_cmd_x();
    void nav_cmd_p();
    void nav_cmd_delete_line(bool is_copy);
    void nav_cmd_y();
    void nav_cmd_yw();
    void nav_cmd_w();
    void nav_cmd_b();
    void nav_cmd_diw();


    // ****** COMMAND MODE COMMANDS ****** //
    void cmd_go_to_line(int line);
    void cmd_esc();
    void cmd_enter();
    void cmd_backspace(MyString mode_name);
    void cmd_default(MyString mode_name, char c);


    // ****** SEARCH MODE COMMANDS ****** //
    void cmd_search1();
    void cmd_search2();
    void cmd_search_forward(MyString cmd);
    void cmd_search_back(MyString cmd);


    // ****** TEXT INPUT COMMANDS ****** //

    void text_input_cmd_default(char c);
    void text_input_backspace();
    void text_input_delete();
    void text_input_enter();
    void text_input_tab();
    void replace_char(char c);

    private:
    PDCursesAdapter* adapter;
    View* view;
    std::vector<MyString> current_file_array;

    int line_pointer;
    int column_pointer;

    MyString cmd_buffer;
    MyString clipboard;
    MyString last_search;
    MyString current_mode;
    MyString current_file;

    void search(int mode, MyString text, int start_line, int end_line, int start_col, int end_col, MyString cmd);
    void init();
    void find_word(MyString& line, int position, int& start, int& end);
    void create_help_file();
    int read_file();
    int save_to_file(MyString& filename);
};

#endif