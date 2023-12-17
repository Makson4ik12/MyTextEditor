#include "Model.hpp"

Model::Model(PDCursesAdapter* adapter) {
    this->adapter = adapter;
    view = new View(adapter);

    init();
}

Model::~Model() {
    delete view;
}


// ****** NAVIGATION MODE COMMANDS ****** //

void Model::nav_cmd_key_up() {
    // если не last line
    if (line_pointer != 0) {
        int y = adapter->get_cursor_y() - 1;
        int x = column_pointer;
        line_pointer--;
        
        // если вверху экрана - обновляем страницу
        if (adapter->get_cursor_y() == 1) {
            view->update_screen(current_file_array, (line_pointer - adapter->y_max), -1, current_file_array.size());
            y = adapter->y_max;
        } else if (x != (adapter->get_cursor_x() - 1))
            view->update_line(current_file_array[line_pointer + 1], y + 1, 0);

        // если строка нулевая
        if (current_file_array[line_pointer].size() == 0){
            adapter->set_cursor(y, 1);
            column_pointer = 0;

        } else if (current_file_array[line_pointer].size() > x) { // если размер строки больше idx курсора
            if (x > adapter->x_max){ // если курсора указывает на часть строки большей, чем размер терминала 
                view->update_line(current_file_array[line_pointer], y, column_pointer);
            } else {
                adapter->set_cursor(y, x + 1);
            }

        } else if (current_file_array[line_pointer].size() <= x) { // если размер строки меньше idx курсора
            column_pointer = current_file_array[line_pointer].size() - 1;

            if (column_pointer > adapter->x_max){ // если курсора указывает на часть строки большей, чем размер терминала 
                view->update_line(current_file_array[line_pointer], y, column_pointer);
            } else {
                adapter->set_cursor(y, column_pointer + 1);
            }

        }

        view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    }
}

void Model::nav_cmd_key_down() {
    // если не last line
    if (line_pointer != (current_file_array.size() - 1)) {
        int y = adapter->get_cursor_y() + 1;
        int x = column_pointer;
        line_pointer++;
        
        // если внизу экрана - обновляем страницу
        if (adapter->get_cursor_y() == adapter->y_max) {
            view->update_screen(current_file_array, line_pointer, -1, current_file_array.size());
            y = 1;
        } else if (x != (adapter->get_cursor_x() - 1))
            view->update_line(current_file_array[line_pointer - 1], y - 1, 0);

        // если строка нулевая
        if (current_file_array[line_pointer].size() == 0){
            adapter->set_cursor(y, 1);
            column_pointer = 0;

        } else if (current_file_array[line_pointer].size() > x) { // если размер строки больше idx курсора
            if (x > adapter->x_max){ // если курсора указывает на часть строки большей, чем размер терминала 
                view->update_line(current_file_array[line_pointer], y, column_pointer);
            } else {
                adapter->set_cursor(y, x + 1);
            }

        } else if (current_file_array[line_pointer].size() <= x) { // если размер строки меньше idx курсора
            column_pointer = current_file_array[line_pointer].size() - 1;

            if (column_pointer > adapter->x_max){ // если курсора указывает на часть строки большей, чем размер терминала 
                view->update_line(current_file_array[line_pointer], y, column_pointer);
            } else {
                adapter->set_cursor(y, column_pointer + 1);
            }

        }

        view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    }
}

