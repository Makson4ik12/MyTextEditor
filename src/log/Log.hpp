#pragma once

#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iomanip>
#include "../../lib/mystring/MyString.h"

class Log {
    public:
    Log ();
    ~Log ();

    void print(const char* text);
};

#endif