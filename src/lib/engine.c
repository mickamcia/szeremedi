#include "engine.h"
#include <stdlib.h>
#include <stdio.h>

state _state;
settings _settings;

int init(){
    return 0;
}

int finish(){
    return 0;
}

int setup(int k, int x, int m){
    _settings.k = k;
    _settings.x = x;
    _settings.m = m;
    for(int i = 0; i < MEM_SIZE; i++){
        _state.set[i] = ((U64)rand() << 32ULL) | (U64)rand();
        _state.set[i] &= ((U64)rand() << 32ULL) | (U64)rand();
        _state.white[i] = 0ULL;
        _state.black[i] = 0ULL;
    }
    return 0;
}

int clean(){
    return 0;
}

void set_state(state s){
    _state = s;
}

state get_state(){
    return _state;
}

int move(int mv){
    pop_bit(_state.set, mv);
    if(_state.to_move == WHITE){
        set_bit(_state.white, mv);
        _state.to_move = BLACK;
    }
    else{
        set_bit(_state.black, mv);
        _state.to_move = WHITE;
    }
    return 0;
}

int check_who_won(){
    return 0;
}

int think(){
    for(int i = 0; i < MAX_NUMBER; i++){
        if(get_bit(_state.set, i)){
            return i;
        }
    }
    return 0;
}

void print_state(){
    printf("\nALL:  ");
    for(int i = 0; i < MAX_NUMBER; i++){
        if(get_bit(_state.set, i)){
            printf("% 4d", i);
        }
    }
    printf("\nWHITE:");
    for(int i = 0; i < MAX_NUMBER; i++){
        if(get_bit(_state.white, i)){
            printf("% 4d", i);
        }
    }
    printf("\nBLACK:");
    for(int i = 0; i < MAX_NUMBER; i++){
        if(get_bit(_state.black, i)){
            printf("% 4d", i);
        }
    }
    printf("\n\n");
}

void print_possible_moves(){

}

void make_random_move(){

}
