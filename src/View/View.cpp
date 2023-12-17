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

void View::update_console_info(MyString& mode_name, MyString& filename, int line_number, int lines_total, int column_num, int column_total) {
    MyString info(" ");
    info += mode_name.c_str();
    info += " | ";
    info += filename.c_str();
    info += " | Ln ";
    info += MyString(std::to_string(line_number + 1));
    info += "/"; 
    info += MyString(std::to_string(lines_total));
    info += " | Col ";
    info += MyString(std::to_string(column_num + 1));
    info += "/"; 
    info += MyString(std::to_string(column_total));
    info += " ";

    adapter->print_status(info);
}

void View::update_console_info(MyString& mode_name, MyString& filename, MyString cmd_name, MyString& cmd, int line_number, int lines_total, int column_num, int column_total) {
    MyString info(" ");
    info += mode_name.c_str();
    info += cmd_name.c_str();
    info += cmd.c_str();
    info += " | ";
    info += filename.c_str();
    info += " | Ln ";
    info += MyString(std::to_string(line_number + 1));
    info += "/"; 
    info += MyString(std::to_string(lines_total));
    info += " | Col ";
    info += MyString(std::to_string(column_num + 1));
    info += "/"; 
    info += MyString(std::to_string(column_total));
    info += " ";

    adapter->print_status(info);
}

void View::update_line(MyString& line, int line_number, int cursor_idx) {
    adapter->clear_line(line_number);

    if (cursor_idx >= adapter->x_max) {
        MyString tmp = line.substr(cursor_idx - adapter->x_max + 1, adapter->x_max);
        adapter->print_string(tmp, line_number, 1);
        adapter->set_cursor(line_number, adapter->x_max);
    } else {
        if (line.size() >= adapter->x_max) {
            MyString tmp = line.substr(cursor_idx, adapter->x_max);
            adapter->print_string(tmp, line_number, 1);
        } else {
            adapter->print_string(line, line_number, 1);
        }
        
        adapter->set_cursor(line_number, cursor_idx);
    }
}

void View::update_line(MyString& line, int line_number, int cursor_idx, int col_pointer) {
    adapter->clear_line(line_number);

    if (line.size() == 0) {
        adapter->set_cursor(line_number, cursor_idx);
        return;
    }

    int start = col_pointer - cursor_idx + 1;
    int count = adapter->x_max;
    int cursor = cursor_idx;

    if (((start + count) > line.size()) && (line.size() > adapter->x_max)) {//
        while((start + count) > line.size()) (start--, cursor++);
    } else if (line.size() < adapter->x_max){
        count = line.size() - start;
    }

    MyString tmp = line.substr(start, count);
    adapter->print_string(tmp, line_number, 1);

    adapter->set_cursor(line_number, cursor);
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

int View::page_up(int line_pointer, std::vector<MyString>& current_file_array) {
    int current_page = line_pointer / adapter->y_max;
    int ret = line_pointer;

    if (current_page != 0) {
        update_screen(current_file_array, adapter->y_max * (current_page - 1), 1, current_file_array.size());
        ret = adapter->y_max * (current_page - 1);
    } else {
        adapter->set_cursor(1, 1);
        update_screen(current_file_array, 0, 1, current_file_array.size());
        ret = 0;   
    }

    return ret;
}

int View::page_down(int line_pointer, std::vector<MyString>& current_file_array) {
    int current_page = line_pointer / adapter->y_max;
    int ret = line_pointer;

    if (current_page != (current_file_array.size() / adapter->y_max)) {
        update_screen(current_file_array, adapter->y_max * (current_page + 1), 1, current_file_array.size());
        ret = adapter->y_max * (current_page + 1);
    } else {
        adapter->set_cursor(1, 1);
        ret = adapter->y_max * (current_page);
    }

    return ret;
}