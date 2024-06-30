#ifndef MAP_H
#define MAP_H

#include "int.h"

struct pos
{
  uint x;
  uint y;
  uint z;
};

struct map
{
  uint length;
  uint width;
  uint height;
  char *content;
  struct pos pos_agent[2];
};

struct map map_new (uint length);
void map_del (struct map map);
static inline char map_get_content_at (const struct map map, struct pos pos);
static inline void map_set_content_at (struct map *map, struct pos pos,
                                       char c);

char
map_get_content_at (const struct map map, struct pos pos)
{
  return map
      .content[pos.z * map.length * map.width + pos.y * map.length + pos.x];
}

void
map_set_content_at (struct map *map, struct pos pos, char c)
{
  map->content[pos.z * map->length * map->width + pos.y * map->length + pos.x]
      = c;
  return;
}

#endif /* MAP_H */
