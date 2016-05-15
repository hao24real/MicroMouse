#ifndef CONTROLLER_H
#define CONTROLLER_H 

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_it.h"
#include "delay.h"
#include "led.h"
#include "button.h"
#include <stdio.h>
#include "usart.h"
#include "SPI.h"
#include "matrixDisplay.h"
#include "pwm.h"
#include "encoder.h"
#include "buzzer.h"
#include "main.h"
#include "sensor_Function.h"
#include "adc.h"
#include "pid.h"


#define ENABLE 1
#define DISABLE 0


#define FL_THRESHOLD 240
#define FR_THRESHOLD 320
#define DL_THRESHOLD 300
#define DR_THRESHOLD 250



extern byte PID_EN;
extern byte MODE;


void systick(void);

void button1_interrupt(void);

void button2_interrupt(void);

void Controller_hardware_init(void );

void Controller_run(int left_distance, int right_distance, int left_speed, int right_speed);

void Controller_maze_calibrate(void);

void Controller_frontwall_corecttion(void);

byte Controller_check_walls(void);
byte Controller_check_right_wall(void);
byte Controller_check_left_wall(void);
byte Controller_check_front_wall(void);
	
#endif
