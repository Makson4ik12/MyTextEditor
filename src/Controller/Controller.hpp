#pragma once

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../../lib/pdcurses_adapter/PDCursesAdapter.hpp"
#include "../model/Model.hpp"
#include <iostream>

class Controller {
    public:
    Controller();
    ~Controller();
    void listen();

    private:
    PDCursesAdapter* adapter;
    Model* model;
    
    int parse_cmd_input_mode(const int c);
    int parse_search_mode(const int c);
    int parse_nav_edit_mode(const int c);
    int parse_text_input_mode(const int c);
    int parse_replace_char_mode(const int c);
};

#endif