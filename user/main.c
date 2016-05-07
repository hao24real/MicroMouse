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
byte path_2_global[MAZE_SIZE*MAZE_SIZE];
byte path_3_global[MAZE_SIZE*MAZE_SIZE];
byte path_run_global[MAZE_SIZE * MAZE_SIZE];

int main(void) {

	byte i;
	byte j;
	Controller_hardware_init();
	
	current_direction_global = START_DIRECTION;
	
	
	
	LED4_ON;
	
	
	for(i = 0; i < MAZE_SIZE; i++){
		for(j = 0; j < MAZE_SIZE; j++){
			maze_dist_array_global[i][j] = 0;
			maze_array_global[i][j] = 0;
		}
	}
	
	for(i = 0 ; i < MAZE_SIZE; i++){
		SET_B(maze_array_global[i][0],WEST);
		SET_B(maze_array_global[0][i],NORTH);
		SET_B(maze_array_global[i][MAZE_SIZE-1], EAST);
		SET_B(maze_array_global[MAZE_SIZE-1][i], SOUTH);
	}
	
	
	
//	for (i = 0; i < MAZE_SIZE * MAZE_SIZE; i ++){
//		path_1_global[i] = 0;
//	}
	
	
	
	
	
	
	//Driver_turn_right_onpost(180,150);
	//Driver_go_straight(90,150);
	//Driver_go_straight(90,150);
	//Driver_go_straight(0,0);	


//	Controller_maze_calibrate();

//	Runner_run(40);
	
//	delay_ms(10000);
	
//	Runner_run(150);
	
	//while(1) {
		
	Runner_explore(40);

		delay_ms(10000);
		
	Runner_run(60);
//	Runner_explore(65);
		
		delay_ms(10000);
		
//			Runner_explore(95);
		
		//delay_ms(10000);
		
	///			Runner_explore(135);
		
		//delay_ms(10000);
		
	//}
}
