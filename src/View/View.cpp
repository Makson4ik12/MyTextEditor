#include "View.hpp"

View::View(PDCursesAdapter* adapter) {
    this->adapter = adapter;

    log = new Log();

    this->adapter->init_windows();
}

View::~View() {
    delete log;
}

void View::update_console_info(MyString& mode_name, MyString& filename, int line_number, int lines_total) {
    MyString info(" ");
    info += mode_name.c_str();
    info += " | ";
    info += filename.c_str();
    info += " | ";
    info += MyString(std::to_string(line_number));
    info += "/"; 
    info += MyString(std::to_string(lines_total));
    info += " ";

    adapter->print_status(info);
}

void View::update_console_info(MyString& mode_name, MyString& filename, MyString* cmd, int line_number, int lines_total) {
    MyString info(" ");
    info += mode_name.c_str();
    info += " | cmd: ";
    info += cmd->c_str();
    info += " | ";
    info += filename.c_str();
    info += " | ";
    info += MyString(std::to_string(line_number));
    info += "/"; 
    info += MyString(std::to_string(lines_total));
    info += " ";

    adapter->print_status(info);
}

void View::update_screen(std::vector<MyString>& text, int vector_start_line, int direction) {
    adapter->clear_main_window();

    int start_pos = (adapter->y_max) * vector_start_line;
    int end_pos = start_pos + adapter->y_max;

    if (end_pos > text.size()) end_pos = text.size();

    for (int i = start_pos, j = 0; i < end_pos; i++, j++) {
        adapter->print_string(text[i], j + 1, 1);
    }

    if (direction) {
        adapter->set_cursor(1, 1);
    } else {
        adapter->set_cursor(adapter->y_max, 1);
    }
}