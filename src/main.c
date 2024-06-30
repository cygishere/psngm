#include <assert.h>
#include <curses.h>
#include <stdbool.h>

#include "agent.h"

static const int psn_room_length = 8;

struct game
{
  int player_controled_agent_id; /* -1 for no agent, 0 for a, 1 for b */
  int agent_pos[2];
  bool is_display_room;
};

void move_left (struct game *game, int agent_id);
void move_right (struct game *game, int agent_id);
void sense (struct game);

void print_room_wall (int length);
void print_room_space (int length);
void print_room (struct game game);

void print_agent_senses (struct game game, struct agent_senses sense);

int
main (void)
{
  initscr ();
  raw ();
  noecho ();
  keypad (stdscr, TRUE);
  curs_set (0);

  struct game game = { 0 };

  int ch;
  while (1)
    {
      /* Fill agent sense */
      struct agent_senses sense = { 0 };
      sense.agent_id = game.player_controled_agent_id;
      sense.left_obj = game.agent_pos[sense.agent_id] == 0 ? '#' : '.';
      sense.right_obj
          = game.agent_pos[sense.agent_id] == psn_room_length - 1 ? '#' : '.';

      /* Render */
      if (game.is_display_room)
        {
          print_room (game);
        }
      else
        {
          print_agent_senses (game, sense);
        }
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
        case 't':
          game.is_display_room = !game.is_display_room;
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
    }

quit_game:
  endwin ();
}

void
move_left (struct game *game, int agent_id)
{
  if (game->agent_pos[agent_id] == 0)
    {
      return;
    }
  game->agent_pos[agent_id]--;
  return;
}

void
move_right (struct game *game, int agent_id)
{
  if (game->agent_pos[agent_id] == psn_room_length - 1)
    {
      return;
    }
  game->agent_pos[agent_id]++;
  return;
}

void
print_room (struct game game)
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

  mvaddch (1, game.agent_pos[0] + 1, 'a');
  mvaddch (3, game.agent_pos[1] + 1, 'b');

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

void
print_agent_senses (struct game game, struct agent_senses sense)
{
  clear ();
  printw ("agent id: %d", sense.agent_id);
  move (1, 0);
  printw ("left obj: '%c'", sense.left_obj);
  move (2, 0);
  printw ("right obj: '%c'", sense.right_obj);
  move (3, 0);
  printw ("heard scream: %s", sense.heard_scream ? "yes" : "no");
}
