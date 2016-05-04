#include "main.h"
#include "Controller.h"
#include "Runner.h"
#include "Driver.h"
#include "delay.h"

byte current_direction_global;
byte current_position_global;
int current_location_global[2];

int main(void) {

	Controller_hardware_init();
	current_direction_global = START_DIRECTION;
	
	LED4_ON;


//	Controller_maze_calibrate();

//	Runner_run(40);
//	delay_ms(5000);
	
	while(1) {
	Runner_explore();
	//Runner_run(40);
		delay_ms(2);
	}
}
