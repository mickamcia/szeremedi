#ifndef ENGINE_H
#define ENGINE_H

typedef unsigned long long int U64;
typedef signed long long int S64;

constexpr int MAX_NUMBER = 64;
constexpr int WIN_AWARD = 1e7;
constexpr int INT_MAX = +WIN_AWARD;
constexpr int INT_MIN = -WIN_AWARD;

constexpr inline auto __set_bit(const U64 word, const U64 index) noexcept -> U64
{
  return word | (1ULL << index);
}

constexpr inline auto __get_bit(const U64 word, const U64 index) noexcept -> U64
{
  return word & (1ULL << index);
}

constexpr inline auto __pop_bit(const U64 word, const U64 index) noexcept -> U64
{
  return word & (~(1ULL << index));
}

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

int init();
int finish();

int setup(int, int, int);
int clean();

void set_state(State);
State get_state();

int move(int);
int check_who_won();

int think();

void print_state();
void print_possible_moves();
void make_random_move();

#endif