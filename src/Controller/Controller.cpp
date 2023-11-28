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
    while (1) {
        int c = adapter->get_char();

        model->parse_input_char(c);
    }
}