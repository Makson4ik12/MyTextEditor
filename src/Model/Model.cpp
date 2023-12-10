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

void Model::init() {
    current_file_array.clear();
    line_pointer = 0;
    current_file = "no open file";

    current_file_array.push_back("");
}

void Model::update_line_idxy() {

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
}

int Model::parse_nav_edit_mode(const int c) {
    switch (c) {        
        case 3: // ^C
            
            break;

        case 22: // ^v
            
            break;
        
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

        case (int) ':':
            change_mode(2);
            break;

        case (int) '/':

            break;

        case (int) '?':

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
                int num = std::atoi(cmd_buffer.c_str());

                if ((num > 0) && (num <= current_file_array.size())) {
                    line_pointer = num - 1;
                    adapter->set_cursor(view->update_screen(current_file_array, num - 1, 1, current_file_array.size()), 1);
                }

                cmd_buffer.clear();
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

int Model::save_to_file(MyString filename) {
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