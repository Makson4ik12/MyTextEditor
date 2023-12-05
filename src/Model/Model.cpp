#include "Model.hpp"

Model::Model(PDCursesAdapter* adapter) {
    this->adapter = adapter;
    view = new View(adapter);
    cmd_buffer = new MyString();

    current_mode = "Navigate && Editing";
    current_file = "no opened file";
    current_line = 1;
    file_lines_count = 0;
    current_text_page = 0;

    change_mode(0);

    view->update_console_info(current_mode, current_file, current_line, file_lines_count);
}

Model::~Model() {
    delete view;
    delete cmd_buffer;
}

void Model::change_mode(int new_mode) {
    switch (new_mode) {
        case 0:
            adapter->nav_edit_mode();

            current_mode = "Navigate && Editing";
            break;

        case 1:

            break;
        
        case 2:
            adapter->cmd_mode();

            current_mode = "Command mode";
            break;

        case 3: 
            
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
            // check if it first line
            if (adapter->get_cursor_y() > 1) {
                adapter->set_cursor((adapter->get_cursor_y() - 1), adapter->get_cursor_x());
                current_line--;
            } else if (current_text_page > 0) {
                view->update_screen(text_of_file, --current_text_page, 0);
                current_line--;
            }

            view->update_console_info(current_mode, current_file, current_line, file_lines_count);
            break;

        case KEY_DOWN:
            if (current_line == file_lines_count) break; // check if last line

            if (current_line == adapter->y_max) { // update page if cursor at bottom line
                view->update_screen(text_of_file, ++current_text_page, 1);
            } else {
                adapter->set_cursor(adapter->get_cursor_y() + 1, adapter->get_cursor_x());
            }

            current_line++;
            view->update_console_info(current_mode, current_file, current_line, file_lines_count);
            break;

        case KEY_LEFT:
            adapter->set_cursor(adapter->get_cursor_y(), ((adapter->get_cursor_x() != 1) ? (adapter->get_cursor_x() - 1) : adapter->get_cursor_x()));
            break;

        case KEY_RIGHT:
            adapter->set_cursor(adapter->get_cursor_y(), adapter->get_cursor_x() + 1);
            break;

        case (int) ':':
            change_mode(2);
            view->update_console_info(current_mode, current_file, current_line, file_lines_count);
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
            view->update_console_info(current_mode, current_file, current_line, file_lines_count);
            cmd_buffer->clear();
            break;

        case '\n': // Enter
            if ((cmd_buffer->c_str()[0] == 'o') and (cmd_buffer->c_str()[1] == ' ')) { // cmd 'o <filename>'
                current_file = cmd_buffer->substr(2, cmd_buffer->length() - 2);
                if(!open_file()) return 0;
                change_mode(0);
                view->update_console_info(current_mode, current_file, current_line, file_lines_count);
            } else if((cmd_buffer->c_str()[0] == 'x') and (cmd_buffer->length() == 1)) { // cmd 'x'

            } else if((cmd_buffer->c_str()[0] == 'w') and (cmd_buffer->length() == 1)) { // cmd 'w'

            } else if((cmd_buffer->c_str()[0] == 'x') and (cmd_buffer->length() == 1)) { // cmd 'w <filename>'

            } else if((cmd_buffer->c_str()[0] == 'q') and (cmd_buffer->length() == 1)) { // cmd 'q'

            } else if((cmd_buffer->c_str()[0] == 'q') and (cmd_buffer->c_str()[1] == '!') and (cmd_buffer->length() == 2)) { // cmd 'q!'
                exit(0);
            } else if((cmd_buffer->c_str()[0] == 'w') and (cmd_buffer->c_str()[1] == 'q') and (cmd_buffer->c_str()[2] == '!') and (cmd_buffer->length() == 3)) { // cmd 'wq!'

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
                view->update_console_info(current_mode, current_file, cmd_buffer, current_line, file_lines_count);
            }
            break;
        
        default:
            if (cmd_buffer->length() <= 60) {
                cmd_buffer->append(1, c);
            }
            
            view->update_console_info(current_mode, current_file, cmd_buffer, current_line, file_lines_count);
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
        view->update_console_info(current_mode, current_file, cmd_buffer, current_line, file_lines_count);
        return 0;
    }

    std::string line;
    int pointer = 1;

    for(std::string line; getline(file, line);)
    {
        text_of_file.push_back(line);
        lines_coordinats.push_back(std::make_pair(pointer, line.length()));

        pointer += line.length();
        file_lines_count += 1;
    }

    view->update_screen(text_of_file, current_text_page, 1);

    return 1;
}