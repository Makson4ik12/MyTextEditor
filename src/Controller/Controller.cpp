#include "Controller.hpp"

Controller::Controller() {
    adapter = new PDCursesAdapter();
    model = new Model(adapter);
}

Controller::~Controller() {
    delete adapter;
    delete model;
}

void Controller::listen() {
    adapter->print_message("Hello in MyTextEditor\n");

    while (1) {
        adapter->get_char();
    }
}