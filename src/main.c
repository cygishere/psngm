#include <assert.h>
#include <curses.h>

#include "agent.h"

static const int psn_room_length = 8;

struct game
{
  int player_controled_agent_id; /* -1 for no agent, 0 for a, 1 for b */
  int pos[2];
};

void move_left (struct game *game, int agent_id);
void move_right (struct game *game, int agent_id);

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
      /* Get user input */
      struct agent_action act = { 0 };
      act.agent_id = 0;
      ch = getch ();
      switch (ch)
        {
        case KEY_LEFT:
        case 'h':
          act.move = AAM_LEFT;
          break;
        case KEY_RIGHT:
        case 'l':
          act.move = AAM_RIGHT;
          break;
        case 'n':
          act.scream = (act.scream == AAS_SKIP ? AAS_SCREAM : AAS_SKIP);
          break;
        case 'q':
          goto quit_game;
        }

      /* Programmed agents decide action */

      /* Game take action */
      if (act.agent_id == 0)
        {
          switch (act.move)
            {
            case AAM_RIGHT:
              move_right (&game, act.agent_id);
              break;
            case AAM_LEFT:
              move_left (&game, act.agent_id);
              break;
            default:
              break;
            }
          switch (act.scream)
            {
            case AAS_SCREAM:
              break;
            default:
              break;
            }
        }
      /* Render */
      print_frame (game);
    }

quit_game:
  endwin ();
}

void
move_left (struct game *game, int agent_id)
{
  if (game->pos[agent_id] == 0)
    {
      return;
    }
  game->pos[agent_id]--;
  return;
}

void
move_right (struct game *game, int agent_id)
{
  if (game->pos[agent_id] == psn_room_length - 1)
    {
      return;
    }
  game->pos[agent_id]++;
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

  mvaddch (1, game.pos[0] + 1, 'a');
  mvaddch (3, game.pos[1] + 1, 'b');

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
