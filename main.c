#include <stdio.h>
#include "botgrid.h"

#define GRID_SZ 27


int main (void)
{
    botGrid *myobj = new_botgrid(GRID_SZ);
    if (myobj == NULL)
        return 1;
        
    botgrid_set(myobj, 5, 2);
    botgrid_set(myobj, 13, 7);
    botgrid_set(myobj, 11, 22);
    botgrid_print(myobj);
    
    botgrid_shift(myobj, -2, 0, 0);
    botgrid_print(myobj);
    
    botgrid_shift(myobj, 0, -1, 0);
    botgrid_print(myobj);
    
    botgrid_shift(myobj, 0, 0, 90);
    botgrid_print(myobj);
    
    botgrid_shift(myobj, 0, 0, -90);
    botgrid_print(myobj);
    
    return 0;
}