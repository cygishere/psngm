#include <assert.h>
#include <curses.h>
#include <limits.h>
#include <stdbool.h>

#include "agent.h"
#include "int.h"
#include "map.h"
#include "mem.h"

static const u8 type_what = 0x00;
static const u8 type_wall = 0x01;
static const u8 type_empty = 0x02;

static const int psn_room_length = 8;

enum game_display_mode
{
  GDM_MEMORY = 0,
  GDM_SENSES,
  GDM_ROOM
};

struct game
{
  int player_controled_agent_id; /* -1 for no agent, 0 for a, 1 for b */
  struct map map;
  struct mem8 mem_agents[2];
  struct agent_action act_agents[2];
  int noise_level_cur;  /* 0 for no scream, 1 for 1 agent scream and so on */
  int noise_level_next; /* 0 for no scream, 1 for 1 agent scream and so on */
  enum game_display_mode display_mode;
};

void move_left (struct game *game, int agent_id);
void move_right (struct game *game, int agent_id);
void scream (struct game *game, int agent_id);
void sense (struct game *game, int agent_id);

void print_room (struct game game);

void print_agent_memory (struct mem8 mem);
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
      /* Clear agent move action */
      for (int i = 0; i < 2; ++i)
        {
          game.act_agents[i].move = AAM_SKIP;
        }
      /* Set some game state */
      game.noise_level_cur = game.noise_level_next;
      game.noise_level_next = 0;
      /* Fill agent sense into its memory */
      sense (&game, game.player_controled_agent_id);
      /* Render */
      switch (game.display_mode)
        {
        case GDM_MEMORY:
          print_agent_memory (game.mem_agents[game.player_controled_agent_id]);
          break;
        case GDM_SENSES:
          {
            /* u8 mem = game.mem_agents[game.player_controled_agent_id].bits;
             */
            /* struct agent_senses sense = { 0 }; */

            /* print_agent_senses (sense); */
          }
          break;
        case GDM_ROOM:
          print_room (game);
          break;
        default:
          break;
        }
      /* Get user input */
      {
        struct agent_action *act
            = &game.act_agents[game.player_controled_agent_id];
        ch = getch ();
        switch (ch)
          {
          case KEY_LEFT:
          case 'h':
            act->move = AAM_LEFT;
            break;
          case KEY_RIGHT:
          case 'l':
            act->move = AAM_RIGHT;
            break;
          case 'n':
            act->scream = (act->scream == AAS_SKIP ? AAS_SCREAM : AAS_SKIP);
            break;
          case 't':
            {
              if (game.display_mode == GDM_ROOM)
                {
                  game.display_mode = GDM_MEMORY;
                }
              else
                {
                  game.display_mode = GDM_ROOM;
                }
            }
            break;
          case 'q':
            goto quit_game;
          }
      }
      /* Programmed agents decide action */

      /* Game take action */
      for (int i = 0; i < 2; ++i)
        {
          struct agent_action *act = &game.act_agents[i];
          switch (act->move)
            {
            case AAM_RIGHT:
              move_right (&game, i);
              break;
            case AAM_LEFT:
              move_left (&game, i);
              break;
            default:
              break;
            }
          switch (act->scream)
            {
            case AAS_SCREAM:
              scream (&game, i);
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
scream (struct game *game, int agent_id)
{
  game->noise_level_next += 1;
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
  printw ("left obj: '%c'", sense.left_obj ? sense.left_obj : ' ');
  move (1, 0);
  printw ("right obj: '%c'", sense.right_obj ? sense.right_obj : ' ');
  move (2, 0);
  printw ("heard scream: %s", sense.heard_scream ? "yes" : "no");
}

void
sense (struct game *game, int agent_id)
{
  uint x = game->map.pos_agent[agent_id].x;
  uint y = game->map.pos_agent[agent_id].y;
  uint z = game->map.pos_agent[agent_id].z;
  char left_obj = map_get_content_at (
      game->map, (struct pos){ .x = x - 1, .y = y, .z = z });
  char right_obj = map_get_content_at (
      game->map, (struct pos){ .x = x + 1, .y = y, .z = z });

  u8 m = 0x00;
  switch (left_obj)
    {
    case 0:
      m = (m & 0x3f) | (type_empty << 6);
      break;
    case '#':
      m = (m & 0x3f) | (type_wall << 6);
      break;
    default:
      m = (m & 0x3f) | (type_what << 6);
      break;
    }
  switch (right_obj)
    {
    case 0:
      m = (m & 0xcf) | (type_empty << 4);
      break;
    case '#':
      m = (m & 0xcf) | (type_wall << 4);
      break;
    default:
      m = (m & 0xcf) | (type_what << 4);
      break;
    }
  if (game->noise_level_cur > 0)
    {
      m = (m & 0xfb) | (0x01 << 3);
    }
  else
    {
      m = (m & 0xfb) | (0x00 << 3);
    }
  game->mem_agents[agent_id].bits = m;
}

void
print_agent_memory (struct mem8 mem)
{
  clear ();
  printw ("%08b", mem.bits);
}
