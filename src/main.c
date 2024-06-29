#include <assert.h>
#include <curses.h>

static const int psn_room_length = 8;

struct game
{
  int pos_a;
  int pos_b;
};

void move_left (struct game *game);
void move_right (struct game *game);

void print_room_wall (int length);
void print_room_space (int length);
void print_frame (struct game game);

int
main (void)
{
  initscr ();
  raw ();
  noecho ();
  keypad (stdscr, TRUE);
  curs_set (0);

  struct game game = { 0 };

  print_frame (game);

  int ch;
  while (1)
    {
      ch = getch ();
      switch (ch)
        {
        case 'q':
          goto quitpsn;
        case KEY_LEFT:
        case 'h':
          move_left (&game);
          break;
        case KEY_RIGHT:
        case 'l':
          move_right (&game);
          break;
        }

      print_frame (game);
    }

quitpsn:
  endwin ();
}

void
move_left (struct game *game)
{
  if (game->pos_a == 0)
    {
      return;
    }
  game->pos_a--;
  return;
}

void
move_right (struct game *game)
{
  if (game->pos_a == psn_room_length - 1)
    {
      return;
    }
  game->pos_a++;
  return;
}

void
print_frame (struct game game)
{
  clear ();
  print_room_wall (psn_room_length);

  move (1, 0);
  print_room_space (psn_room_length);

  move (2, 0);
  print_room_wall (psn_room_length);

  move (3, 0);
  print_room_space (psn_room_length);

  move (4, 0);
  print_room_wall (psn_room_length);

  mvaddch (1, game.pos_a + 1, 'a');
  mvaddch (3, game.pos_b + 1, 'b');

  refresh ();
}

void
print_room_wall (int length)
{
  addch ('#');
  for (int i = 0; i < length; ++i)
    {
      addch ('#');
    }
  addch ('#');
}

void
print_room_space (int length)
{
  addch ('#');
  for (int i = 0; i < length; ++i)
    {
      addch ('.');
    }
  addch ('#');
}
