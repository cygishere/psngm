#ifndef AGENT_H
#define AGENT_H

#include <stdbool.h>

#include "int.h"

enum agent_action_move
{
  AAM_SKIP = 0,
  AAM_RIGHT,
  AAM_LEFT
};

enum agent_action_scream
{
  AAS_SKIP = 0,
  AAS_SCREAM
};

struct agent_action
{
  enum agent_action_move move;
  enum agent_action_scream scream;
};

struct agent_senses
{
  char left_obj;
  char right_obj;
  bool heard_scream;
};

#endif /* AGENT_H */
