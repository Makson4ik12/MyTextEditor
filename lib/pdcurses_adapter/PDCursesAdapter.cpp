#include "PDCursesAdapter.hpp"

PDCursesAdapter::PDCursesAdapter () {
	// raw();	//Берём управление клавиатурой на себя
	initscr(); // Инициализация curses
    cbreak(); // Включение режима посимвольного ввода
    noecho(); // Отключение отображения вводимых символов

    getmaxyx(stdscr, y_max, x_max);
    y_max -= 3;
	x_max -= 2;
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

    wrefresh(this->bottom_window);
	wrefresh(this->text_window);
}

int PDCursesAdapter::get_char() const {
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
	mvwaddch(text_window, get_cursor_y(), get_cursor_x() - 1, ' ');  // Вставляем пробел в текущем месте
	wmove(text_window, get_cursor_y(), get_cursor_x() - 1);  // Перемещаем курсор влево
	wrefresh(text_window);
}

void PDCursesAdapter::clear_main_window() {
    wclear(text_window); // Очистка всего окна
    box(this->text_window, 0, 0);
    wrefresh(this->text_window);
}

void PDCursesAdapter::nav_edit_mode() {
    noecho(); // Отключение отображения вводимых символов
    keypad(this->text_window, TRUE); // Включение обработки специальных клавиш
}

void PDCursesAdapter::cmd_mode() {
    noecho(); // Отключение отображения вводимых символов
    keypad(stdscr, TRUE); // Включение обработки специальных клавиш
}