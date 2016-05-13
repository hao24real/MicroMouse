/* File name: Controller.c
 * This file contain all method called by Driver
 * Will N.B.
 */
#include "Controller.h"
#include <stdlib.h>

#define KP 1000
#define KI 0
#define KD 0

byte PID_EN = DISABLE;
byte MODE = MODE_DEFAULT;

int LEFT_WALL_DISTANCE = 1035;
int RIGHT_WALL_DISTANCE = 1063;
int FRONT_RIGHT_WALL_DISTANCE = 3251;
int FRONT_LEFT_WALL_DISTANCE = 2759;

#define MAZE_ADRESS 0x08040000

#define LEFT_WALL_DISTANCE_ADRESS 0x08040100
#define RIGHT_WALL_DISTANCE_ADRESS 0x08040104
#define FRONT_LEFT_WALL_DISTANCE_ADRESS 0x08040108
#define FRONT_RIGHT_WALL_DISTANCE_ADRESS 0x0804010C

#define LEFT_WALL_THRESHOLD_ADRESS 0x08040110
#define RIGHT_WALL_THRESHOLD_ADRESS 0x08040114
#define FRONT_LEFT_WALL_THRESHOLD_ADRESS 0x08040118
#define FRONT_RIGHT_THRESHOLD_ADRESS 0x0804011C


#define VALID_ERR 15


int LSS;
int RSS;
int FLSS;
int FRSS;





