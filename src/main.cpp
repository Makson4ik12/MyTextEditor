#include <iostream>
#include "MyString.h"
#include "PDCursesAdapter.hpp"

int main () {
    //PDCursesAdapter pdcur;
    MyString ms("hello");
    std::cout << ms + "hit" << std::endl;
    std::cin >> ms;
}