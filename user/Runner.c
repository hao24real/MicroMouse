#include "Runner.h"
#include "Driver.h"
#include "main.h"
#include "delay.h"

/*
 * Maze is 16x16 blocks
 * Every Block is 180mm x 180 mm
 * Wall is 12mm thick
 * So the "road" is 180mm - (12mm/2)*2 = 168mm
 */

void Runner_explore(){
	// Declaration off variables
	byte walls;
	int counter = 0;
	int speed = 50;
	
							Driver_go_straight(90, speed);

//Driver_go_straight(0, 0);
//delay_ms(500);	
	
	while (1){
		counter ++;
		
//Driver_go_straight(0, 0);
//delay_ms(500);			
		
		
		/* All of the code is for testing only*/
		walls = Driver_check_walls();
		
		if (!(READ_B(walls, FRONT))){	

						Driver_go_straight(180, speed);	
			
//Driver_go_straight(0, 0);
//delay_ms(500);				
			
		}else if(!(READ_B(walls, RIGHT)) && !(READ_B(walls, LEFT))){
					
			if(counter & 1){
			
				
				Driver_go_straight(90, speed);
				Driver_frontwall_correction();
				Driver_turn_right(0,85, speed);
				Driver_go_straight(90, speed);
	
//Driver_go_straight(0, 0);
//delay_ms(500);	
				
			// Driver_frontwall_correction();
			current_direction_global = RIGHT_DIRECT(current_direction_global);
			}else{
				
				Driver_go_straight(90, speed);
				Driver_frontwall_correction();
				Driver_turn_left(0,83, speed);
				Driver_go_straight(90, speed);
			
//Driver_go_straight(0, 0);
//delay_ms(500);					
				
			
			current_direction_global = LEFT_DIRECT(current_direction_global);
				
			}
			
		}else if (!(READ_B(walls, RIGHT))){
	
				Driver_go_straight(90, speed);
				Driver_frontwall_correction();
				Driver_turn_right(0,85, speed);
				Driver_go_straight(90, speed);

//Driver_go_straight(0, 0);
//delay_ms(500);				
			
			current_direction_global = RIGHT_DIRECT(current_direction_global);
			
			
		} else if (!(READ_B(walls, LEFT))){
			
				Driver_go_straight(90, speed);
				Driver_frontwall_correction();
				Driver_turn_left(0,83, speed);
				Driver_go_straight(90, speed);
			
			
			
			
			current_direction_global = LEFT_DIRECT(current_direction_global);
	
		} else {
			
			
//			Driver_go_straight(45, speed);
			Driver_go_straight(90, speed);
			Driver_frontwall_correction();			
			Driver_turn_left(0,83, speed);
			Driver_frontwall_correction();			
			Driver_turn_left(0,83, speed);
			Driver_go_straight(90, speed);
			
//Driver_go_straight(0, 0);
//delay_ms(500);	

			current_direction_global = BACK_DIRECT(current_direction_global);

		}
		
		
		
		

		
		
		
	}
}	
	
void Runner_run(int speed){

	Driver_go_straight(540, 60);

	//Driver_frontwall_correction();
		
}
