#include "main.h"
#include "Controller.h"
#include "Runner.h"
#include "Driver.h"
#include "delay.h"

byte current_direction_global;
byte current_position_global[2];
byte maze_array_global[MAZE_SIZE][MAZE_SIZE];
byte path_global[MAZE_SIZE*MAZE_SIZE];
byte maze_dist_array_global[MAZE_SIZE][MAZE_SIZE];

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
	
	for (i = 0; i < MAZE_SIZE * MAZE_SIZE; i ++){
		path_global[i] = 0;
	}
	
	
	
	
	
	
	


//	Controller_maze_calibrate();

//	Runner_run(40);
	
//	delay_ms(10000);
	
//	Runner_run(150);
	
	while(1) {
		
	Runner_explore(35);

		delay_ms(10000);
	}
}
