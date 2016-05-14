#include "main.h"
#include "Controller.h"
#include "Runner.h"
#include "Driver.h"
#include "delay.h"

byte current_direction_global;
byte current_position_global[2];
byte maze_array_global[MAZE_SIZE][MAZE_SIZE];
byte maze_dist_array_global[MAZE_SIZE][MAZE_SIZE];
byte path_1_global[MAZE_SIZE*MAZE_SIZE];
byte general_purpose_array_1[GENERAL_ARRAY_SIZE];
byte general_purpose_array_2[GENERAL_ARRAY_SIZE];
byte path_run_global[MAZE_SIZE * MAZE_SIZE];

int main(void) {

	byte i;
	byte j;
	
	// 1. HARDWARE INITIALIZE
	Controller_hardware_init();
	LED4_ON;
	
	// 2. INITIALIZE GLOABL VARIABLES
	// a. current postion global, current direction global
	current_position_global[ROW_INDEX] = 0;
	current_position_global[COLUMN_INDEX] = 0;
	current_direction_global = START_DIRECTION;

	// b. maze distance array global
	for(i = 0; i < MAZE_SIZE; i++){
		for(j = 0; j < MAZE_SIZE; j++){
			maze_dist_array_global[i][j] = 0;
			maze_array_global[i][j] = 0;
		}
	}
	
	// c. maze walls info array
	for(i = 0 ; i < MAZE_SIZE; i++){
		SET_B(maze_array_global[i][0],WEST);
		SET_B(maze_array_global[0][i],NORTH);
		SET_B(maze_array_global[i][MAZE_SIZE-1], EAST);
		SET_B(maze_array_global[MAZE_SIZE-1][i], SOUTH);
	}
	SET_B(maze_array_global[0][0], SOUTH);

	
	// 3. START PROGRAM ---
  while (1){

		switch (MODE){
		
			case MODE_EXPLORE:
																
																LED1_ON;
																Runner_explore(80);
																MODE = MODE_SPEED_RUN;
																break;
			case MODE_SPEED_RUN:
																
																Runner_run(100);
																LED2_ON;
																MODE = MODE_DEFAULT;
																
																break;
			case MODE_2:							
																Driver_check_walls();
																printf("FLSensor: %d\n", FLSensor);
																printf("FRSensor: %d\n\n", FRSensor);
																printf("DLSensor: %d\n", DLSensor);
																printf("DRSensor: %d\n\n", DRSensor);
																LED3_ON;
																break;
			case MODE_3:
																LED4_ON;
																Driver_turn_right(0, 45, 50);
																Driver_go_straight(0,0);
																delay_ms(1000);
																Driver_turn_right(0, 45, 50);
																Driver_go_straight(0,0);
																delay_ms(1000);
																Driver_turn_right(0, 45, 50);
																Driver_go_straight(0,0);
																delay_ms(1000);
																Driver_turn_right(0, 45, 50);
																Driver_go_straight(0,0);
																delay_ms(1000);
																MODE = MODE_DEFAULT;
																break;
			case MODE_4:
				//Runner_run_onpost(80);
																LED5_ON;
																Driver_go_straight(720, 30);
																Driver_go_straight(0,0);
																MODE = MODE_DEFAULT;
																break;
			case MODE_CALIBRATE:
																Controller_maze_calibrate();
																LED6_ON;
																break;
			default:
																LED4_ON;
																break;
		}
	
		
	}


}
