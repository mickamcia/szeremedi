#ifndef STRUCTURES_H
#define STRUCTURES_H

extern "C"
{
  typedef unsigned long long int U64;
  typedef signed long long int S64;

  enum class Result
  {
    BLACK,
    WHITE,
    DRAW,
    PLAY,
  };

  enum class ToMove
  {
    BLACK,
    WHITE,
  };

  struct State_t
  {
    U64 set;
    U64 white;
    U64 black;
    ToMove to_move;
  };

  struct Settings_t
  {
    U64 k;
    U64 x;
    U64 m;
  };

  typedef struct State_t State;
  typedef struct Settings_t Settings;
}

#endif