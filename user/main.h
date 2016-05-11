#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx.h"


// BIT manipulate macros
#define CLR_B(p,n) ((p) &= ~((1) << (n)))
#define SET_B(p,n) ((p) |= ((1) << (n)))
#define READ_B(p,n) ((p) & ((1) << (n)))

#define RIGHT_DIRECT(p) ((p==3)? 0:p+1)
#define LEFT_DIRECT(p) ((p==0)? 3:p-1)
#define BACK_DIRECT(p) ((p==1)? 3:((p==0)? 2:(p-2)))
#define ROTATE_DIRECT(p, d) (((p << d)|(p >> (4-d))) & 0x0F)  

#define M_PI 31415 //x10^(-4)
#define E_4	10000 // 10^4
#define CIRCLE_ANGLE 360
#define CELL_WIDTH 180
#define MOUSE_WIDTH 72	// 65mm from left wheels to right wheels
#define LW_MM2COUNT_RATIO 88  // 1mm = 140 count
#define RW_MM2COUNT_RATIO 88  // 1mm = 140 count

// Direction
#define EAST 0
#define SOUTH 1
#define WEST 2
#define NORTH 3

#define FRONT 0
#define RIGHT 1
#define BACK 2
#define LEFT 3

#define VISITED 4

#define ROW_INDEX 0
#define COLUMN_INDEX 1


#define START_DIRECTION 0

#define MAZE_SIZE 3

#define GENERAL_ARRAY_SIZE 256 


typedef unsigned char byte;


// Global variables
extern byte current_direction_global;
extern byte current_position_global[2];
//bit indicate wall info and visited
extern byte maze_array_global[MAZE_SIZE][MAZE_SIZE];

//path in there
extern byte path_1_global[MAZE_SIZE*MAZE_SIZE];
extern byte general_purpose_array_1[GENERAL_ARRAY_SIZE];
extern byte general_purpose_array_2[GENERAL_ARRAY_SIZE];
extern byte maze_dist_array_global[MAZE_SIZE][MAZE_SIZE];
extern byte path_run_global[MAZE_SIZE*MAZE_SIZE];
#endif
