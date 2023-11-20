#include "PDCursesAdapter.hpp"

PDCursesAdapter::PDCursesAdapter () {
    initscr();			/* Start curses mode 		  */
	printw("Hello World !!!");	/* Print Hello World		  */
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */
}

PDCursesAdapter::~PDCursesAdapter () {
    
}