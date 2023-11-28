#include "PDCursesAdapter.hpp"

PDCursesAdapter::PDCursesAdapter () {
	// raw();	//Берём управление клавиатурой на себя
	initscr(); // Инициализация curses
    cbreak(); // Включение режима посимвольного ввода
    noecho(); // Отключение отображения вводимых символов
    keypad(stdscr, TRUE); // Включение обработки специальных клавиш
}

PDCursesAdapter::~PDCursesAdapter () {
	delwin(this->text_window);
	delwin(this->bottom_window);
    endwin();	// Выход из curses-режима. Обязательная команда.
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

void PDCursesAdapter::resize_windows() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    wresize(this->text_window, max_y - 1, max_x);
    wresize(this->bottom_window, 1, max_x);

    mvwin(this->bottom_window, max_y - 1, 0);

    werase(stdscr);
    box(this->text_window, 0, 0);
    box(this->bottom_window, 0, 0);

    wrefresh(this->text_window);
    wrefresh(this->bottom_window);
}

void PDCursesAdapter::init_windows() {
	int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    this->text_window = newwin(max_y - 1, max_x, 0, 0); // w, w, sy, sx
    scrollok(this->text_window, TRUE);
    box(this->text_window, 0, 0);
	keypad(this->text_window, TRUE);
	set_cursor(1, 1);

    this->bottom_window = newwin(1, max_x, max_y - 1, 0);
    box(this->bottom_window, 0, 0);

    mvwprintw(this->bottom_window, 0, 1, "Press 'q' to exit"); // (window*, y, x, const char*)
    wrefresh(this->bottom_window);
	wrefresh(this->text_window);
}

int PDCursesAdapter::get_char() const {
	return wgetch(this->text_window);
}

void PDCursesAdapter::print_string(const char* text, int y, int x) {
	mvwprintw(this->text_window, 1, 1, text);
	wrefresh(this->text_window);
}

void PDCursesAdapter::print_char(const char c, int y, int x) {
	waddch(this->text_window, c);
	wrefresh(this->text_window);
}

void PDCursesAdapter::del_char() {
    mvwdelch(text_window, get_cursor_y(), get_cursor_x() - 1);
	box(this->text_window, 0, 0);
	wrefresh(text_window);
}