void Model::nav_cmd_key_left() {
    if ((column_pointer != 0) && (adapter->get_cursor_x() == 1)) {
        column_pointer--;
        view->update_line(current_file_array[line_pointer], adapter->get_cursor_y(), column_pointer);
        adapter->set_cursor(adapter->get_cursor_y(), 1);

    } else if (adapter->get_cursor_x() == 1) {
        if ((line_pointer - 1) >= 0) {
            line_pointer--;

            int y = adapter->get_cursor_y() - 1;

            if ((adapter->get_cursor_y() == 1) && (line_pointer != 0)) {
                view->update_screen(current_file_array, (line_pointer - adapter->y_max), 0, current_file_array.size());
                y = adapter->y_max;
            }

            if (current_file_array[line_pointer].size() > adapter->x_max) {
                column_pointer = current_file_array[line_pointer].size() - 1;
                view->update_line(current_file_array[line_pointer], y, column_pointer);
            } else {
                if (current_file_array[line_pointer].size() != 0) {
                    adapter->set_cursor(y, current_file_array[line_pointer].size());
                    column_pointer = current_file_array[line_pointer].size() - 1;
                } else {
                    adapter->set_cursor(y, 1);
                    column_pointer = 0;
                }
            }
        }
    } else {
        adapter->set_cursor(adapter->get_cursor_y(), adapter->get_cursor_x() - 1);
        column_pointer--;
    }

    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
}

void Model::nav_cmd_key_right() {
    MyString& line = current_file_array[line_pointer];

    if (((column_pointer + 1) >= adapter->x_max) && (line.size() > adapter->x_max) && (column_pointer != (line.size() - 1))) {
        column_pointer++;
        view->update_line(line, adapter->get_cursor_y(), column_pointer);
        
    } else if ((column_pointer + 1) >= line.size()) {
        if ((column_pointer + 1) >= adapter->x_max) view->update_line(line, adapter->get_cursor_y(), 0);
        line_pointer++;

        // если конец терминала
        if (adapter->get_cursor_y() == adapter->y_max) {
            view->update_screen(current_file_array, line_pointer, 1, current_file_array.size());
        } else {
            adapter->set_cursor(adapter->get_cursor_y() + 1, 1);
        }

        column_pointer = 0;
    } else {
        column_pointer++;
        adapter->set_cursor(adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
    }

    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
}

void Model::nav_cmd_first_page() {
    line_pointer = view->page_up(1, current_file_array);
    column_pointer = 0;
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    cmd_buffer.clear();
}

void Model::nav_cmd_last_page() {
    if (current_file_array.size() > adapter->y_max) {
        line_pointer = view->page_down(current_file_array.size() - adapter->y_max, current_file_array);
    } else {
        line_pointer = view->page_up(1, current_file_array);
    }
    
    column_pointer = 0;
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    cmd_buffer.clear();
}

void Model::nav_cmd_move_start() {
    if (column_pointer > adapter->x_max)
        view->update_line(current_file_array[line_pointer], adapter->get_cursor_y(), 0);
    
    adapter->set_cursor(adapter->get_cursor_y(), 1);
    column_pointer = 0;

    cmd_buffer.clear();
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
}

void Model::nav_cmd_move_end() {
    if (current_file_array[line_pointer].size() <= adapter->x_max) {
        if (current_file_array[line_pointer].size() != 0) {
            adapter->set_cursor(adapter->get_cursor_y(), current_file_array[line_pointer].size());
        } else {
            adapter->set_cursor(adapter->get_cursor_y(), 1);
        }
        
    } else {
        view->update_line(current_file_array[line_pointer], adapter->get_cursor_y(), current_file_array[line_pointer].size() - 1);
    }
    
    if (current_file_array[line_pointer].size() != 0)
        column_pointer = current_file_array[line_pointer].size() - 1;
    else
        column_pointer = 0;

    cmd_buffer.clear();
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
}

void Model::nav_cmd_x() {
    if ((column_pointer >= 0) && ((column_pointer + 1) < (current_file_array[line_pointer].size()))) {
        current_file_array[line_pointer].erase(column_pointer + 1, 1);
        view->update_line(current_file_array[line_pointer], adapter->get_cursor_y(), adapter->get_cursor_x(), column_pointer);
    }

    cmd_buffer.clear();
}

void Model::nav_cmd_dd() {
    nav_cmd_y();

    if (current_file_array[line_pointer].size() != 0) {
        current_file_array[line_pointer].clear();
    }

    current_file_array.erase(current_file_array.begin() + line_pointer);
    
    if (current_file_array.size() == 0) {
        current_file_array.push_back("");
        line_pointer = 0;
    } else if (line_pointer >= current_file_array.size()) {
        line_pointer--;
    }

    column_pointer = 0;
    view->update_screen(current_file_array, line_pointer, -1, current_file_array.size());
    adapter->set_cursor(view->update_screen(current_file_array, line_pointer, -1, current_file_array.size()), 1);
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());

    cmd_buffer.clear();
}

