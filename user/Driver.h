#ifndef DRIVER_H
#define DRIVER_H

#include "Controller.h"
#include "main.h"

extern	int left_cnt, right_cnt;
extern 	int left_spd, right_spd;
extern	int radius;// = distance;

void Driver_turn_left(int distance, int angle, int speed);

void Driver_turn_right(int distance, int angle, int speed);

void Driver_go_straight(int distance, int speed);

void Driver_frontwall_correction(void);

byte Driver_check_walls(void );

#endif
