#pragma once

#ifndef VIEW_H
#define VIEW_H

#include "PDCursesAdapter.hpp"
#include <iostream>

class View {
    public:
    View(PDCursesAdapter* adapter);
    ~View();

    private:
    PDCursesAdapter* adapter;
};

#endif