#include "Runner.h"
#include "Driver.h"
#include "main.h"


byte walls_info;
byte frontSS, leftSS, rightSS;


/*
 * Maze is 16x16 blocks
 * Every Block is 180mm x 180 mm
 * Wall is 12mm thick
 * So the "road" is 180mm - (12mm/2)*2 = 168mm
 */



void Runner_explore(){
	// Declaration off variables
	byte walls;

	//initialize current position
	current_position_global = 0;

	//initialize current position;
	int index i = 0, j = 0;


	while (1){

		walls = Driver_check_walls();
		walls_info = walls;

		//set the maze_array's wall bit
		if((READ_B(walls, FRONT)))
			SET_B(maze_array_global[i][j], current_direction_global);
		if((READ_B(walls, RIGHT)))
			SET_B(maze_array_global[i][j], RIGHT_DIRECT(current_direction_global));
		if((READ_B(walls,LEFT)))
			SET_B(maze_array_global[i][j],LEFT_DIRECT(current_direction_global));


		//depending on where current direction and wall sensor, make a turn or straight
		if (!(READ_B(walls, FRONT))){
			frontSS = (READ_B(walls, FRONT));
			Driver_go_straight(45, 30);

		} else if (!(READ_B(walls, RIGHT))){
			rightSS = (READ_B(walls, RIGHT));
			Driver_go_straight(45, 30);
			Driver_turn_right(0,90, 30);

			current_direction_global = RIGHT_DIRECT(current_direction_global);


		} else if (!(READ_B(walls, LEFT))){
			leftSS = (READ_B(walls, LEFT));
			Driver_go_straight(45, 30);
			Driver_turn_left(0,90, 30);
			
			current_direction_global = LEFT_DIRECT(current_direction_global);
		} else {
			Driver_go_straight(45, 30);
			Driver_turn_left(0,90, 30);
			Driver_turn_left(0,90, 30);

			current_direction_global = BACK_DIRECT(current_direction_global);
		}
		Driver_go_straight(135, 30);

		//set visit this cell already
		SET_B(maze_array_global[i][j], VISITED);

		//set up maze_array index to make modification
		switch(current_direction_global) {

			case EAST:
				current_position_global += 16;
				i++;
				break;
			case SOUTH:
				current_position_global++;
				j++;
				break;
			case WEST:
				current_position_global -= 16;
				i--;
				break;
			case NORTH:
				current_position_global --;
				j--;
				break;
		}

	}
}	
	
void Runner_run(int speed){
	
}
