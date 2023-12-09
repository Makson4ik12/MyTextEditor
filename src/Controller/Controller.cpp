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

        switch (model->mode) {
            case 0:
                model->parse_nav_edit_mode(c);
                break;

            case 1:
                model->parse_text_input_mode(c);
                break;
            
            case 2:
                model->parse_cmd_input_mode(c);
                break;

            case 3: 
                model->parse_search_mode(c);
                break;
        }
    }
}