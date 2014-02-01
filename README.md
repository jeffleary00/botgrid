BOTGRID README
==============

A simple C program for tracking object on a small moveable grid. 
Designed for simple robots.

This is still under heavy development, and is not ready for use.

NOTES
=====
The grid is built and manipulated at a bit level to minimize memory usage,
but care must still be taken to ensure your platform has enough memory.
 
The amount of memory used by the object depends on the size of the grid. 
 
Be aware that during grid shifts a temporary copy of the grid is needed
in order to perform the transformations. 
 
So, to calculate MAX memory needed by a grid:
 
(((X axis * Y axis)/byte-size) + 1) * 2 + ((sizeof char * 2) + sizeof short) 
 
For examples, (assuming bit sizes: char = 8, short = 16);
  - A 9x9 grid would use 26 bytes of memory.
  - A 27x27 grid would use 192 bytes of memory.
  - A 81x81 grid would use 1646 bytes of memory.
