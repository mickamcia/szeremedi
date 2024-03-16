#include <stdlib.h>
#include <stdio.h>

#include "../lib/api.h"
#include "../lib/utils.h"

int main(){
    api_init();
    api_setup(10, 4, 20);
    int who_finished = 0;
    int move_count = 0;
    while(who_finished == 0){
        int move = api_think();
        api_move(move);
        utils_print_state();
        if(move_count++ > 25) break;
    }
        
    api_clean();
    api_finish();
}