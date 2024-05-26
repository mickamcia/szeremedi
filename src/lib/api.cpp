#include "api.h"
#include "engine.h"

int api_init(){
  return init();
}

int api_finish(){
  return finish();
}

int api_setup(int k, int x, int m){
  return setup(k, x, m);
}

void api_set_state(ApiState s){
  State internal;
  internal.black = s.black;
  internal.white = s.white;
  internal.set = s.set;
  internal.to_move = static_cast<ToMove>(s.to_move);
  set_state(internal);
}

ApiState api_get_state(){
  auto internal = get_state();
  auto external = ApiState();
  external.black = internal.black;
  external.white = internal.white;
  external.set = internal.set;
  external.to_move = static_cast<int>(internal.to_move);
}

int api_clean(){
  return clean();
}

int api_move(int mv){
  return move(mv);
}

int api_check_who_won(){
  return check_who_won();
}

int api_think(){
  return think();
}