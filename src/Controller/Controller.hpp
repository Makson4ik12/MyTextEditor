#pragma once

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "PDCursesAdapter.hpp"
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
};

#endif