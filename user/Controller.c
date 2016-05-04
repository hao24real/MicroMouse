#include "Controller.h"
#include <stdlib.h>


void Controller_hardware_init(void) {

	//Local variable

	
	Systick_Configuration();
	LED_Configuration();
	//button_Configuration();
	//usart1_Configuration(9600);
	//SPI_Configuration();
  TIM4_PWM_Init();
	Encoder_Configration();
	//buzzer_Configuration();
	ADC_Config();
	
	// Turn off all led
	ALL_LED_OFF;
	
	// Stop both motor
	setLeftPwm(0);	
	setRightPwm(0);
	delay_ms(1000);
}

void Controller_run(int left_distance, int right_distance, int left_speed, int right_speed){
	
	// Declaration of all variables
	int left_EN; // left encoder count
	int right_EN; // right encoder count
	
	if ((left_distance == 0)&&(right_distance == 0)&&(left_speed == 0)&&(right_speed == 0)) {
		setLeftSpeed(0);
		setRightSpeed(0);
		return;
	}
	
	// Read the value
	left_EN = getLeftEncCount();
	right_EN = getRightEncCount();
	
	setLeftSpeed(left_speed);
	setRightSpeed(right_speed);
	
	// This line of code can cause trouble if the encoder rigister is overflow
	while ( (abs(getLeftEncCount() - left_EN) < abs(left_distance))||
					(abs(getRightEncCount() - right_EN) < abs(right_distance))){
		// Read sensor
		readSensor();
		
		// Case 1: Correct position if it see front wall
			if ((FLSensor > FL_THRESHOLD)&&(FRSensor > FR_THRESHOLD)){
				
			}
	}
	
}

byte Controller_check_walls(){
	// Return value
	byte ret_val = 0;
	
	// read all sensor
	readSensor();
	
	if ((FLSensor > FL_THRESHOLD)&&(FRSensor > FR_THRESHOLD))
		SET_B(ret_val, FRONT);
	if (DRSensor > DR_THRESHOLD)
		SET_B(ret_val, RIGHT);
	if (DLSensor > DL_THRESHOLD)
		SET_B(ret_val, LEFT);
	
	// Rotate the last 4 bits according to current direction before return
	return ret_val;  //((ret_val&0x0F)>>(4-current_direction_global))|(ret_val<<current_direction_global);
}

/*
 * This fuction check the right wall only
 */
byte Controller_check_right_wall(void);

/*
 * This function check the left wall only
 */
byte Controller_check_left_wall(void);


void systick(void) {
}

void button1_interrupt(void) {
}


void button2_interrupt(void) {
}
