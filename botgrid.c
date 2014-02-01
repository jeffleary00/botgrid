#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "botgrid.h"


botGrid * 
new_botgrid (unsigned char axis_sz)
/*
 * Instantiate a new botgrid 'object'
 *
 * PARAMS:
 *  Unsigned char (0 - 255). The size of 1 axis length of the grid.
 *	Recommended value = 27 (see notes)
 *
 * RETURNS:
 *  A botGrid object on success,
 *  NULL on failure.
 *
 *
 * NOTES:
 * 	The axis length should ideally always be a power of 3.
 *		3, 9, 27, 81, 245... 
 *  Minimum size should be 9, Recommended is 27.
 *
 * 	Total memory usage for the grid itself will be;
 *		((axis * axis) * Copies) * sizeof unsigned char
 *
 *	The value of Copies will be 2 during grid rotations and movements since
 *	a duplicate copy of the grid must be created to do the transforms.
 *	Memory of the copied grid is freed as soon as the transform is done, but 
 *	extra memory requirement needs to be taken into consideration just the same.
 *
 *  Because of these requirements, most microcontrollers probably do not even 
 *  have enough memory to handle a grid larger than 81 x 81. Even this value may
 *	be too much, so a recommended value is probably 27.
 *
 */
{   
    botGrid *obj;
    
    if (! axis_sz)
    {
        return NULL;
    }
    
    obj = malloc(sizeof(botGrid));
    if (obj == NULL)
    {
        return NULL;
    }
    
    obj->axis = axis_sz;
    obj->size = (axis_sz * axis_sz);
    obj->blocks = obj->size/8;	
    if (obj->size % 8)
    { 
        obj->blocks ++;
    }
    
    obj->grid = malloc(sizeof(unsigned char) * 1);
    if (obj->grid == NULL) 
    {
        free(obj); 
        return NULL;
    }
    
    if (botgrid_flush(obj) != FUNC_OK)
    {
        free(obj->grid);
        free(obj);
        return NULL;
    }
    
    return obj;
}


int
botgrid_flush (botGrid *obj)
/*
 * Set all elements in the grid to 0
 *
 * PARAMS:
 *  A botgrid object pointer
 *
 * RETURNS:
 *  FUNC_OK, or FUNC_FAIL
 *
 */
{
    unsigned char i;
    unsigned char *tmp;
    
    if (*(obj->grid) != NULL)
    {
        free(*(obj->grid));
    }
    
    tmp = malloc(sizeof(unsigned char) * obj->blocks);
    if (tmp == NULL)
    { 
        return FUNC_FAIL;
    }
    
    for (i = 0; i < obj->blocks; i ++)
    {
        tmp[i] = 0;
    }
    *(obj->grid) = tmp;
    
    return FUNC_OK;    
}


int
botgrid_is_empty (botGrid *obj)
/*
 * Return boolean true if all elements in grid are 0
 *
 * PARAMS:
 *  A botgrid object pointer
 *
 * RETURNS:
 *  1 on true, 0 on false
 *
 */
{
    unsigned char i;
    
    for (i = 0; i < obj->blocks; i ++)
    {
        if (*(obj->grid + i) > 0)
        {
            return 0;
        }
    }
    
    return 1;    
}


unsigned char 
botgrid_get (botGrid *obj, unsigned char x, unsigned char y)
/*
 * Get value of an x-y coordinate in the grid
 *
 * PARAMS:
 *  A botgrid object pointer.
 *	An X coord value
 *	A Y coord value
 *
 * RETURNS:
 *  an unsigned char, 0 or 1
 *
 */
{
    return botgrid_pget(*(obj->grid), obj->axis, x, y);
}


void 
botgrid_set (botGrid *obj, unsigned char x, unsigned char y)
/*
 * Set value of an x-y coordinate in the grid
 *
 * PARAMS:
 *  A botgrid object pointer.
 *	An X coord value
 *	A Y coord value
 *
 * RETURNS:
 *  void
 *
 */
{
    botgrid_pset(*(obj->grid), obj->axis, x, y);
}


