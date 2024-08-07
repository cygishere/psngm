#include <stdlib.h>

#include "map.h"

struct map
map_new (uint length)
{
  struct map map = { 0 };
  map.length = length;
  map.width = 5;
  map.height = 2;
  map.content = malloc (sizeof *map.content * map.length * map.width);
  map.pos_agent[0] = (struct pos){ .x = 1, .y = 1, .z = 1 };
  map.pos_agent[1] = (struct pos){ .x = 1, .y = 3, .z = 1 };

  for (uint y = 0; y < map.width; ++y)
    {
      for (uint x = 0; x < map.length; ++x)
        {
          map_set_content_at (&map, (struct pos){ .x = x, .y = y, .z = 0 },
                              '.');
        }
    }
  for (uint y = 0; y < map.width; ++y)
    {
      for (uint x = 0; x < map.length; ++x)
        {
          map_set_content_at (&map, (struct pos){ .x = x, .y = y, .z = 1 },
                              '#');
        }
    }
  for (uint x = 1; x < map.length - 1; ++x)
    {
      map_set_content_at (&map, (struct pos){ .x = x, .y = 1, .z = 1 }, 0);
      map_set_content_at (&map, (struct pos){ .x = x, .y = 3, .z = 1 }, 0);
    }
  map_set_content_at (&map, map.pos_agent[0], 'a');
  map_set_content_at (&map, map.pos_agent[1], 'b');

  return map;
}

void
map_del (struct map map)
{
  free (map.content);
}
