#include <stdio.h>


#ifndef _BOTGRID_H_INCLUDED
#define _BOTGRID_H_INCLUDED

#define FUNC_OK 0
#define FUNC_FAIL -1

 
/* object type definitions */
struct _botGrid 
{
    unsigned char axis;         /* length of one axis of the grid */
    unsigned short size;        /* size of (axis * axis) */ 
    unsigned char blocks;       /* number of char blocks the grid uses */ 
    unsigned char **grid;
};
typedef struct _botGrid botGrid;



/* function prototypes */
botGrid * 
new_botgrid (unsigned char axis_sz);

int 
botgrid_flush (botGrid *obj);

int 
botgrid_is_empty (botGrid *obj);

unsigned char 
botgrid_get (botGrid *obj, unsigned char x, unsigned char y);

int 
botgrid_set (botGrid *obj, unsigned char x, unsigned char y);

void 
botgrid_print (botGrid *obj);

int 
botgrid_shift (botGrid *obj, char xaxis, char yaxis, int rotation);



/* 'primitive' function prototypes (private) */
int 
botgrid_pget (unsigned char *grid, unsigned char axis, unsigned char x, unsigned char y);

void 
botgrid_pset (unsigned char *grid, unsigned char axis, unsigned char x, unsigned char y);

void 
botgrid_pprint (unsigned char *grid, unsigned char axis);


#endif