void 
botgrid_print (botGrid *obj)
{
    botgrid_pprint(*(obj->grid), obj->axis);
}


int 
botgrid_shift (botGrid *obj, char xaxis, char yaxis, int rotation)
{
    unsigned int change = 0;
    unsigned int i;
    unsigned char *tmp;
    
    /* nothing to do? */
    if (xaxis == 0 && yaxis == 0 && rotation == 0)
    {
        return FUNC_OK;
    }
    
    /* empty temp grid for transposing moves */
    tmp = malloc(sizeof(unsigned char) * obj->blocks);
    if (tmp == NULL)
    {
        return FUNC_FAIL;
    } else { 
        for (i = 0; i < obj->blocks; i ++)
        {
            tmp[i] = 0;
        }
    }
       
    for (i = 0; i < obj->size; i++)
    {
        unsigned char x = 0;
        unsigned char y = 0;
        unsigned char xx = 0;
        unsigned char yy = 0;
        char xoffs = 0;
        char yoffs = 0;
        double rad = 0.0;
        
        if (i != 0) 
        {
            x = (i % obj->axis);
            y = (i / obj->axis);    
        }
        
        if (botgrid_get(obj, x, y))
        {
            rad = rotation * ((atan(1.0) * 4)/180.0);
            xoffs = (x - (obj->axis/2)) + xaxis;    /* offset from x-center */
            yoffs = (y - (obj->axis/2)) + yaxis;    /* offset from y-center */
            
            /* calculate new x-y points */
            xx = (xoffs * cos(rad) - yoffs * sin(rad)) + (obj->axis/2);
            yy = (xoffs * sin(rad) + yoffs * cos(rad)) + (obj->axis/2);
            
            /* new points are within bounds of our grid? */ 
            if (xx >= 0 && xx < obj->axis && yy >= 0 && yy < obj->axis)
            {
                botgrid_pset(tmp, obj->axis, xx, yy);
                change ++;
            }
        }
    }
    
    /* free old grid and overwrite it with new one */
    if (change) 
    {
        free(*(obj->grid));
        *(obj->grid) = tmp;
    } else {
        free(tmp);
    }
    
    return FUNC_OK;
}


int 
botgrid_pget (unsigned char *grid, unsigned char axis, unsigned char x, unsigned char y)
{
    int index = (y * axis) + x;
    int n = 7;
    int i = 0;
    
    if (index != 0)
    {
        i = index / 8;
        n = n - (index % 8);
    }
    
    if (grid[i] & (1 << n))
    {
        return 1;
    }
       
    return 0;
}


void 
botgrid_pset (unsigned char *grid, unsigned char axis, unsigned char x, unsigned char y)
{
    int index = (y * axis) + x;
    int n = 7;
    int i = 0;
    
    if (index != 0)
    {
        i = index / 8;
        n = n - (index % 8);
    }
    
    grid[i] = (grid[i] | (1 << n));
}


void 
botgrid_pprint (unsigned char *grid, unsigned char axis)
{
    int i;
    unsigned int size = (axis * axis);
    
    for (i = 0; i < size; i++) {    
        int x = 0;
        int y = 0;
        
        if (i != 0) 
        {
            x = (i % axis);
            y = (i / axis);    
        }
        
        if (! (i % axis))
        {
            printf("\n");    
        }
        
        if (x == (axis/2) && y == (axis/2))
        {
            printf("O ");
        } else {
            if (botgrid_pget(grid, axis, x, y))
            {
                printf("X ");
            } else {
                printf(". ");    
            }
        }
    }
    printf("\n");    
}


/*      


GRID:

        |       |
    s1  |  s2   |  s3
        |       |
  ------+-------+------
        |       |
    s4  |  s5*  |  s6
        |       |
  ------+-------+------
        |       |
    s7  |  s8   |  s9
        |       |

        
*/