void Controller_hardware_init(void) {

	//Local variable

	
	Systick_Configuration();
	LED_Configuration();
	button_Configuration();
	usart1_Configuration(9600);
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
	MAX_ERR = left_speed/15; // 18 before
	
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
			if ((DLSensor > (LEFT_WALL_DISTANCE-50))&&(DRSensor > (RIGHT_WALL_DISTANCE-50))){
				if (temp_cnt >200){				
					ERR = (DLSensor - DRSensor - (LEFT_WALL_DISTANCE - RIGHT_WALL_DISTANCE))/25;
					// Check if error is valid for correction ( too far from wall)
					if (ERR > (MAX_ERR)) ERR = MAX_ERR;
					if (ERR < -(MAX_ERR)) ERR = -(MAX_ERR);	
					ERR_LEFT_TO_PID = ERR;
					ERR_RIGHT_TO_PID = -ERR;
				} else {
					temp_cnt ++;
				}
				
				
				
				// CASE 2: have left wall
			} else	if (DLSensor > (LEFT_WALL_DISTANCE-50)){
				if (temp_cnt >200){				
					ERR = (DLSensor - LEFT_WALL_DISTANCE)/25;
					// Check if error is valid for correction ( too far from wall)
					if (ERR < -VALID_ERR){
						ERR = 0;
					} else{
						if (ERR > (MAX_ERR)) ERR = MAX_ERR;
						if (ERR < -(MAX_ERR)) ERR = -(MAX_ERR);			
					}
					ERR_LEFT_TO_PID = ERR;
					ERR_RIGHT_TO_PID = -ERR;
				} else {
					temp_cnt ++;
				}
				
				
				
				
				// Case 3: Use right wall for correction
			} else if (DRSensor > (RIGHT_WALL_DISTANCE-50)){
				if (temp_cnt >200){				
					ERR = (DRSensor - RIGHT_WALL_DISTANCE)/25;
					// Check if error is valid for correction ( too far from wall)
					if (ERR < -VALID_ERR){
						ERR = 0;
					} else{
						if (ERR > (MAX_ERR)) ERR = MAX_ERR;
						if (ERR < -(MAX_ERR)) ERR = -(MAX_ERR);			
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




void Controller_writeFlash(void){
  int i, j;

  setLeftSpeed(0);
  setRightSpeed(0);
  FLASH_Unlock();
	

  FLASH_ClearFlag( FLASH_FLAG_EOP|FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
/**
  * @brief  Erases a specified FLASH Sector.
  *   
  * @param  FLASH_Sector: The Sector number to be erased.
  *          This parameter can be a value between FLASH_Sector_0 and FLASH_Sector_11
  *    
  * @param  VoltageRange: The device voltage range which defines the erase parallelism.  
  *          This parameter can be one of the following values:
  *            @arg VoltageRange_1: when the device voltage range is 1.8V to 2.1V, 
  *                                  the operation will be done by byte (8-bit) 
  *            @arg VoltageRange_2: when the device voltage range is 2.1V to 2.7V,
  *                                  the operation will be done by half word (16-bit)
  *            @arg VoltageRange_3: when the device voltage range is 2.7V to 3.6V,
  *                                  the operation will be done by word (32-bit)
  *            @arg VoltageRange_4: when the device voltage range is 2.7V to 3.6V + External Vpp, 
  *                                  the operation will be done by double word (64-bit)
  *       
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PROGRAM,
  *                       FLASH_ERROR_WRP, FLASH_ERROR_OPERATION or FLASH_COMPLETE.
  */
  FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);

  for(i=0; i<MAZE_SIZE; i++)
    for(j=0; j<MAZE_SIZE; j++)
      FLASH_ProgramHalfWord((MAZE_ADRESS + (i*MAZE_SIZE+j)*4),maze_array_global[i][j]);
			
			
	FLASH_ProgramHalfWord(LEFT_WALL_DISTANCE_ADRESS, LEFT_WALL_DISTANCE);
  FLASH_ProgramHalfWord(RIGHT_WALL_DISTANCE_ADRESS, RIGHT_WALL_DISTANCE);
  FLASH_ProgramHalfWord(FRONT_LEFT_WALL_DISTANCE_ADRESS, FRONT_LEFT_WALL_DISTANCE);
  FLASH_ProgramHalfWord(FRONT_RIGHT_WALL_DISTANCE_ADRESS,FRONT_RIGHT_WALL_DISTANCE);

  FLASH_Lock();
}


void Controller_readMazeFlash(void){
  u32 i, j;
  for(i=0; i<MAZE_SIZE; i++)
    for(j=0; j<MAZE_SIZE; j++)
      maze_array_global[i][j] = *(int16_t *)(MAZE_ADRESS + (i*MAZE_SIZE+j)*4);
}

void Controller_readSensorFlash(void){
  LEFT_WALL_DISTANCE = *(int16_t *)(FRONT_LEFT_WALL_DISTANCE_ADRESS);
  RIGHT_WALL_DISTANCE = *(int16_t *)(FRONT_LEFT_WALL_DISTANCE_ADRESS);
  FRONT_RIGHT_WALL_DISTANCE = *(int16_t *)(FRONT_LEFT_WALL_DISTANCE_ADRESS);
  FRONT_LEFT_WALL_DISTANCE = *(int16_t *)(FRONT_LEFT_WALL_DISTANCE_ADRESS);
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
	

	// THESE VARIABLES SHOULD BE IN GLOBAL>>
	// PUT HERE FOR TESTIG>>
	int count, num;
	int sum;
	
	
	delay_ms(2000);
	
	ALL_LED_OFF;
	LED1_ON;
	LED3_ON;
	
	setLeftSpeed(30);
	setRightSpeed(30);
	
	for (count = 0; count < 256; count ++){
	
		readSensor();
		
		general_purpose_array_1[count] = DLSensor;
		general_purpose_array_2[count] = DRSensor;
		
		delay_ms(15);	
	}
	
	setLeftSpeed(0);
	setRightSpeed(0);
	delay_ms(5000);

	// Calculate Left D sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		sum+= general_purpose_array_1[count];
	}
	// Remove high error values	
	LSS = sum/GENERAL_ARRAY_SIZE;
	num = GENERAL_ARRAY_SIZE;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		if (abs(general_purpose_array_1[count] - LSS) > 150){
			sum-=general_purpose_array_1[count];
			num--;
		}
	}
	LSS = sum/num;
	
	
	
	
	// Calculate Right D sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		sum+= general_purpose_array_2[count];
	}
	// Remove high error values	
	RSS = sum/GENERAL_ARRAY_SIZE;
	num = GENERAL_ARRAY_SIZE;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		if (abs(general_purpose_array_2[count] - RSS) > 150){
			sum-=general_purpose_array_2[count];
			num--;
		}
	}
	RSS = sum/num;
	
	
	LED1_OFF;	
	LED3_OFF;
	delay_ms(10000);
	
	LED2_ON;	
	LED4_ON;
	delay_ms(5000);	
	
	// Get front data
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
	
		readSensor();

		general_purpose_array_1[count] = FLSensor;
		general_purpose_array_2[count] = FRSensor;
		
		delay_ms(10);	
	}
	
	
	
		
	setLeftSpeed(0);
	setRightSpeed(0);
	delay_ms(2000);
	
	// Calculate left F sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		sum+= general_purpose_array_1[count];
	}
	// Remove high error values	
	FLSS = sum/GENERAL_ARRAY_SIZE;
	num = GENERAL_ARRAY_SIZE;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		if (abs(general_purpose_array_1[count] - FLSS) > 150){
			sum-=general_purpose_array_1[count];
			num--;
		}
	}
	FLSS = sum/num;
	
	
	
	// Calculate right F sensor
	// Calculate sum
	sum = 0;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		sum+= general_purpose_array_2[count];
	}
	// Remove high error values	
	FRSS = sum/GENERAL_ARRAY_SIZE;
	num = GENERAL_ARRAY_SIZE;
	for (count = 0; count < GENERAL_ARRAY_SIZE; count ++){
		if (abs(general_purpose_array_2[count] - FRSS) > 150){
			sum-=general_purpose_array_2[count];
			num--;
		}
	}
	// Recalculate average
	FRSS = sum/num;
	
	
	
	
	LED2_OFF;
	LED4_OFF;
	delay_ms(5000);
	
	// Done 
	// TODO: Write to Flash
	Controller_writeFlash();
	Controller_readSensorFlash();
	
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