void Model::nav_cmd_y() {
    if (current_file_array[line_pointer].size() != 0) {
        clipboard = current_file_array[line_pointer];
    }
}

void Model::nav_cmd_yw() {
    MyString& line = current_file_array[line_pointer];

    if ((line.size() == 0) || (line[column_pointer] == ' ')) {
        cmd_buffer.clear();
        return;
    }

    int word_start = 0;
    int word_end = 0;

    find_word(line, column_pointer, word_start, word_end);

    clipboard = line.substr(word_start, word_end - word_start + 1);
    cmd_buffer.clear();
}

void Model::nav_cmd_diw() {
    MyString& line = current_file_array[line_pointer];

    if ((line.size() == 0) || (line[adapter->get_cursor_x() - 1] == ' ')) {
        cmd_buffer.clear();
        return;
    }

    int word_start = 0;
    int word_end = 0;
    int cursor_idx = adapter->get_cursor_x();
    
    find_word(line, column_pointer, word_start, word_end);
    
    if ((word_start == 0) && (word_end == (line.size() - 1))){
        line.clear();
    } else {
        line.erase(word_start, word_end - word_start + 1);
    }

    if (line.size() == 0) {
        column_pointer = 0;
        cursor_idx = 1;
    } else if (column_pointer > line.size()) {
        column_pointer = line.size() - 1;
        cursor_idx = line.size();
    }

    view->update_line(current_file_array[line_pointer], adapter->get_cursor_y(),  cursor_idx, column_pointer);
    cmd_buffer.clear();
}

void Model::nav_cmd_p() {
    if (clipboard.size() != 0) {
        if ((current_file_array[line_pointer].size() == 0) || (column_pointer == (current_file_array[line_pointer].size() - 1))) {
            current_file_array[line_pointer].append(clipboard.c_str());
        } else {
            current_file_array[line_pointer].insert(column_pointer + 1, clipboard.c_str());
        }
        
        view->update_line(current_file_array[line_pointer], adapter->get_cursor_y() , adapter->get_cursor_x(), column_pointer);
    }

    cmd_buffer.clear();
}

void Model::nav_cmd_w(){
    int tmp_line = line_pointer;
    int tmp_col = column_pointer;

    while (tmp_line < current_file_array.size()) {
        if (current_file_array[tmp_line].size() == 0) {
            tmp_line++;
            tmp_col = 0;
        } else if ((current_file_array[tmp_line][tmp_col] == ' ') || (tmp_col == (current_file_array[tmp_line].size() - 1))) {
            if ((tmp_col + 1) < current_file_array[tmp_line].size()) {
                tmp_col++;
            } else {
                tmp_line++;
                tmp_col = 0;
            }
        } else {
            int find_index = current_file_array[tmp_line].find(" ", tmp_col);

            if (find_index == (tmp_col + 1)) {
                if ((tmp_col + 1) < current_file_array[tmp_line].size()) {
                    tmp_col++;
                } else {
                    tmp_line++;
                    tmp_col = 0;
                }

                continue;

            } else if (find_index == -1) {
                column_pointer = current_file_array[tmp_line].size() - 1;

            } else {
                column_pointer = find_index - 1;
            }

            int cursor_idy = view->update_screen(current_file_array, tmp_line, -1, current_file_array.size());

            line_pointer = tmp_line;
            
            if ((column_pointer + 1) > adapter->x_max) {
                adapter->set_cursor(cursor_idy, adapter->x_max);
            } else {
                adapter->set_cursor(cursor_idy, column_pointer + 1);
            }

            view->update_line(current_file_array[line_pointer], adapter->get_cursor_y() , adapter->get_cursor_x(), column_pointer);
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
            return;
        }
    }

    cmd_buffer.clear();
}

