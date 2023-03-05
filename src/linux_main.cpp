#include "loki_main.hpp"
#include <stdio.h>

int main(int, char**)
{

    loki_init();
    
    // Main loop
    while (1) {
        loki_main();        
    }
    
    loki_destroy();
    return 0;
}
