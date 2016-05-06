/* File name: Controller.c
 * This file contain all method called by Driver
 * Will N.B.
 */
#include "Controller.h"
#include <stdlib.h>

#define KP 1000
#define KI 0
#define KD 0

#define LEFT_WALL_DISTANCE 1035
#define RIGHT_WALL_DISTANCE 1063
#define FRONT_RIGHT_WALL_DISTANCE 3251
#define FRONT_LEFT_WALL_DISTANCE 2759


#define VALID_ERR 15


int LSS;
int RSS;
int FLSS;
int FRSS;


	int RD[500];
	int LD[500];
	
	int FL[500];
	int FR[500];



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
	delay_ms(5000);
}

// POST REQUIREMENT: speed and distance is passed in. NEVER pass in ZERO SPEED and NON_ZERO DISTANCE
void Controller_run(int left_distance, int right_distance, int left_speed, int right_speed){
	
	// Declaration of all variables
	int left_EN; // left encoder count
	int right_EN; // right encoder count
	int ERR;
	int MAX_ERR;
	int temp_cnt = 0; // Software filter for sensor noise

	
	if ((left_distance == 0)&&(right_distance == 0)) {
		setLeftSpeed(left_speed);
		setRightSpeed(right_speed);
		return;
	}
	
	// Save the value in the Encoder counter reg
	left_EN = getLeftEncCount();
	right_EN = getRightEncCount();


	// Set speed of left and right motor
	// Then keep track on the distance
	// Return when finish the distance.
	setLeftSpeed(left_speed);
	setRightSpeed(right_speed);
	
	// Calculate the MAX_ERR to send to PID function
	MAX_ERR = left_speed/10; // 18 before
	
	// This line of code can cause trouble if the encoder rigister is overflow
	// Loop until mice finish the given distance
	// Distance is allway positive, this was calculated from caller (Driver)
	while ( (abs(getLeftEncCount() - left_EN) < (left_distance))||
					(abs(getRightEncCount() - right_EN) < (right_distance))){
	
		// Correct position base on wall sensor infomation
		// Only corret when going straight
		if (left_speed == right_speed){
			
			// check wall infomation
			readSensor();;
				// CASE 1: correct position base on both wall
				// For reliable sensor to correct position. we need to read the cloe value only
			if ((FLSensor > (FL_THRESHOLD+500))&&(FRSensor > (FR_THRESHOLD+500))){
				if (temp_cnt >200){				
					ERR = (DLSensor - DRSensor - (LEFT_WALL_DISTANCE - RIGHT_WALL_DISTANCE));
					// Check if error is valid for correction ( too far from wall)
						if (ERR > (MAX_ERR+5)) ERR = MAX_ERR+5;
						if (ERR < -(MAX_ERR+5)) ERR = -(MAX_ERR+5);	
					ERR_LEFT_TO_PID = ERR;
					ERR_RIGHT_TO_PID = -ERR;
				} else {
					temp_cnt ++;
				}
				
				
				
				// CASE 2: have left wall
			} else	if (DLSensor > (DL_THRESHOLD+500)){
				if (temp_cnt >200){				
					ERR = (DLSensor - LEFT_WALL_DISTANCE)/10;
					// Check if error is valid for correction ( too far from wall)
					if (ERR < -VALID_ERR){
						ERR = 0;
					} else{
						if (ERR > (MAX_ERR+5)) ERR = MAX_ERR+5;
						if (ERR < -(MAX_ERR+5)) ERR = -(MAX_ERR+5);			
					}
					ERR_LEFT_TO_PID = ERR;
					ERR_RIGHT_TO_PID = -ERR;
				} else {
					temp_cnt ++;
				}
				
				
				
				
				// Case 3: Use right wall for correction
			} else if (DRSensor > (DR_THRESHOLD+500)){
				if (temp_cnt >200){				
					ERR = (DRSensor - RIGHT_WALL_DISTANCE)/10;
					// Check if error is valid for correction ( too far from wall)
					if (ERR < -VALID_ERR){
						ERR = 0;
					} else{
						if (ERR > (MAX_ERR+5)) ERR = MAX_ERR+5;
						if (ERR < -(MAX_ERR+5)) ERR = -(MAX_ERR+5);			
					}
					ERR_LEFT_TO_PID = -ERR;
					ERR_RIGHT_TO_PID = ERR;
				} else {
					temp_cnt ++;
				}
			
			} else {
				temp_cnt = 0;
				ERR = 0;
				ERR_LEFT_TO_PID = 0;
				ERR_RIGHT_TO_PID = 0;
			}
		}
						
	}
					
	// Reset ERR after finish one short run
	ERR_LEFT_TO_PID = 0;
	ERR_RIGHT_TO_PID = 0;
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
	
	return ret_val;  
	// Rotate the last 4 bits according to current direction before return
	//((ret_val&0x0F)>>(4-current_direction_global))|(ret_val<<current_direction_global);
}


