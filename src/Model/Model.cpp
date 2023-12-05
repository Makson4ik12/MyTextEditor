#include "Model.hpp"

Model::Model(PDCursesAdapter* adapter) {
    this->adapter = adapter;
    view = new View(adapter);
    cmd_buffer = new MyString();

    current_file = "no opened file";
    file_lines_count = 1;
    __line = 0;
    lines_positions.push_back(1);

    change_mode(0);
}

Model::~Model() {
    delete view;
    delete cmd_buffer;
}

void Model::change_mode(int new_mode) {
    switch (new_mode) {
        case 0:
            adapter->nav_edit_mode();
            current_mode = "> Navigaion mode <";
            view->update_console_info(current_mode, current_file, lines_positions[__line], file_lines_count);
            break;

        case 1:

            break;
        
        case 2:
            adapter->cmd_mode();
            current_mode = "> Command mode <";
            view->update_console_info(current_mode, current_file, cmd_buffer, lines_positions[__line], file_lines_count);
            break;

        case 3: 
            
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
                __line--;
            } else if (__line > 1) {
                view->update_screen(text_of_file, (__line - adapter->y_max + 1), 0, tmp_line);
                __line--;
            }
            
            view->update_console_info(current_mode, current_file, lines_positions[__line], file_lines_count);
            break;

        case KEY_DOWN:
            if (lines_positions[__line] == file_lines_count) break; // check if last line

            if (adapter->get_cursor_y() == adapter->y_max) { // update page if cursor at bottom line
                view->update_screen(text_of_file, ++__line, 1, tmp_line);
            } else {
                adapter->set_cursor(adapter->get_cursor_y() + 1, adapter->get_cursor_x());
                __line++;
            }

            view->update_console_info(current_mode, current_file, lines_positions[__line], file_lines_count);
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
            cmd_buffer->clear();
            break;

        case '\n': // Enter
            if ((cmd_buffer->c_str()[0] == 'o') and (cmd_buffer->c_str()[1] == ' ')) { // cmd 'o <filename>'
                current_file = cmd_buffer->substr(2, cmd_buffer->length() - 2);
                if(!open_file()) return 0;
                change_mode(0);
            } else if(((cmd_buffer->c_str()[0] == 'x') and (cmd_buffer->length() == 1)) or  // cmd 'x'
                     ((cmd_buffer->c_str()[0] == 'w') and (cmd_buffer->c_str()[1] == 'q') and (cmd_buffer->c_str()[2] == '!') and (cmd_buffer->length() == 3))) {  // cmd 'wq!'
                if(!save_to_file(current_file)) return 0;

                lines_positions.clear();
                text_of_file.clear();
                __line = 0;
                 file_lines_count = 1;
                lines_positions.push_back(1);
                current_file = "no opened file";

                change_mode(0);

                view->clear_screen();
            } else if((cmd_buffer->c_str()[0] == 'w') and (cmd_buffer->length() == 1)) { // cmd 'w'
                if(!save_to_file(current_file)) return 0;
                change_mode(0);
            } else if((cmd_buffer->c_str()[0] == 'w') and (cmd_buffer->c_str()[1] == ' ')) { // cmd 'w <filename>'
                current_file = cmd_buffer->substr(2, cmd_buffer->length() - 2);
                if(!save_to_file(current_file)) return 0;
                change_mode(0);
            } else if((cmd_buffer->c_str()[0] == 'q') and (cmd_buffer->length() == 1)) { // cmd 'q'
                if(!save_to_file(current_file)) return 0;
                exit(0);
            } else if((cmd_buffer->c_str()[0] == 'q') and (cmd_buffer->c_str()[1] == '!') and (cmd_buffer->length() == 2)) { // cmd 'q!'
                exit(0);
            } else if((cmd_buffer->c_str()[0] == 'n') and (cmd_buffer->c_str()[1] == 'u') and (cmd_buffer->c_str()[2] == 'm') and (cmd_buffer->length() == 3)) { // cmd 'num'

            } else if((cmd_buffer->c_str()[0] == 'h') and (cmd_buffer->length() == 1)) { // cmd 'h'

            } else {
                break;
            }

            cmd_buffer->clear();
            break;

        case '\b': // Backspace
            if (cmd_buffer->length() >= 1) {
                cmd_buffer->erase(cmd_buffer->length() - 1, 1);
                view->update_console_info(current_mode, current_file, cmd_buffer, lines_positions[__line], file_lines_count);
            }
            break;
        
        default:
            if (cmd_buffer->length() <= 60) {
                cmd_buffer->append(1, c);
            }
            
            view->update_console_info(current_mode, current_file, cmd_buffer, lines_positions[__line], file_lines_count);
            break;
    }

    return 0;
}

int Model::parse_search_mode(const int c) {
    
    return 0;
}

int Model::parse_input_char(const int c) {
    switch (mode) {
        case 0:
            parse_nav_edit_mode(c);
            break;

        case 1:
            parse_text_input_mode(c);
            break;
        
        case 2:
            parse_cmd_input_mode(c);
            break;

        case 3: 
            parse_search_mode(c);
            break;
    }


    // switch (c) {
    //     case KEY_RESIZE:
    //         // resize window handler
    //         view->log->print("resize screen");
    //         adapter->resize_windows();
    //         break;
        
    //     case (int) '\n':
    //         // enter handler
    //         adapter->set_cursor(adapter->get_cursor_y() + 1, 1);
    //         break;

    //     case (int) '\b':
    //         // backspace handler
    //         if (adapter->get_cursor_x() > 1) {
    //             adapter->del_char();
    //         }
    //         break;
        
    //     case (int) '\t':
    //         // tab handler
    //         adapter->print_char('\t', adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
    //         break;
        
    //     default:
    //         view->log->print(c);
    //         adapter->print_char(c, adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
    //}

    return 0;
}

int Model::open_file() {
    std::fstream file(current_file.c_str());

    if (!file.is_open()) {
        current_file = "no opened file";
        view->update_console_info(current_mode, current_file, cmd_buffer, lines_positions[__line], file_lines_count);
        return 0;
    }

    std::string line;

    lines_positions.clear();
    text_of_file.clear();
    file_lines_count = 0;
    __line = 0;

    for(std::string line; getline(file, line);)
    {
        text_of_file.push_back(line);

        // if line more than screen size of line -> it can be printed as a few -> line counter cannot be modifed at this lines
        if (line.length() > (adapter->x_max - 2)){
            int virtual_lines = line.size() / (adapter->x_max - 2);
            if ((line.size() % (adapter->x_max - 2)) != 0) virtual_lines += 1;

            for (int i = 0; i < virtual_lines; i++) {
                lines_positions.push_back(file_lines_count + 1);
            }
        } else {
            lines_positions.push_back(file_lines_count + 1);
        }

        file_lines_count += 1;
    }

    view->update_screen(text_of_file, lines_positions[__line], 1, tmp_line);

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