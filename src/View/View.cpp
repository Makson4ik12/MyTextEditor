#include "View.hpp"

View::View(PDCursesAdapter* adapter) {
    this->adapter = adapter;

    buffer = new std::vector<MyString>;
    log = new Log();

    this->adapter->init_windows();
}

View::~View() {
    delete buffer;
}