/*
 * This function adjust the parametter for new maze
 * PROCEDURE:
 * STEP 1: BOTH GREEN LIGHTS IS ON 
 */
void Controller_maze_calibrate(){
	

	
	int count, num;
	int sum;
	
	
	delay_ms(2000);
	
	LED1_ON;
	
		setLeftSpeed(30);
		setRightSpeed(30);
	
	for (count = 0; count < 500; count ++){
	
		readSensor();
		
		LD[count] = DLSensor;
		RD[count] = DRSensor;
		
		delay_ms(10);	
	}
	
	setLeftSpeed(0);
	setRightSpeed(0);
	delay_ms(5000);
	
	LED1_OFF;
	LED2_ON;
	// Calculate Left D sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < 500; count ++){
		sum+= LD[count];
	}
	// Remove high error values	
	LSS = sum/500;
	num = 500;
	for (count = 0; count < 500; count ++){
		if (abs(LD[count] - LSS) > 150){
			sum-=LD[count];
			num--;
		}
	}
	// Recalculate average
	LSS = sum/num;
	
	
	
	
	// Calculate Right D sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < 500; count ++){
		sum+= RD[count];
	}
	// Remove high error values	
	RSS = sum/500;
	num = 500;
	for (count = 0; count < 500; count ++){
		if (abs(RD[count] - RSS) > 100){
			sum-=RD[count];
			num--;
		}
	}
	// Recalculate average
	RSS = sum/num;
	
	
	
	
	LED2_OFF;	
	LED3_ON;

	delay_ms(5000);
	
	
	// Get front data
	for (count = 0; count < 500; count ++){
	
		readSensor();

		FR[count] = FRSensor;
		FL[count] = FLSensor;
		
		delay_ms(4);	
	}
	
	
	
		
	setLeftSpeed(0);
	setRightSpeed(0);
	delay_ms(10000);
	
	
	// Calculate Left D sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < 500; count ++){
		sum+= FR[count];
	}
	// Remove high error values	
	FRSS = sum/500;
	num = 500;
	for (count = 0; count < 500; count ++){
		if (abs(FR[count] - FRSS) > 100){
			sum-=FR[count];
			num--;
		}
	}
	// Recalculate average
	FRSS = sum/num;
	
	
	
	
	// Calculate Right D sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < 500; count ++){
		sum+= FL[count];
	}
	// Remove high error values	
	FLSS = sum/500;
	num = 500;
	for (count = 0; count < 500; count ++){
		if (abs(FL[count] - FLSS) > 100){
			sum-=FL[count];
			num--;
		}
	}
	// Recalculate average
	FLSS = sum/num;
	
	delay_ms(5000);
	
	LED3_OFF;
	
}

/*
 * Return 0 for NO RIGHT WALL and non-zero otherwise.  
 */
byte Controller_check_right_wall(void){
	// read all sensor
	readSensor();
	return (DRSensor > DR_THRESHOLD);
}

/*
 * Return 0 for NO LEFT WALL and non-zero otherwise.  
 */
byte Controller_check_left_wall(void){
	// read all sensor
	readSensor();
	return 	(DLSensor > DL_THRESHOLD);
}



/*
 * This function check the left wall only
 * Return 0 for NO FRONT WALL and non-zero otherwise.  
 */
byte Controller_check_front_wall(void){
	// read all sensor
	readSensor();
	return ((FLSensor > FL_THRESHOLD)&&(FRSensor > FR_THRESHOLD));
}


void Controller_frontwall_corecttion(){

	int left_err;
	int right_err;
	int count = 0;
	
	while(count < 700){
		readSensor();	
		left_err = (FRONT_LEFT_WALL_DISTANCE - FLSensor)/25; // 20 work ok
		right_err = (FRONT_RIGHT_WALL_DISTANCE - FRSensor)/25; // 20 work ok
		if (left_err > 8) left_err = 8;
		if (left_err < -8) left_err = -8;
		if (right_err > 8) right_err = 8;
		if (right_err < -8) right_err = -8;
		setLeftSpeed(left_err);
		setRightSpeed(right_err);
		delay_ms(1);
		count ++;
	}
	
		setLeftSpeed(0);
		setRightSpeed(0);
			
}


void systick() {
}

void button1_interrupt() {
}


void button2_interrupt() {
}