void Model::nav_cmd_b() {
    int tmp_line = line_pointer;
    int tmp_col = column_pointer;

    while (tmp_line >= 0) {
        if (current_file_array[tmp_line].size() == 0) {
            tmp_line--;
            tmp_col = current_file_array[tmp_line].size() - 1;
        } else if ((current_file_array[tmp_line][tmp_col] == ' ') || (tmp_col == 0)) {
            if ((tmp_col - 1) >= 0) {
                tmp_col--;
            } else {
                tmp_line--;
                tmp_col = current_file_array[tmp_line].size() - 1;
            }
        } else {
            int find_index = tmp_col;
            while((find_index >= 0) && (current_file_array[tmp_line][find_index] != ' ')) find_index--;
            

            if (find_index == (tmp_col - 1)) {
                if ((tmp_col - 1) >= 0) {
                    tmp_col--;
                } else {
                    tmp_line--;
                    tmp_col = current_file_array[tmp_line].size() - 1;
                }

                continue;

            } else if (find_index == -1) {
                column_pointer = 0;

            } else {
                column_pointer = find_index + 1;
            }

            int cursor_idy = view->update_screen(current_file_array, tmp_line, -1, current_file_array.size());

            line_pointer = tmp_line;
            
            if ((column_pointer + 1) > adapter->x_max) {
                adapter->set_cursor(cursor_idy, adapter->x_max);
            } else {
                adapter->set_cursor(cursor_idy, column_pointer + 1);
            }

            view->update_line(current_file_array[line_pointer], adapter->get_cursor_y() , adapter->get_cursor_x(), column_pointer);
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
            return;
        }
    }

    cmd_buffer.clear();
}

void Model::nav_cmd_page_up() {
    line_pointer = view->page_up(line_pointer, current_file_array);
    column_pointer = 0;
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
}

void Model::nav_cmd_page_down() {
    line_pointer = view->page_down(line_pointer, current_file_array);
    column_pointer = 0;
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
}

void Model::nav_cmd_default(char c) {
    cmd_buffer.append(1, c);

    if (cmd_buffer.find("gg") != -1) {
        nav_cmd_first_page();

    } else if (cmd_buffer.find("w") != -1) {
        if ((cmd_buffer.size() >= 2) && (cmd_buffer[cmd_buffer.size() - 2] == 'y')) {
            nav_cmd_yw();
        } else if ((cmd_buffer.size() >= 3) && (cmd_buffer[cmd_buffer.size() - 2] == 'i') && (cmd_buffer[cmd_buffer.size() - 3] == 'd')){
            nav_cmd_diw();
        } else {
            nav_cmd_w();
        }

    } else if (cmd_buffer.find("dd") != -1) { 
        nav_cmd_dd();

    } else if (cmd_buffer.find("y") != -1) { 
        nav_cmd_y();

    } else if (cmd_buffer.find("G") != -1) {
        if ((cmd_buffer.size() == 1) || (std::isdigit(cmd_buffer[cmd_buffer.size() - 2]) == 0)) {
            nav_cmd_last_page();
        } else {
            int pointer = cmd_buffer.size() - 2;

            while ((pointer > 0) && (std::isdigit(cmd_buffer[pointer]) != 0)) pointer--;
            if (std::isdigit(cmd_buffer[pointer]) == 0) pointer++;

            cmd_go_to_line(std::atoi(cmd_buffer.c_str() + pointer));
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
        }
    }
}



// ****** COMMAND MODE COMMANDS ****** //

