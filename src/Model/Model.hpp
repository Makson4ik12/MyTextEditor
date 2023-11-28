#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "PDCursesAdapter.hpp"
#include "../View/View.hpp"
#include <iostream>

class Model {
    public:
    Model(PDCursesAdapter* adapter);
    ~Model();

    int parse_input_char(const int c);

    private:
    PDCursesAdapter* adapter;
    View* view;
};

#endif