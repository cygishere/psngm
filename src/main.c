#include <assert.h>
#include <curses.h>
#include <limits.h>
#include <stdbool.h>

#include "agent.h"
#include "int.h"
#include "map.h"

static const int psn_room_length = 8;

struct game
{
  int player_controled_agent_id; /* -1 for no agent, 0 for a, 1 for b */
  struct map map;
  bool is_display_room;
};

void move_left (struct game *game, int agent_id);
void move_right (struct game *game, int agent_id);
void sense (struct game);

void print_room (struct game game);

void print_agent_senses (struct agent_senses sense);

int
main (void)
{
  initscr ();
  raw ();
  noecho ();
  keypad (stdscr, TRUE);
  curs_set (0);

  struct game game = { 0 };
  game.map = map_new (psn_room_length + 2);

  int ch;
  while (1)
    {
      /* Fill agent sense */
      struct agent_senses sense = { 0 };
      {
        int id = game.player_controled_agent_id;
        uint x = game.map.pos_agent[sense.agent_id].x;
        uint y = game.map.pos_agent[sense.agent_id].y;
        uint z = game.map.pos_agent[sense.agent_id].z;
        sense.agent_id = id;
        sense.left_obj = map_get_content_at (
            game.map, (struct pos){ .x = x - 1, .y = y, .z = z });
        sense.right_obj = map_get_content_at (
            game.map, (struct pos){ .x = x + 1, .y = y, .z = z });
      }
      /* Render */
      if (game.is_display_room)
        {
          print_room (game);
        }
      else
        {
          print_agent_senses (sense);
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
  map_del (game.map);
  endwin ();
}

void
move_left (struct game *game, int agent_id)
{
  struct pos *pos = &(game->map.pos_agent[agent_id]);
  if ((*pos).x == 1)
    {
      return;
    }
  map_set_content_at (&game->map, *pos, 0);
  (*pos).x--;
  map_set_content_at (&game->map, *pos, 'a');

  return;
}

void
move_right (struct game *game, int agent_id)
{
  struct pos *pos = &(game->map.pos_agent[agent_id]);
  if ((*pos).x == game->map.length - 2)
    {
      return;
    }
  map_set_content_at (&game->map, *pos, 0);
  (*pos).x++;
  map_set_content_at (&game->map, *pos, 'a');
  return;
}

void
print_room (struct game game)
{
  clear ();

  assert (game.map.width <= INT_MAX && "game.map.width cannot cast to int");
  assert (game.map.length <= INT_MAX && "game.map.length cannot cast to int");

  for (uint z = 0; z < game.map.height; ++z)
    {
      for (uint y = 0; y < game.map.width; ++y)
        {
          for (uint x = 0; x < game.map.length; ++x)
            {
              chtype c = (chtype)map_get_content_at (
                  game.map, (struct pos){ .x = x, .y = y, .z = z });
              if (c)
                {
                  mvaddch ((int)y, (int)x, c);
                }
            }
        }
    }

  refresh ();
}

void
print_agent_senses (struct agent_senses sense)
{
  clear ();
  printw ("agent id: %d", sense.agent_id);
  move (1, 0);
  printw ("left obj: '%c'", sense.left_obj ? sense.left_obj : ' ');
  move (2, 0);
  printw ("right obj: '%c'", sense.right_obj ? sense.right_obj : ' ');
  move (3, 0);
  printw ("heard scream: %s", sense.heard_scream ? "yes" : "no");
}
