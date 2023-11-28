#include "Model.hpp"

Model::Model(PDCursesAdapter* adapter) {
    this->adapter = adapter;
    view = new View(adapter);
}

Model::~Model() {
    delete view;
}

int Model::parse_input_char(const int c) {
    if (c == KEY_RESIZE) {
        adapter->resize_windows();
        view->log->print("resize window");
    } else if (c == '\n') {
        adapter->set_cursor(adapter->get_cursor_y() + 1, 1);
        // enter handler
    } else if (c == '\b') {
        if (adapter->get_cursor_x() > 1) {
            adapter->del_char();
        }
        // backspace handler
    } else if (c == '\t') {
        adapter->print_char('\t', adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
        // tab handler
    } else {
        adapter->print_char(c, adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
    }

    return 0;
}