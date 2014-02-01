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


GRID 
====
resembles something like this. With the robot always exising in the center.

<pre>
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
</pre>

MORE NOTES
==========


Copyright (c) 2012 Jeffrey Leary
Released under Simplified BSD License. See docs/LICENSE for details.

DESCRIPTION:
Implements a simple environment grid for robots. The grid acts as a 
short-term memory map of obstacles the robot has encountered.

SYNOPSIS:
  botGrid *grid = new_botgrid(27);

    // space directly in front of robot is clear?
    // 1 = an obstacle, 0 = no obstacle
    
    if ( ! botgrid_get_front(grid) ) {
        move_robot_forward(1);
        botgrid_shift(grid, 0, 1, 0);
    }

    // The robot rotates RIGHT 90 degrees, so grid should be shifted the 
    // same amount in the OPPSITE direction (LEFT -90, counter clockwise).

    if ( turn_robot_right(90) ) {
        botgrid_shift(grid, 0, 0, -90)
    }

DETAILS:
As a robot moves and rotates, the grid should be shifted accordingly. 
Objects will eventually 'fall off' the grid and be forgotten as the 
robot travels.

MEMORY REQUIREMENTS:
Objects in the grid are stored at a bit level inside of an array of bytes.
To calculate memory usage:

Memory used by the grid object.
 BYTES = (( grid_axis_sizegrid_axis_size ) / 8) + 1

During grid shifts, a temporary grid must be created to do the
This module is intended to be used by other modules, and not by the
user directly.  For safe, Lisp-like lists, see Containers/SLists.h
instead.

KNOWN ISSUES:
Due to the small size of the grids, and the loss of accuracy that occurs
when doing grid rotations, significant distortions can happen when doing
rotations that are not in 90 degree increments. This, combined with the fact
that a real-world robot probably does not make 100% accurate rotations 
either, 
