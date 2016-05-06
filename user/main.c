#include "main.h"
#include "Controller.h"
#include "Runner.h"
#include "Driver.h"
#include "delay.h"

byte current_direction_global;
byte current_position_global[2];
byte maze_array_global[MAZE_SIZE][MAZE_SIZE];
byte path_1_global[MAZE_SIZE*MAZE_SIZE];
byte path_2_global[MAZE_SIZE*MAZE_SIZE];

int main(void) {

	Controller_hardware_init();
	current_direction_global = START_DIRECTION;
	
	LED4_ON;


//	Controller_maze_calibrate();

//	Runner_run(40);
	
//	delay_ms(10000);
	
	Runner_run(150);
	
	while(1) {
		
	//Runner_explore();

		delay_ms(2);
	}
}
