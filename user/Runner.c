#include "Runner.h"
#include "Driver.h"
#include "main.h"
#include "delay.h"
#include <stdlib.h>


/*
 * Maze is 16x16 blocks
 * Every Block is 180mm x 180 mm
 * Wall is 12mm thick
 * So the "road" is 180mm - (12mm/2)*2 = 168mm
 */

	// Declaration off variables
	byte walls_ESWN;
	
	byte walls_FLBR;
	
	// Change_flag for flood fill algorithm
	byte row_Dest = MAZE_SIZE-1, column_Dest = MAZE_SIZE-1;
	// Next interested position
	byte next_position;
	// neighbor value
	byte next_dist, east_neighbor, south_neighbor, west_neighbor, north_neighbor;


void maze_initialize(byte row_Dest, byte column_Dest){
	byte row, column;
	
	for (row = 0; row< MAZE_SIZE; row++){
		for (column = 0; column < MAZE_SIZE; column++){
				maze_dist_array_global[row][column] = abs(row - row_Dest) + abs(column - column_Dest);
		}
	}
}


void maze_floodfill(){
	byte row, column;
	byte min_neighbor;byte old_val;
	byte change_flag = 1;
	// Change this..
	while(change_flag){
		
		change_flag = 0;
		
		for (row = 0; row< MAZE_SIZE; row++){
			for (column = 0; column < MAZE_SIZE; column++){
				
				min_neighbor = maze_dist_array_global[row][column];
				old_val = min_neighbor;
				
				if (READ_B(maze_array_global[row][column], VISITED)){

						if (!READ_B(maze_array_global[row][column], EAST))
								if (min_neighbor > maze_dist_array_global[row][column+1])
									min_neighbor = maze_dist_array_global[row][column+1];
							
						if (!READ_B(maze_array_global[row][column], SOUTH))
							if (min_neighbor > maze_dist_array_global[row+1][column])
								min_neighbor = maze_dist_array_global[row+1][column];
							
						if (!READ_B(maze_array_global[row][column], WEST))
							if (min_neighbor > maze_dist_array_global[row][column-1])
								min_neighbor = maze_dist_array_global[row][column-1];

						if (!READ_B(maze_array_global[row][column], NORTH))
							if (min_neighbor > maze_dist_array_global[row-1][column])
								min_neighbor = maze_dist_array_global[row-1][column];
							
						maze_dist_array_global[row][column] = min_neighbor + 1;
							
						if (old_val != maze_dist_array_global[row][column])
							change_flag = 1;
				
				}
					

			}
		}
		
	}
	
}




void Runner_explore(int speed ){
	

	byte i,j;
	
	current_position_global[ROW_INDEX] = 0;
	current_position_global[COLUMN_INDEX] = 0;
	
	
	// Initialize the maze;
	maze_initialize(row_Dest, column_Dest);
	
	// Read first
	maze_array_global[0][0] = 0x1E;
	next_position = EAST;


	current_position_global[COLUMN_INDEX] ++;

	Driver_go_straight(90, speed);
//delay_ms(500);	

	
	while (1){
		
//Driver_go_straight(0, 0);
//delay_ms(500);			

		
		


		/* All of the code is for testing only*/
		walls_FLBR = Driver_check_walls();
		
		
		
		// Rotate the wall info accordingly to direction
		walls_ESWN = ROTATE_DIRECT(walls_FLBR, current_direction_global);
		
		// Set this cell as visited		
		SET_B(walls_ESWN,VISITED);
		
		
		// Update the maze info
		maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]] = walls_ESWN;
		
		// Fload fill algorithm
		maze_floodfill();
		/*
				// ===========================================================
		for (i=0;i<MAZE_SIZE; i++)
			for (j=0; j<MAZE_SIZE; j++){
				path_1_global[i*MAZE_SIZE +j]= maze_array_global[i][j];
				path_2_global[i*MAZE_SIZE +j] = maze_dist_array_global[i][j];
		}
		
		Driver_go_straight(0, 0);
		delay_ms(1000);
	
//==============================================================
		*/
		
		
		
		// Determine next position. Calculate current position is acctually last position
		next_dist = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]];
		
		
		if (!READ_B(walls_ESWN, EAST)){
			
			east_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]+1];
			if (next_dist > east_neighbor){
				next_dist = east_neighbor;
				next_position = EAST;
			}
		}
		if (!READ_B(walls_ESWN, SOUTH)){
			
		  south_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]+1][current_position_global[COLUMN_INDEX]];
			if (next_dist > east_neighbor){
				next_dist = south_neighbor;
				next_position = SOUTH;
			}
		}
		if (!READ_B(walls_ESWN, NORTH)){
			
		north_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]-1][current_position_global[COLUMN_INDEX]];
			if (next_dist > north_neighbor){
				next_dist = north_neighbor;
				next_position = NORTH;
			}
		}
		if (!READ_B(walls_ESWN, WEST)){
		west_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]-1];
			if (next_dist > west_neighbor){
				next_dist = west_neighbor;
				next_position = WEST;
			}
		}		

		
		if (( current_direction_global - next_position)== 0){	
					Driver_go_straight(180, speed);	
			
//Driver_go_straight(0, 0);
//delay_ms(500);
			
		}else if (next_position == RIGHT_DIRECT(current_direction_global)){
	
			Driver_go_straight(90, speed);
			if (READ_B(walls_FLBR, FRONT))
				Driver_frontwall_correction();
			Driver_turn_right(0,85, speed);
			Driver_go_straight(90, speed);

//Driver_go_straight(0, 0);
//delay_ms(500);				
			
			current_direction_global = next_position;
			
			
		} else if (next_position == LEFT_DIRECT(current_direction_global)){
			
				Driver_go_straight(90, speed);
				if (READ_B(walls_FLBR, FRONT))
					Driver_frontwall_correction();
				Driver_turn_left(0,83, speed);
				Driver_go_straight(90, speed);
			
			
			current_direction_global = next_position;
	
		} else {
			
			
//			Driver_go_straight(45, speed);
			Driver_go_straight(90, speed);
			if (READ_B(walls_FLBR, FRONT))
				Driver_frontwall_correction();			
			Driver_turn_left(0,83, speed);
			if (READ_B(walls_FLBR, FRONT))
				Driver_frontwall_correction();			
			Driver_turn_left(0,83, speed);
			Driver_go_straight(90, speed);
			
//Driver_go_straight(0, 0);
//delay_ms(500);	

			current_direction_global = BACK_DIRECT(current_direction_global);
		}
		
		
		
		
		
		// Update the corrent position global
		switch (current_direction_global){
			case EAST: 		current_position_global[COLUMN_INDEX]++;
										break;
			case SOUTH: 	current_position_global[ROW_INDEX]++;
										break;
			case WEST: 		current_position_global[COLUMN_INDEX]--;
										break;
			case NORTH: 	current_position_global[ROW_INDEX]--;
										break;
			
		}

/*
// ===========================================================
		for (i=0;i<MAZE_SIZE; i++)
			for (j=0; j<MAZE_SIZE; j++){
				path_1_global[i*MAZE_SIZE +j]= maze_array_global[i][j];
				path_2_global[i*MAZE_SIZE +j] = maze_dist_array_global[i][j];
		}
			
		
		Driver_go_straight(0, 0);
  	delay_ms(1000);
//==============================================================
*/		
	}
}

	
void Runner_run(int speed){

		// Declaration off variables
	byte walls;
	int counter = 0;
	
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
			
			
			// Copy data for monitoring

		}
		
	}
		
}



