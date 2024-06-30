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
static inline char map_get_content_at (const struct map map, uint x, uint y,
                                       uint z);
static inline void map_set_content_at (struct map *map, uint x, uint y, uint z,
                                       char c);

char
map_get_content_at (const struct map map, uint x, uint y, uint z)
{
  return map.content[z * map.length * map.width + y * map.length + x];
}

void
map_set_content_at (struct map *map, uint x, uint y, uint z, char c)
{
  map->content[z * map->length * map->width + y * map->length + x] = c;
  return;
}

#endif /* MAP_H */
