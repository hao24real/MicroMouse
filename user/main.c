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
// Change_flag for flood fill algorithm
byte row_Dest = MAZE_SIZE-1, column_Dest = MAZE_SIZE-1;

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
	

	Runner_explore(40);
	delay_ms(1000);
	
//	delay_ms(1000);


	delay_ms(10000);
	Runner_run(80);
	
	delay_ms(10000);
		



	
	while(1) {
		

		
	}
}