void Model::cmd_go_to_line(int line) {
    if ((line > 0) && (line <= current_file_array.size())) {
        line_pointer = line - 1;
        adapter->set_cursor(view->update_screen(current_file_array, line - 1, 1, current_file_array.size()), 1);
    }

    cmd_buffer.clear();
}

void Model::cmd_esc() {
    change_mode(0);
    cmd_buffer.clear();
}

void Model::cmd_enter() {
    if (cmd_buffer.size() == 0) return;

    if ((cmd_buffer.find("o ") == 0) && (cmd_buffer.length() > 2)){// cmd 'o <filename>'
        current_file = cmd_buffer.substr(2, cmd_buffer.length() - 2);
        if(!read_file()) return;
        change_mode(0);
    } else if((cmd_buffer == "x") || (cmd_buffer == "wq!")) {  // cmd 'x' & 'wq!'
        if(!save_to_file(current_file)) return;

        init();
        change_mode(0);

        view->clear_screen();
    } else if(cmd_buffer == "w") { // cmd 'w'
        if(!save_to_file(current_file)) return;
        change_mode(0);
    } else if((cmd_buffer.find("w ") == 0) && (cmd_buffer.length() > 2)) { // cmd 'w <filename>'
        current_file = cmd_buffer.substr(2, cmd_buffer.length() - 2);
        if(!save_to_file(current_file)) return;
        change_mode(0);
    } else if(cmd_buffer == "q") { // cmd 'q'
        if(!save_to_file(current_file)) return;
        exit(0);
    } else if(cmd_buffer == "q!") { // cmd 'q!'
        exit(0);
    } else if(cmd_buffer == "h") { // cmd 'h'
        MyString help_message = "Welcome to vim-lile MyTextEditor\nThere is 4 work mode";
    } else { // cmd <num>
        cmd_go_to_line(std::atoi(cmd_buffer.c_str()));
        view->update_console_info(current_mode, current_file," | cmd: ", cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    }

    cmd_buffer.clear();
}

void Model::cmd_backspace(MyString mode_name) {
    if (cmd_buffer.length() >= 1) {
        cmd_buffer.erase(cmd_buffer.length() - 1, 1);
        view->update_console_info(current_mode, current_file, mode_name, cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    }
}

void Model::cmd_default(MyString mode_name, char c) {
    if (cmd_buffer.length() <= 60) {
        cmd_buffer.append(1, c);
    }
    
    view->update_console_info(current_mode, current_file, mode_name, cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
}



// ****** SEARCH COMMANDS ****** //

void Model::cmd_search1() {
    if (cmd_buffer.size() != 0)
        search(1, cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[current_file_array.size() - 1].size() - 1, " | /");
}

void Model::cmd_search2() {
    if (cmd_buffer.size() != 0)
        search(2, cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[current_file_array.size() - 1].size() - 1, " | ?");
}

void Model::cmd_search_forward(MyString cmd){
    if (((line_pointer + 1) != current_file_array.size()) && (last_search.size() != 0)) 
        search(1, last_search, line_pointer + 1, current_file_array.size(), 0, current_file_array[current_file_array.size() - 1].size() - 1, cmd);
}

void Model::cmd_search_back(MyString cmd) {
    if (((line_pointer - 1) >= 0) && (last_search.size() != 0)) 
        search(2, last_search, line_pointer - 1, current_file_array.size(), 0, current_file_array[current_file_array.size() - 1].size() - 1, cmd);
}

void Model::search(int mode, MyString text, int start_line, int end_line, int start_col, int end_col, MyString cmd) {
    // search forward
    if (mode == 1) {
        int tmp_line = start_line;
        int tmp_col = start_col;

        while (tmp_line < end_line){
            if ((current_file_array[tmp_line]).size() == 0) {
                tmp_line++;
                tmp_col = 0;
            } else if ((current_file_array[tmp_line]).find(text.c_str(), tmp_col) == -1) {
                tmp_line++;
                tmp_col = 0;
            } else {
                break;
            }
        }

        last_search = text;
        cmd_buffer.clear();

        if ((tmp_line != end_line)) {
            line_pointer = tmp_line;
            column_pointer = 0;
            adapter->set_cursor(view->update_screen(current_file_array, line_pointer, -1, current_file_array.size()), 1);
        }

        view->update_console_info(current_mode, current_file, cmd, cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    
    } else if (mode == 2) {
        int tmp_line = start_line;
        int tmp_col = start_col;

        while (tmp_line >= 0){
            if ((current_file_array[tmp_line]).size() == 0) {
                tmp_line--;
                tmp_col = 0;
            } else if ((current_file_array[tmp_line]).find(text.c_str(), tmp_col) == -1) {
                tmp_line--;
                tmp_col = 0;
            } else {
                break;
            }
        }

        last_search = text;
        cmd_buffer.clear();

        if (tmp_line >= 0) {
            line_pointer = tmp_line;
            column_pointer = 0;
            adapter->set_cursor(view->update_screen(current_file_array, line_pointer, -1, current_file_array.size()), 1);
        }

        view->update_console_info(current_mode, current_file, cmd, cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
    }
}



// ****** PARSERS ****** //

int Model::parse_text_input_mode(const int c) {
    
    return 0;
}
    
//     case (int) '\t':
//         // tab handler
//         adapter->print_char('\t', adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
//         break;



// ****** OTHER COMMANDS ****** //

void Model::init() {
    current_file_array.clear();
    line_pointer = 0;
    column_pointer = 0;
    current_file = "no open file";
    current_file_array.push_back("");

    change_mode(0);
}

void Model::change_mode(int new_mode) {
    switch (new_mode) {
        case 0:
            adapter->nav_edit_mode();
            current_mode = "> Navigation mode <";
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
            break;

        case 1:

            break;
        
        case 2:
            adapter->cmd_mode();
            current_mode = "> Command mode <";
            view->update_console_info(current_mode, current_file, " | cmd: ", cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
            break;

        case 3: 
            adapter->cmd_mode();
            current_mode = "> Search mode <";
            view->update_console_info(current_mode, current_file, " | /",  cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
            break;

        case 4: 
            adapter->cmd_mode();
            current_mode = "> Search mode <";
            view->update_console_info(current_mode, current_file, " | ?",  cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
            break;

        default:
            return;
    }

    mode = new_mode;
    cmd_buffer.clear();
    last_search.clear();
}

void Model::find_word(MyString& line, int position, int& start, int& end) {
    int _start = position, _end = position;

    while ((_start > 0) && (line[_start - 1] != ' ')) {
        _start--;
    }

    if (position >= (line.length() - 1)) {
        _end = position;
    } else {
        while ((_end < line.length()) && (line[_end] != ' ')) {
            _end++;
        }

        if (_end == line.length()) _end--;
    }

    start = _start;
    end = _end;
}

int Model::read_file() {
    std::fstream file(current_file.c_str());

    if (!file.is_open()) {
        current_file = "no open file";
        view->update_console_info(current_mode, current_file, " | cmd: ", cmd_buffer, line_pointer, current_file_array.size(), column_pointer, current_file_array[line_pointer].size());
        return 0;
    }

    std::string line;

    current_file_array.clear();
    line_pointer = 0;
    column_pointer = 0;

    for(std::string line; getline(file, line);)
    {
        line.append(" ");
        current_file_array.push_back(line);
    }

    view->update_screen(current_file_array, line_pointer, 1, current_file_array.size());

    return 1;
}

int Model::save_to_file(MyString& filename) {
    std::ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        view->log->print("not open");
        view->log->print(filename.c_str());
        return 0;
    }

    for (MyString str : current_file_array) {
        if (str.size() != 1)
            str = str.substr(0, str.size() - 1);
        else 
            str = "";
        file << str << std::endl;
    }

    file.close();

    return 1;
}