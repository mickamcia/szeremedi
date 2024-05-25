#include "engine.h"
#include <iostream>
#include <chrono>
#include <bitset>
#include <cmath>

State _state;
Settings _settings;

int check_if_black_won(){
  for (U64 i = 0; i < _settings.m; ++i) {
    if (not __get_bit(_state.black, i)) continue;
      for (U64 j = i + 1; j < _settings.m; ++j) {
        if (not __get_bit(_state.black, j)) continue;
          U64 diff = j - i;
          U64 k = j + diff;
          U64 count = 2ULL;
          while (__get_bit(_state.black, k)) {
            count++;
            k = k + diff;
            if (count >= _settings.k) {
              return 1;
          }
        }
      }
    }
  return 0;
}

int check_if_white_won(){
  for (U64 i = 0; i < _settings.m; ++i) {
    if (not __get_bit(_state.white, i)) continue;
    for (U64 j = i + 1; j < _settings.m; ++j) {
      if (not __get_bit(_state.white, j)) continue;
      U64 diff = j - i;
      U64 k = j + diff;
      U64 count = 2ULL;
      while (__get_bit(_state.white, k)) {
        count++;
        k = k + diff;
        if (count >= _settings.k) {
          return 1;
        }
      }
    }
  }
  return 0;
}

S64 evaluate_pass(U64 set, U64 mask){
  S64 score = 0;
  U64 spree_length = 0;
  U64 unblocked_length = 0;
  for(U64 index = 0; index < _settings.m; index++)
  {
    if(__get_bit(mask, index))
    {
      unblocked_length = 0;
      spree_length = 0;
    }
    else{
      unblocked_length++;
      if(__get_bit(set, index)){
        spree_length++;
      }
      else{
        spree_length = 0;
      }
    }
    score += unblocked_length;
    if(unblocked_length >= _settings.k){
      score += spree_length * spree_length * spree_length * spree_length;
    }
    if(spree_length >= _settings.k){
      score += WIN_AWARD;
    }
  }
  spree_length = 0;
  unblocked_length = 0;
  for(U64 index = _settings.m - 1; index < _settings.m; index--) // will overflow and stop
  {
    if(__get_bit(mask, index))
    {
      unblocked_length = 0;
      spree_length = 0;
    }
    else{
      unblocked_length++;
      if(__get_bit(set, index)){
        spree_length++;
      }
      else{
        spree_length = 0;
      }
    }
    score += unblocked_length;
    if(unblocked_length >= _settings.k){
      score += spree_length * spree_length * spree_length * spree_length;
    }
    if(spree_length >= _settings.k){
      score += WIN_AWARD;
    }
  }
  return score;
}

std::pair<U64, U64> evaluate_masked(U64 white, U64 black, U64 mask)
{
  U64 white_block = (black | mask) & (~white);
  U64 black_block = (white | mask) & (~black);
  // std::cout << std::bitset<64>(white) << std::endl;
  // std::cout << std::bitset<64>(white_block) << std::endl;
  // std::cout << std::bitset<64>(black) << std::endl;
  // std::cout << std::bitset<64>(black_block) << std::endl;
  // std::cout << evaluate_pass(white, white_block) << " " << evaluate_pass(black, black_block) << std::endl << std::endl;
  return {evaluate_pass(white, white_block), evaluate_pass(black, black_block)};
}

S64 evaluate(){
  std::pair<U64, U64> scores = {};
  for(U64 step = 1; step <= _settings.m / _settings.k; step++)
  {
    for(U64 index = 0; index < step; index++)
    {
      U64 white = 0ULL;
      U64 black = 0ULL;
      U64 mask = ~0ULL;
      for(U64 i = index; i < _settings.m; i += step){
        white <<= 1ULL;
        black <<= 1ULL;
        mask <<= 1ULL;
        if(__get_bit(_state.white, i)){
          white |= 1ULL;
        }
        if(__get_bit(_state.black, i)){
          black |= 1ULL;
        }
        if(not __get_bit(_state.set, i)){
          mask |= 1ULL;
        }
      }
      // std::cout << step << " " << index << std::endl;
      // std::cout << std::bitset<64>(white) << std::endl;
      // std::cout << std::bitset<64>(black) << std::endl;
      // std::cout << std::bitset<64>(mask) << std::endl;
      // std::cout << std::endl;
      auto temp = evaluate_masked(white, black, mask);
      scores.first += temp.first;
      scores.second += temp.second;
    }
  }
  if(_state.to_move == ToMove::BLACK){
    if(scores.second >= WIN_AWARD){
      return -scores.second;
    }
    else{
      return scores.first - scores.second;
    }
  }
  else{
    if(scores.first >= WIN_AWARD){
      return scores.first;
    }
    else{
      return scores.first - scores.second;
    }
  }
}

auto check_if_move_exists() -> int
{
  return _state.set != 0ULL;
}


auto init() -> int
{
  srand((int)time(NULL));
  return 0;
}

auto finish() -> int
{
  return 0;
}

