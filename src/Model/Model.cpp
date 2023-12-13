#include "Model.hpp"

Model::Model(PDCursesAdapter* adapter) {
    this->adapter = adapter;
    view = new View(adapter);

    init();

    change_mode(0);
}

Model::~Model() {
    delete view;
}


// ****** NAVIGATION COMMANDS ****** //

void Model::nav_cmd_first_page() {
    line_pointer = view->page_up(1, current_file_array);
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
    cmd_buffer.clear();
}

void Model::nav_cmd_last_page() {
    if (current_file_array.size() > adapter->y_max) {
        line_pointer = view->page_down(current_file_array.size() - adapter->y_max, current_file_array);
    } else {
        line_pointer = view->page_up(1, current_file_array);
    }
    
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
    cmd_buffer.clear();
}

void Model::nav_cmd_move_start() {
    adapter->set_cursor(adapter->get_cursor_y(), 1);
    cmd_buffer.clear();
}

// !!!!
void Model::nav_cmd_move_end() {
    if (current_file_array[line_pointer].size() <= adapter->x_max) {
        if (current_file_array[line_pointer].size() != 0) {
            adapter->set_cursor(adapter->get_cursor_y(), current_file_array[line_pointer].size());
        } else {
            adapter->set_cursor(adapter->get_cursor_y(), 1);
        }
        
    } else {
        // прокрутка вправо
        // ..
    }
    
    cmd_buffer.clear();
}

void Model::nav_cmd_x() {
    int x = adapter->get_cursor_x() - 1;

    if ((x >= 0) && ((x + 1) < (current_file_array[line_pointer].size()))) {
        current_file_array[line_pointer].erase(x + 1, 1);
        view->update_line(current_file_array[line_pointer], adapter->get_cursor_y(), x + 1);
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

    view->update_screen(current_file_array, line_pointer, -1, current_file_array.size());
    adapter->set_cursor(view->update_screen(current_file_array, line_pointer, -1, current_file_array.size()), 1);
    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());

    cmd_buffer.clear();
}

void Model::nav_cmd_y() {
    if (current_file_array[line_pointer].size() != 0) {
        clipboard = current_file_array[line_pointer];
    }
}

void Model::nav_cmd_yw() {
    MyString& line = current_file_array[line_pointer];

    if ((line.size() == 0) || (line[adapter->get_cursor_x() - 1] == ' ')) {
        cmd_buffer.clear();
        return;
    }

    int word_start = 0;
    int word_end = 0;

    find_word(line, adapter->get_cursor_x() - 1, word_start, word_end);

    clipboard = line.substr(word_start, word_end - word_start + 1);
    cmd_buffer.clear();
}

// !!!!!!
void Model::nav_cmd_diw() {
    MyString& line = current_file_array[line_pointer];

    if ((line.size() == 0) || (line[adapter->get_cursor_x() - 1] == ' ')) {
        cmd_buffer.clear();
        return;
    }

    int word_start = 0;
    int word_end = 0;
    int cursor_idx = adapter->get_cursor_x();
    
    find_word(line, adapter->get_cursor_x() - 1, word_start, word_end);
    
    if ((word_start == 0) && (word_end == (line.size() - 1))){
        line.clear();
    } else {
        line.erase(word_start, word_end - word_start + 1);
    }

    if (line.size() == 0) {
        cursor_idx = 1;
    } else if (cursor_idx > line.size()) {
        cursor_idx = line.size();
    }

    view->update_line(current_file_array[line_pointer], adapter->get_cursor_y(), cursor_idx);

    cmd_buffer.clear();
}

// !!!!!!
void Model::nav_cmd_p() {
    // вставка в длинную строке !

    if (clipboard.size() != 0) {
        if ((current_file_array[line_pointer].size() == 0) || ((adapter->get_cursor_x() - 1) == (current_file_array[line_pointer].size() - 1))) {
            current_file_array[line_pointer].append(clipboard.c_str());
        } else {
            current_file_array[line_pointer].insert(adapter->get_cursor_x(), clipboard.c_str());
        }
        
        view->update_line(current_file_array[line_pointer], adapter->get_cursor_y() , adapter->get_cursor_x());
    }

    cmd_buffer.clear();
}

