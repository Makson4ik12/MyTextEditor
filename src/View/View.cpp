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
    info += " | ";
    info += MyString(std::to_string(line_number));
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
    info += " | ";
    info += MyString(std::to_string(line_number));
    info += "/"; 
    info += MyString(std::to_string(lines_total));
    info += " ";

    adapter->print_status(info);
}

void View::update_screen(std::vector<MyString>& text, int current_line, int direction, MyString& tmp_l) {
    adapter->clear_main_window();   

    int start_pos = current_line - 1;
    int end_pos = start_pos + adapter->y_max + 1;

    if (end_pos > text.size()) end_pos = text.size();

    if (tmp_l.length() != 0) { // if buffer has part of last line that wasn't printed at console
        int virtual_lines = tmp_l.size() / (adapter->x_max - 2);
        if ((tmp_l.size() % (adapter->x_max - 2)) != 0) virtual_lines += 1;

        if (virtual_lines > adapter->y_max) {

        } else {
            MyString tmp;

            for (int i = 0, j = 0; i < virtual_lines; i++, j++){
                int s = i * adapter->x_max;
                int e = s + adapter->x_max - 1;

                if (e > tmp_l.length()) e = tmp_l.length();

                tmp = tmp_l.substr(s, e);
                adapter->print_string(tmp, j + 1, 1);
            }

            start_pos += virtual_lines;
            end_pos = start_pos + adapter->y_max;

            if (end_pos > text.size()) end_pos = text.size();
        }
    }

    for (int i = start_pos, j = 1; i < end_pos; i++, j++) {
        if (text[i].length() > (adapter->x_max - 2)) { // if line more than console line
            int virtual_lines = text[i].size() / (adapter->x_max - 2);
            if ((text[i].size() % (adapter->x_max - 2)) != 0) virtual_lines += 1;

            if ((virtual_lines + i) >= end_pos) {
                // когда в конце строки
            } else {
                MyString tmp;

                for (int k = 0, t = 0; k < virtual_lines; k++, t++){
                    int delimer_idx = k * (adapter->x_max - 2);
                    int count = adapter->x_max - 2;

                    if ((delimer_idx + count) > text[i].length()) count = text[i].length() - delimer_idx;

                    tmp = text[i].substr(delimer_idx, count);
                    adapter->print_string(tmp, j++, 1);
                    end_pos--;
                }

                j--;
            }
        } else {
            adapter->print_string(text[i], j, 1);
        }
    }

    if (direction) {
        adapter->set_cursor(1, 1);
    } else {
        adapter->set_cursor(adapter->y_max, 1);
    }
}