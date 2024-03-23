#include <stdlib.h>
#include <stdio.h>

#include "../lib/api.h"
#include "../lib/utils.h"

int main(){
    api_init();
    api_setup(4, 10, 20);
    int curr = PLAY;
    while(curr == PLAY){
        int move = api_think();
        api_move(move);
        utils_print_state();
        curr = api_check_who_won();
    }
        
    api_clean();
    api_finish();
}