#ifndef PID_H
#define PID_H

#include "pwm.h"
#include "encoder.h"
#include "sensor_Function.h"
#include "delay.h"
#include "Controller.h"

extern int ERR_LEFT_TO_PID;
extern int ERR_RIGHT_TO_PID;

void pid_motor_speed(void);
void front_wall_pid(void);

#endif