// !!!!
void Model::nav_cmd_w(){
    MyString& cur_line = current_file_array[line_pointer];
    int current_cursor_position = adapter->get_cursor_x();

    // лагает, сделать поддержку прокуртки + переход на некст / ласт страницу

    while (true) {
        if ((cur_line.size() == 0) || (adapter->get_cursor_x() >= cur_line.size())) {
            line_pointer++;
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            cur_line = current_file_array[line_pointer];
            adapter->set_cursor(line_pointer + 1, 1);
            continue;
        }

        int find_index = cur_line.find(" ", adapter->get_cursor_x()); // find near space

        // if space doesnt exist on this line
        if (find_index == - 1) {
                adapter->set_cursor(line_pointer + 1, cur_line.size());
                break;
        } else {

            // if find space not after current word
            if (find_index != current_cursor_position) {
                adapter->set_cursor(line_pointer + 1, find_index);
                break;

            } else { // if find cursor after current word
                adapter->set_cursor(line_pointer + 1, find_index + 1);
            }
        }
    }

    cmd_buffer.clear();
}


// ****** COMMAND COMMANDS ****** //

void Model::cmd_go_to_line(int line) {
    if ((line > 0) && (line <= current_file_array.size())) {
        line_pointer = line - 1;
        adapter->set_cursor(view->update_screen(current_file_array, line - 1, 1, current_file_array.size()), 1);
    }

    cmd_buffer.clear();
}


// ****** PARSERS ****** //
int Model::parse_nav_edit_mode(const int c) {
    switch (c) {        
        case KEY_UP:
            if (line_pointer != 0) {
                int y = adapter->get_cursor_y() - 1;
                int x = adapter->get_cursor_x();
                line_pointer--;
                
                // если вверху экрана
                if (adapter->get_cursor_y() == 1) {
                    view->update_screen(current_file_array, (line_pointer - adapter->y_max), -1, current_file_array.size());
                    y = adapter->y_max;
                }

                // если строка выше короче текущей
                if (current_file_array[line_pointer].size() < x) {
                    if (current_file_array[line_pointer].size() != 0){
                        adapter->set_cursor(y, current_file_array[line_pointer].size());
                    } else {
                        adapter->set_cursor(y, 1);
                    }
                } else {
                    adapter->set_cursor(y, x);
                }

                view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            }
            break;

        case KEY_DOWN:
            if (line_pointer != (current_file_array.size() - 1)) {
                int y = adapter->get_cursor_y() + 1;
                int x = adapter->get_cursor_x();
                line_pointer++;
                
                // если внизу экрана
                if (adapter->get_cursor_y() == adapter->y_max) {
                    view->update_screen(current_file_array, line_pointer, -1, current_file_array.size());
                    y = 1;
                }

                // если строка ниже короче текущей
                if (current_file_array[line_pointer].size() < x) {
                    if (current_file_array[line_pointer].size() != 0){
                        adapter->set_cursor(y, current_file_array[line_pointer].size());
                    } else {
                        adapter->set_cursor(y, 1);
                    }
                } else {
                    adapter->set_cursor(y, x);
                }

                view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            }
            break;

        case KEY_LEFT:
            //если слева
            if (adapter->get_cursor_x() == 1) {
                if ((line_pointer - 1) >= 0) {
                    line_pointer--;

                    int y = adapter->get_cursor_y() - 1;
                    
                    // если вверху
                    if ((adapter->get_cursor_y() == 1) && (line_pointer != 0)) {
                        view->update_screen(current_file_array, (line_pointer - adapter->y_max), 0, current_file_array.size());
                        y = adapter->y_max;
                    }

                    if (current_file_array[line_pointer].size() > adapter->x_max) {
                        // если край строки выше больше, чем размер терминала
                        // ..
                    } else {
                        if (current_file_array[line_pointer].size() != 0) {
                            adapter->set_cursor(y, current_file_array[line_pointer].size());
                        } else {
                            adapter->set_cursor(y, 1);
                        }
                    }
                }
            } else {
                adapter->set_cursor(adapter->get_cursor_y(), adapter->get_cursor_x() - 1);
            }

            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            break;

        case KEY_RIGHT:
            // end console line or end text line
            if ((adapter->get_cursor_x() == adapter->x_max) || (current_file_array[line_pointer].size() == adapter->get_cursor_x()) || (current_file_array[line_pointer].size() == 0)){
                if (current_file_array[line_pointer].size() > adapter->x_max) {
                    // если дошли до конца и строка больше чем экран терминала
                    // ..
                } else if (current_file_array.size() > (line_pointer + 1)) {
                    // enter
                    line_pointer++;

                    // если конец терминала
                    if (adapter->get_cursor_y() == adapter->y_max) {
                        view->update_screen(current_file_array, line_pointer, 1, current_file_array.size());
                    } else {
                        adapter->set_cursor(adapter->get_cursor_y() + 1, 1);
                    }
                }
            } else {
                adapter->set_cursor(adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
            }

            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            break;

        case 339: // PAGE_UP
            line_pointer = view->page_up(line_pointer, current_file_array);
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            break;

        case 338: // PAGE_DOWN
            line_pointer = view->page_down(line_pointer, current_file_array);
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            break;

        case (int) '^':
            nav_cmd_move_start();
            break;

        case (int) '$':
            nav_cmd_move_end();
            break;

        case (int) 'b':

            break;

        case (int) 'x':
            nav_cmd_x();
            break;

        case (int) 'p':
            nav_cmd_p();
            break;
            
        case (int) ':':
            change_mode(2);
            break;

        case (int) '/':

            break;

        case (int) '?':

            break;

        default:
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
                    view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
                }
            }

            break;
    }

    return 0;
}

