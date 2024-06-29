#ifndef AGENT_H
#define AGENT_H

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
  int agent_id;
  enum agent_action_move move;
  enum agent_action_scream scream;
};

#endif /* AGENT_H */
