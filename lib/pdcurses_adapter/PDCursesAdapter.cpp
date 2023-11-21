#include "PDCursesAdapter.hpp"

PDCursesAdapter::PDCursesAdapter () {
    initscr();	// Переход в curses-режим
}

PDCursesAdapter::~PDCursesAdapter () {
    endwin();	// Выход из curses-режима. Обязательная команда.
}

int PDCursesAdapter::get_char() {
	return getch();
}

void PDCursesAdapter::print_message(const char* text) {
	printw(text);
	refresh();	// Вывод приветствия на настоящий экран
}