auto setup(int k, int x, int m) -> int
{
  if(k < 3 || x < 6 || m < 6 || x > m || m > MAX_NUMBER) return -1;
  _settings.k = k;
  _settings.x = x;
  _settings.m = m;

  _state.set = 0ULL;
  _state.white = 0ULL;
  _state.black = 0ULL;
  _state.to_move = ToMove::WHITE;

  int count = 0;
  while(count < x){
    int num = rand() % m;
    if(not __get_bit(_state.set, num)){
      _state.set = __set_bit(_state.set, num);
      count++;
    }
  }
  return 0;
}

auto clean() -> int
{
  return 0;
}

auto set_state(State s) -> void
{
  _state = s;
}

auto get_state() -> State
{
  return _state;
}

auto move(int mv) -> int
{
  auto move = static_cast<U64>(mv);
  _state.set = __pop_bit(_state.set, move);
  switch (_state.to_move)
  {
  case ToMove::WHITE:
    _state.white = __set_bit(_state.white, move);
    _state.to_move = ToMove::BLACK;
    break;
  case ToMove::BLACK:
    _state.black = __set_bit(_state.black, move);
    _state.to_move = ToMove::WHITE;
    break;
  default:
    return 1;
  }
  // std::cout << "SCORE " << evaluate() << std::endl;
  return 0;
}

auto take_back(int mv) -> int
{
  auto move = static_cast<U64>(mv);
  _state.set = __set_bit(_state.set, move);
  switch (_state.to_move)
  {
  case ToMove::WHITE:
    _state.black = __pop_bit(_state.black, move);
    _state.to_move = ToMove::BLACK;
    break;
  case ToMove::BLACK:
    _state.white = __pop_bit(_state.white, move);
    _state.to_move = ToMove::WHITE;
    break;
  default:
    return 1;
  }
  return 0;
}

auto check_who_won() -> int
{
  if(check_if_white_won()){
    return static_cast<int>(Result::WHITE);
  }
  if(check_if_black_won()){
    return static_cast<int>(Result::BLACK);
  }
  if(not check_if_move_exists()){
    return static_cast<int>(Result::DRAW);
  }
  return static_cast<int>(Result::PLAY);
}

auto __check_who_won() -> Result
{
  if(check_if_white_won()){
    return Result::WHITE;
  }
  if(check_if_black_won()){
    return Result::BLACK;
  }
  if(not check_if_move_exists()){
    return Result::DRAW;
  }
  return Result::PLAY;
}

auto alpha_beta(int* best_move, int depth, int alpha, int beta, ToMove maximizing_player) -> int
{
  auto result = __check_who_won();
  if (depth == 0 || __check_who_won() != Result::PLAY) {
    return evaluate();
  }

  if (maximizing_player == ToMove::WHITE) {
    int value = std::numeric_limits<int>::min();
    for(U64 i = 0; i < _settings.m; i++){
      if(__get_bit(_state.set, i)){
        move(i);
        int score = alpha_beta(NULL, depth - 1, alpha, beta, ToMove::BLACK);
        take_back(i);
        value = std::max(value, score);
        alpha = std::max(alpha, value);
        if (alpha >= beta) {
          break;
        }
      }
    }
    return value;
  } 
  else {
    int value = std::numeric_limits<int>::max();
    for(U64 i = 0; i < _settings.m; i++){
      if(__get_bit(_state.set, i)){
        move(i);
        int score = alpha_beta(NULL, depth - 1, alpha, beta, ToMove::WHITE);
        // if(best_move) printf("\t%2lld %d\n", i, score);
        take_back(i);
        if (score < value){
          value = score;
          if(best_move){
            *best_move = static_cast<int>(i);
          }
        }
        value = std::min(value, score);
        beta = std::min(beta, value);
        if (alpha >= beta) {
          break;
        }
      }
    }
    return value;
  }
}

auto think() -> int
{
  auto start_time = std::chrono::steady_clock::now();
  int best_move = -1;
  int depth = 0;
  while(duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() <= 1){
    int score = alpha_beta(&best_move, ++depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), _state.to_move);
    // printf("Depth: %2d\tMove: %2d\tScore: %d\n", depth, best_move, score);
    if(depth >= MAX_NUMBER) break;
  }
  // printf("CURR SCORE: %d", evaluate());
  if(best_move == -1){
    for(U64 i = 0; i < _settings.m; i++){
      if(__get_bit(_state.set, i)){
        return i;
      }
    }
  }
  return best_move;
}

auto print_state() -> void
{
  printf("\nALL:  ");
  for(U64 i = 0; i < _settings.m; i++){
    if(__get_bit(_state.set, i)){
      printf("% 4lld", i);
    }
    else{
      printf("    ");
    }
  }
  printf("\nWHITE:");
  for(U64 i = 0; i < _settings.m; i++){
    if(__get_bit(_state.white, i)){
      printf("% 4lld", i);
    }
    else{
      printf("    ");
    }
  }
  printf("\nBLACK:");
  for(U64 i = 0; i < _settings.m; i++){
    if(__get_bit(_state.black, i)){
      printf("% 4lld", i);
    }
    else{
      printf("    ");
    }
  }
  printf("\n\n");
}

auto print_possible_moves() -> void
{

}

auto make_random_move() -> void
{

}