int Model::parse_text_input_mode(const int c) {
    
    return 0;
}

int Model::parse_cmd_input_mode(const int c) {
    switch (c) {        
        case 27: // ESC
            change_mode(0);
            cmd_buffer.clear();
            break;

        case '\n': // Enter
            if (cmd_buffer.size() == 0) break;

            if ((cmd_buffer.find("o ") == 0) && (cmd_buffer.length() > 2)){// cmd 'o <filename>'
                current_file = cmd_buffer.substr(2, cmd_buffer.length() - 2);
                if(!read_file()) return 0;
                change_mode(0);
            } else if((cmd_buffer == "x") || (cmd_buffer == "wq!")) {  // cmd 'x' & 'wq!'
                if(!save_to_file(current_file)) return 0;

                init();
                change_mode(0);

                view->clear_screen();
            } else if(cmd_buffer == "w") { // cmd 'w'
                if(!save_to_file(current_file)) return 0;
                change_mode(0);
            } else if((cmd_buffer.find("w ") == 0) && (cmd_buffer.length() > 2)) { // cmd 'w <filename>'
                current_file = cmd_buffer.substr(2, cmd_buffer.length() - 2);
                if(!save_to_file(current_file)) return 0;
                change_mode(0);
            } else if(cmd_buffer == "q") { // cmd 'q'
                if(!save_to_file(current_file)) return 0;
                exit(0);
            } else if(cmd_buffer == "q!") { // cmd 'q!'
                exit(0);
            } else if(cmd_buffer == "h") { // cmd 'h'
                MyString help_message = "Welcome to vim-lile MyTextEditor\nThere is 4 work mode";
            } else { // cmd <num>
                cmd_go_to_line(std::atoi(cmd_buffer.c_str()));
                view->update_console_info(current_mode, current_file, cmd_buffer, line_pointer, current_file_array.size());
            }

            cmd_buffer.clear();
            break;

        case '\b': // Backspace
            if (cmd_buffer.length() >= 1) {
                cmd_buffer.erase(cmd_buffer.length() - 1, 1);
                view->update_console_info(current_mode, current_file, cmd_buffer, line_pointer, current_file_array.size());
            }
            break;
        
        default:
            if (cmd_buffer.length() <= 60) {
                cmd_buffer.append(1, c);
            }
            
            view->update_console_info(current_mode, current_file, cmd_buffer, line_pointer, current_file_array.size());
            break;
    }
    
    return 0;
}

int Model::parse_search_mode(const int c) {
    
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
    current_file = "no open file";

    current_file_array.push_back("");
}

void Model::change_mode(int new_mode) {
    switch (new_mode) {
        case 0:
            adapter->nav_edit_mode();
            current_mode = "> Navigation mode <";
            view->update_console_info(current_mode, current_file, line_pointer, current_file_array.size());
            break;

        case 1:

            break;
        
        case 2:
            adapter->cmd_mode();
            current_mode = "> Command mode <";
            view->update_console_info(current_mode, current_file, cmd_buffer, line_pointer, current_file_array.size());
            break;

        case 3: 
            adapter->cmd_mode();
            current_mode = "> Search mode <";
            view->update_console_info(current_mode, current_file, cmd_buffer, line_pointer, current_file_array.size());
            break;

        default:
            return;
    }

    mode = new_mode;
    cmd_buffer.clear();
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
        view->update_console_info(current_mode, current_file, cmd_buffer, line_pointer, current_file_array.size());
        return 0;
    }

    std::string line;

    current_file_array.clear();
    line_pointer = 0;

    for(std::string line; getline(file, line);)
    {
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
        file << str << std::endl;
    }

    file.close();

    return 1;
}