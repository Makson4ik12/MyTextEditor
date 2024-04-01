#include "PDCursesAdapter.hpp"

PDCursesAdapter::PDCursesAdapter () {
	initscr(); 
    cbreak(); 
    noecho(); 
    keypad(stdscr, TRUE); 

    getmaxyx(stdscr, y_max, x_max);
    y_max -= 3;
	x_max -= 2;
}

PDCursesAdapter::~PDCursesAdapter () {
	delwin(this->text_window);
	delwin(this->bottom_window);
    endwin();
}

int PDCursesAdapter::get_cursor_x() {
	return getcurx(this->text_window);
}

int PDCursesAdapter::get_cursor_y() {
	return getcury(this->text_window);
}

void PDCursesAdapter::set_cursor(const int y, const int x) {
	wmove(this->text_window, y, x);
	wrefresh(this->text_window);
}

void PDCursesAdapter::init_windows() {
	int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    this->text_window = newwin(max_y - 1, max_x, 0, 0);
    scrollok(this->text_window, TRUE);
    box(this->text_window, 0, 0);
	keypad(this->text_window, TRUE);
	set_cursor(1, 1);

    this->bottom_window = newwin(1, max_x, max_y - 1, 0);
    box(this->bottom_window, 0, 0);

    wrefresh(this->bottom_window);
	wrefresh(this->text_window);
}

int PDCursesAdapter::get_char() {
	return wgetch(this->text_window);
}

void PDCursesAdapter::print_string(MyString& text, int y, int x) {
	mvwprintw(this->text_window, y, x, text.c_str());
	wrefresh(this->text_window);
}

void PDCursesAdapter::print_char(const char c, int y, int x) {
	waddch(this->text_window, c);
	wrefresh(this->text_window);
}

void PDCursesAdapter::print_status(MyString& text) {
    wmove(this->bottom_window, 0, 1);
    wclrtoeol(this->bottom_window);
	mvwprintw(this->bottom_window, 0, 1, text.c_str());
	wrefresh(this->bottom_window);
}

void PDCursesAdapter::del_char() {
	mvwaddch(text_window, get_cursor_y(), get_cursor_x() - 1, ' '); 
	wmove(text_window, get_cursor_y(), get_cursor_x() - 1); 
	wrefresh(text_window);
}

void PDCursesAdapter::clear_main_window() {
    wclear(text_window);
    box(this->text_window, 0, 0);
    wrefresh(this->text_window);
}

void PDCursesAdapter::clear_line(int line_idy) {
	int x = this->get_cursor_x(), y = this->get_cursor_y();

    set_cursor(1, 1);
    wmove(this->text_window, line_idy, 0);
    wclrtoeol(this->text_window);
    set_cursor(x, y);
	box(this->text_window, 0, 0);

    wrefresh(this->text_window);
}
