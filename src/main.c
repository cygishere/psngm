#include <curses.h>

int
main (void)
{
  initscr ();
  printw ("Hwllo, olrd");
  refresh ();
  getch ();
  endwin ();
}
