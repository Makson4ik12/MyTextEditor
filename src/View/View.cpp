#include "View.hpp"

View::View(PDCursesAdapter* adapter) {
    this->adapter = adapter;
    log = new Log();

    this->adapter->init_windows();
}

View::~View() {
    delete log;
}

void View::clear_screen() {
    adapter->clear_main_window();   
    adapter->set_cursor(1, 1);
}

void View::update_console_info(MyString& mode_name, MyString& filename, int line_number, int lines_total) {
    MyString info(" ");
    info += mode_name.c_str();
    info += " | ";
    info += filename.c_str();
    info += " | Ln ";
    info += MyString(std::to_string(line_number + 1));
    info += "/"; 
    info += MyString(std::to_string(lines_total));
    info += " ";

    adapter->print_status(info);
}

void View::update_console_info(MyString& mode_name, MyString& filename, MyString& cmd, int line_number, int lines_total) {
    MyString info(" ");
    info += mode_name.c_str();
    info += " | cmd: ";
    info += cmd.c_str();
    info += " | ";
    info += filename.c_str();
    info += " | Ln ";
    info += MyString(std::to_string(line_number + 1));
    info += "/"; 
    info += MyString(std::to_string(lines_total));
    info += " ";

    adapter->print_status(info);
}

int View::update_screen(std::vector<MyString>& text, int current_line, int direction, int lines_total) {
    adapter->clear_main_window();   

    int start_pos = adapter->y_max * (current_line / adapter->y_max);
    int end_pos = start_pos + adapter->y_max;

    if (end_pos > text.size()) end_pos = text.size();

    for (int i = start_pos, j = 1; i < end_pos; i++, j++) {
        if (text[i].size() > adapter->x_max) {
            MyString tmp = text[i].substr(0, adapter->x_max);
            adapter->print_string(tmp, j, 1);
        } else {
            adapter->print_string(text[i], j, 1);
        }
    }

    if (direction == 1) {
        adapter->set_cursor(1, 1);
    } else if (direction == 0){
        adapter->set_cursor(adapter->y_max, 1);
    }

    return (current_line % adapter->y_max) + 1;
}
