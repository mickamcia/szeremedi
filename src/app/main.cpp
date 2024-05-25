#include "../lib/api.h"
#include "utils.h"

#include <iostream>

int main(){
  api_init();
  api_setup(5, 16, 21);
  utils_print_state();
  Result curr = Result::PLAY;
  bool white_to_play = true;
  while(curr == Result::PLAY){
    int move;
    if(white_to_play){
      std::cin >> move;
    }
    else{
      move = api_think();
    }
    api_move(move);
    curr = static_cast<Result>(api_check_who_won());
    white_to_play = not white_to_play;
    utils_print_state();
  }
      
  api_clean();
  api_finish();
}