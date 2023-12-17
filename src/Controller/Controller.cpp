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
                parse_nav_edit_mode(c);
                break;

            case 1:
                model->parse_text_input_mode(c);
                break;
            
            case 2:
                parse_cmd_input_mode(c);
                break;

            case 3: 
                parse_search_mode(c);
                break;
            
            case 4: 
                parse_search_mode(c);
                break;
        }
    }
}

int Controller::parse_nav_edit_mode(const int c) {
    switch (c) {        
        case KEY_UP:
            model->nav_cmd_key_up();
            break;

        case KEY_DOWN:
            model->nav_cmd_key_down();
            break;

        case KEY_LEFT:
            model->nav_cmd_key_left();
            break;

        case KEY_RIGHT:
            model->nav_cmd_key_right();
            break;

        case 339: // PAGE_UP
            model->nav_cmd_page_up();
            break;

        case 338: // PAGE_DOWN
            model->nav_cmd_page_down();
            break;

        case (int) '^':
            model->nav_cmd_move_start();
            break;

        case (int) '$':
            model->nav_cmd_move_end();
            break;

        case (int) 'b':
            model->nav_cmd_b();
            break;

        case (int) 'x':
            model->nav_cmd_x();
            break;

        case (int) 'p':
            model->nav_cmd_p();
            break;
            
        case (int) ':':
            model->change_mode(2);
            break;

        case (int) '/':
            model->change_mode(3);
            break;

        case (int) '?':
            model->change_mode(4);
            break;

        default:
            model->nav_cmd_default(c);

            break;
    }

    return 0;
}

int Controller::parse_cmd_input_mode(const int c) {
    switch (c) {        
        case 27: // ESC
            model->cmd_esc();
            break;

        case '\n': // Enter
            model->cmd_enter();
            break;

        case '\b': // Backspace
            model->cmd_backspace(" | cmd: ");
            break;
        
        default:
            model->cmd_default(" | cmd: ", c);
            break;
    }
    
    return 0;
}

int Controller::parse_search_mode(const int c) {
    switch (c) {        
        case 27: // ESC
            model->cmd_esc();
            break;

        case '\n': // Enter
            if (model->mode == 3) {
                model->cmd_search1();
            } else {
                model->cmd_search2();
            }
            break;

        case '\b': // Backspace
            if (model->mode == 3) {
                model->cmd_backspace(" | /");
            } else {
                model->cmd_backspace(" | ?");
            }
            
            break;

        case 'n':
            if (model->mode == 3) {
                model->cmd_search_forward(" | /");
            } else {
                model->cmd_search_back(" | ?");
            }
            break;

        case 'N':
            if (model->mode == 3) {
                model->cmd_search_back(" | /");
            } else {
                model->cmd_search_forward(" | ?");
            }
            break;
        
        default:
            if (model->mode == 3) {
                 model->cmd_default(" | /", c);
            } else {
                model->cmd_default(" | ?", c);
            }
            
            break;
    }
    
    return 0;
}