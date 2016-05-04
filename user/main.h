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

#define START_DIRECTION 0

typedef unsigned char byte;


// Global variables
extern byte current_direction_global;
extern byte current_position_global;
extern int current_location_global[2];

#endif
