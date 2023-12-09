#include "Model.hpp"

Model::Model(PDCursesAdapter* adapter) {
    this->adapter = adapter;
    view = new View(adapter);

    current_file = "no open file";
    file_lines_count = 1;
    line_pointer = 0;
    lines_screen_positions.push_back(1);

    change_mode(0);
}

Model::~Model() {
    delete view;
}

void Model::change_mode(int new_mode) {
    switch (new_mode) {
        case 0:
            adapter->nav_edit_mode();
            current_mode = "> Navigation mode <";
            view->update_console_info(current_mode, current_file, lines_screen_positions[line_pointer], file_lines_count);
            break;

        case 1:

            break;
        
        case 2:
            adapter->cmd_mode();
            current_mode = "> Command mode <";
            view->update_console_info(current_mode, current_file, cmd_buffer, lines_screen_positions[line_pointer], file_lines_count);
            break;

        case 3: 
            adapter->cmd_mode();
            current_mode = "> Search mode <";
            view->update_console_info(current_mode, current_file, cmd_buffer, lines_screen_positions[line_pointer], file_lines_count);
            break;

        default:
            return;
    }

    mode = new_mode;
}


/**
 * 
 * Исправить вставку большой строки внизу экрана (в View), так как щас оно пока не работает (там пусто)
 * 
*/



int Model::parse_nav_edit_mode(const int c) {
    switch (c) {        
        case 3: // ^C
            
            break;

        case 22: // ^v
            
            break;
        
        case KEY_UP:
            // check if it first line
            if (adapter->get_cursor_y() > 1) {
                adapter->set_cursor((adapter->get_cursor_y() - 1), adapter->get_cursor_x());
                line_pointer--;
            } else if (line_pointer > 1) {
                view->update_screen(text_of_file, (line_pointer - adapter->y_max + 1), 0, tmp_line);
                line_pointer--;
            }
            
            view->update_console_info(current_mode, current_file, lines_screen_positions[line_pointer], file_lines_count);
            break;

        case KEY_DOWN:
            if (lines_screen_positions[line_pointer] == file_lines_count) break; // check if last line

            if (adapter->get_cursor_y() == adapter->y_max) { // update page if cursor at bottom line
                view->update_screen(text_of_file, ++line_pointer, 1, tmp_line);
            } else {
                adapter->set_cursor(adapter->get_cursor_y() + 1, adapter->get_cursor_x());
                line_pointer++;
            }

            view->update_console_info(current_mode, current_file, lines_screen_positions[line_pointer], file_lines_count);
            break;

        case KEY_LEFT:
            adapter->set_cursor(adapter->get_cursor_y(), ((adapter->get_cursor_x() != 1) ? (adapter->get_cursor_x() - 1) : adapter->get_cursor_x()));
            break;

        case KEY_RIGHT:
            adapter->set_cursor(adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
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

                lines_screen_positions.clear();
                text_of_file.clear();
                line_pointer = 0;
                 file_lines_count = 1;
                lines_screen_positions.push_back(1);
                current_file = "no open file";

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

                if ((num > 0) && (num <= lines_screen_positions.size())) {
                    for (int i = 0; i < lines_screen_positions.size(); i++) {
                        if (num == lines_screen_positions[i]) {
                            int page_pointer = i / adapter->y_max;
                            int cursor_pos = i % adapter->y_max;

                            view->update_screen(text_of_file, lines_screen_positions[adapter->y_max * page_pointer], 1, tmp_line);
                            line_pointer = num;
                            adapter->set_cursor(cursor_pos + 1, 1);
                        }
                    }
                }

                cmd_buffer.clear();
                view->update_console_info(current_mode, current_file, cmd_buffer, lines_screen_positions[line_pointer], file_lines_count);
            }

            cmd_buffer.clear();
            break;

        case '\b': // Backspace
            if (cmd_buffer.length() >= 1) {
                cmd_buffer.erase(cmd_buffer.length() - 1, 1);
                view->update_console_info(current_mode, current_file, cmd_buffer, lines_screen_positions[line_pointer], file_lines_count);
            }
            break;
        
        default:
            if (cmd_buffer.length() <= 60) {
                cmd_buffer.append(1, c);
            }
            
            view->update_console_info(current_mode, current_file, cmd_buffer, lines_screen_positions[line_pointer], file_lines_count);
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
        view->update_console_info(current_mode, current_file, cmd_buffer, lines_screen_positions[line_pointer], file_lines_count);
        return 0;
    }

    std::string line;

    lines_screen_positions.clear();
    text_of_file.clear();
    file_lines_count = 0;
    line_pointer = 0;

    for(std::string line; getline(file, line);)
    {
        text_of_file.push_back(line);

        // if line more than screen size of line -> it can be printed as a few -> line counter cannot be modifed at this lines
        if (line.length() > (adapter->x_max - 2)){
            int virtual_lines = line.size() / (adapter->x_max - 2);
            if ((line.size() % (adapter->x_max - 2)) != 0) virtual_lines += 1;

            for (int i = 0; i < virtual_lines; i++) {
                lines_screen_positions.push_back(file_lines_count + 1);
            }
        } else {
            lines_screen_positions.push_back(file_lines_count + 1);
        }

        file_lines_count += 1;
    }

    view->update_screen(text_of_file, lines_screen_positions[line_pointer], 1, tmp_line);

    return 1;
}

int Model::save_to_file(MyString filename) {
    std::ofstream file(filename.c_str());
    
    if (!file.is_open()) {
        view->log->print("not open");
        view->log->print(filename.c_str());
        return 0;
    }

    for (MyString str : text_of_file) {
        file << str << std::endl;
    }

    file.close();

    return 1;
}