/*
 * This function using the 2 front sensor to make correction
 */
void Controller_frontwall_corecttion(){

	int left_err;
	int right_err;
	int count = 0;
	
	while(count < 400){
		readSensor();	
		left_err = (FRONT_LEFT_WALL_DISTANCE - FLSensor)/22; // 20 work ok
		right_err = (FRONT_RIGHT_WALL_DISTANCE - FRSensor)/22; // 20 work ok
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
	LED2_ON;
}


void button2_interrupt() {
	int encode_val, exit;
	int count = 1;



	setLeftSpeed(0);
	setRightSpeed(0);
	resetRightEncCount();
	resetLeftEncCount();
	
	PID_EN = DISABLE;

	
	while (exit != 100000){
		
		encode_val = getRightEncCount();	
		exit = getLeftEncCount();

		
		// This determine to exit the loop
		count = 0;
		while (exit > 2000){
			exit = getLeftEncCount();
			ALL_LED_ON;
			delay_ms(300);
			ALL_LED_OFF;
			delay_ms(300);
			count ++;
			if (count  == 10){
				exit = 100000;
				break;
			}	
		}
		
		if (encode_val < 3001){
			MODE = MODE_EXPLORE;
			ALL_LED_OFF;
			LED1_ON;
		} else if ((encode_val > 3000)&&(encode_val < 5001)){
			MODE = MODE_SPEED_RUN;
			ALL_LED_OFF;
			LED2_ON;
		} else if ((encode_val > 5000)&&(encode_val < 7001)){
			MODE = MODE_2;
			ALL_LED_OFF;
			LED3_ON;
		} else if ((encode_val > 7000)&&(encode_val < 9001)){
			MODE = MODE_3;
			ALL_LED_OFF;
			LED4_ON;
		}else if ((encode_val > 9000)&&(encode_val < 11001)){
			MODE = MODE_4;
			ALL_LED_OFF;
			LED5_ON;
		}else if ((encode_val > 11000)&&(encode_val < 13001)){
			MODE = MODE_CALIBRATE;
			ALL_LED_OFF;
			LED6_ON;
		}else{
			MODE = MODE_DEFAULT;		
			ALL_LED_OFF;
		}
	}
	resetRightEncCount();
	resetLeftEncCount();	
	setLeftSpeed(0);
	setRightSpeed(0);

	PID_EN = ENABLE;
}
