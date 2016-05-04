#ifndef CONTROLLER_H
#define CONTROLLER_H 

#include "stm32f4xx.h"
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

#define FL_THRESHOLD 500
#define FR_THRESHOLD 500
#define DL_THRESHOLD 600
#define DR_THRESHOLD 600

#define ACCELERATION 1


void systick(void);

void button1_interrupt(void);

void button2_interrupt(void);

void Controller_hardware_init(void );

void Controller_run(int left_distance, int right_distance, int left_speed, int right_speed);

byte Controller_check_walls(void);
byte Controller_check_right_wall(void);
byte Controller_check_left_wall(void);

#endif
