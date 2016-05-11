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
	
	
	// Next interested position
	byte next_position;
	// neighbor value
	byte next_dist, east_neighbor, south_neighbor, west_neighbor, north_neighbor;
	
	byte path_index;

void maze_initialize(byte row_Dest, byte column_Dest){
	byte row, column;
	
	for (row = 0; row< MAZE_SIZE; row++){
		for (column = 0; column < MAZE_SIZE; column++){
				maze_dist_array_global[row][column] = abs(row - row_Dest) + abs(column - column_Dest);
		}
	}
}


void maze_floodfill(){
	
	
	byte row, column, counter;
	byte min_neighbor;
	

	
	// Change this..
	for(counter = 0; counter < MAZE_SIZE*MAZE_SIZE;counter++)
		for (row = 0; row< MAZE_SIZE; row++)
			for (column = 0; column < MAZE_SIZE; column++){
				
				if (READ_B(maze_array_global[row][column], VISITED)){
					
						min_neighbor = MAZE_SIZE * MAZE_SIZE - 1;


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
							
				}
	}
	
}// end method

void debug_dist(){
	int i;
	int j;
	for( i = 0; i < MAZE_SIZE; i++){
		for(j = 0; j < MAZE_SIZE; j++){
			path_1_global[(i*6)+ j ] = maze_dist_array_global[i][j];
		}
	}
}
	


void store_path(){
	
	byte index, current_dist, next_position_local, east_neighbor_dist, west_neighbor_dist, south_neighbor_dist, north_neighbor_dist;

	byte current_direction_local = EAST;
	
	LED1_ON;
	
	row_Dest = MAZE_SIZE -1;
	column_Dest = MAZE_SIZE -1;
	
	CLR_B(maze_array_global[row_Dest][column_Dest], VISITED);
	maze_dist_array_global[row_Dest][column_Dest] = 0;
	
	maze_floodfill();	
	
	debug_dist();
	
	SET_B(maze_array_global[row_Dest][column_Dest] ,VISITED);
	
	
	//use counter to keep track so we can use the path_1_global as a stack
	path_index = 0;
	
	//initialize the array for turning 
	for(index = 0; index < MAZE_SIZE * MAZE_SIZE; index++)
		path_run_global[index] = 0;

	//initialization from origin again
	current_position_global[ROW_INDEX] = 0;
	current_position_global[COLUMN_INDEX] = 0;


	while((current_position_global[ROW_INDEX] != row_Dest)||(current_position_global[COLUMN_INDEX] != column_Dest )) {

	
		current_dist = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]];	
		
		// Check EAST neighbor
		if (!READ_B(maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]], EAST)){		
			east_neighbor_dist = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]+1];
			if ((current_dist == (east_neighbor_dist + 1) )
				&& READ_B(maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX] + 1] ,VISITED)) 
			{
				next_position_local = EAST;
			}
		}
		
		// Check NORTH neighbor
		if (!READ_B(maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]], NORTH)){
			north_neighbor_dist = maze_dist_array_global[current_position_global[ROW_INDEX]-1][current_position_global[COLUMN_INDEX]];
			if ((current_dist == (north_neighbor_dist + 1)) 
				 && READ_B(maze_array_global[current_position_global[ROW_INDEX]-1][current_position_global[COLUMN_INDEX]] ,VISITED))
			{
				next_position_local = NORTH;
			}
		}
		
		// Check WEST neighbor
		if (!READ_B(maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]], WEST)){
			west_neighbor_dist = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]-1];
			if ((current_dist == (west_neighbor_dist + 1)) 
				&& READ_B(maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]-1] ,VISITED))
			{
				next_position_local = WEST;
			}
		}

		// Check SOUTH neighbor
		if (!READ_B(maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]], SOUTH)){
		  south_neighbor_dist = maze_dist_array_global[current_position_global[ROW_INDEX]+1][current_position_global[COLUMN_INDEX]];
			if ((current_dist == (south_neighbor_dist + 1))
				&& READ_B(maze_array_global[current_position_global[ROW_INDEX]+1][current_position_global[COLUMN_INDEX]] ,VISITED))
			{
				next_position_local = SOUTH;
			}
		}

		if(next_position_local == current_direction_local)
			path_run_global[path_index] = FRONT;
		else if (next_position_local == RIGHT_DIRECT(current_direction_local))
			path_run_global[path_index] = RIGHT;
		else if (next_position_local == LEFT_DIRECT(current_direction_local))
			path_run_global[path_index] = LEFT;
		else if (next_position_local == BACK_DIRECT(current_direction_local))
			path_run_global[path_index] = BACK;
		
		current_direction_local = next_position_local;
		
		// Update the corrent position global
		if (current_direction_local == EAST)
			current_position_global[COLUMN_INDEX]++;

		else if (current_direction_local == SOUTH)
			current_position_global[ROW_INDEX]++;

		else if (current_direction_local == WEST)
			current_position_global[COLUMN_INDEX]--;

		else if (current_direction_local == NORTH)
			current_position_global[ROW_INDEX]--;
		
		path_index ++;
	}
	
	
	LED1_OFF;
}



void Runner_explore(int speed ){
	
	
	byte i,j;
	
	byte stop = 0;
	// ===========================================================
		for (i=0;i<MAZE_SIZE; i++)
			for (j=0; j<MAZE_SIZE; j++){
				path_1_global[i*MAZE_SIZE +j]= maze_array_global[i][j];
				general_purpose_array_1[i*MAZE_SIZE +j] = maze_dist_array_global[i][j];
		}
		
		Driver_go_straight(0, 0);
		delay_ms(1000);
	
//==============================================================
	
	// Initialize the maze;
	maze_initialize(row_Dest, column_Dest);
	//initialization from origin
	current_position_global[ROW_INDEX] = 0;
	current_position_global[COLUMN_INDEX] = 0;
		
		// ===========================================================
		for (i=0;i<MAZE_SIZE; i++)
			for (j=0; j<MAZE_SIZE; j++){
				path_1_global[i*MAZE_SIZE +j]= maze_array_global[i][j];
				general_purpose_array_1[i*MAZE_SIZE +j] = maze_dist_array_global[i][j];
		}
		
		Driver_go_straight(0, 0);
		delay_ms(1000);
	
//==============================================================
		
	// Read first
	maze_array_global[0][0] = 0x1E;
	current_position_global[COLUMN_INDEX] ++;

	Driver_go_straight(90, speed);
	
	while(!stop) {
		
		while ((current_position_global[ROW_INDEX] != row_Dest)||(current_position_global[COLUMN_INDEX] != column_Dest )){

			//1. Check wall for current position. Current position is at the head of the mice
			walls_FLBR = Driver_check_walls();
			// Rotate the wall info accordingly to direction
			walls_ESWN = ROTATE_DIRECT(walls_FLBR, current_direction_global);
			
			
			// 2. Update walls info for current position. Set this cell as visited		
			SET_B(walls_ESWN,VISITED);
			// Update the maze info
			maze_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]] |= walls_ESWN;


			// 3. Flood fill the maze to make sure we can find next position
			maze_floodfill();	
			
		
			
			// 3. Find next position. The flood fill just happened after this. So it guaranty a open position for next
			// 3.a. Determine next position. next_dist here is set default by current distance
			next_dist = 255;	
			
			// Find the smallest and closet neighbor of current position
			// Dont use if else statement because we have to go through four neighbor
			
			// Check EAST neighbor
			if (!READ_B(walls_ESWN, EAST)){		
				east_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]+1];
				if (next_dist >= east_neighbor){
					next_dist = east_neighbor;
					next_position = EAST;
				}
			}
			
			// Check NORTH neighbor
			if (!READ_B(walls_ESWN, NORTH)){
				north_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]-1][current_position_global[COLUMN_INDEX]];
				if (next_dist > north_neighbor){
					next_dist = north_neighbor;
					next_position = NORTH;
				}
			}
			
			// Check WEST neighbor
			if (!READ_B(walls_ESWN, WEST)){
				west_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]-1];
				if (next_dist > west_neighbor){
					next_dist = west_neighbor;
					next_position = WEST;
				}
			}
			
			// Check SOUTH neighbor
			if (!READ_B(walls_ESWN, SOUTH)){
				south_neighbor = maze_dist_array_global[current_position_global[ROW_INDEX]+1][current_position_global[COLUMN_INDEX]];
				if (next_dist >= south_neighbor){
					next_dist = south_neighbor;
					next_position = SOUTH;
				}
			}

			// Case 1: Next position is in front of current position
			if (next_position == current_direction_global ){	
				Driver_go_straight(180, speed);	
				// Dont need to update the current direction

			// Case 2: Next position is on the right of current position
			} else if (next_position == RIGHT_DIRECT(current_direction_global)){

				Driver_go_straight(90, speed);
				// Now we are in the center of 1 cell. Check if there is a wall in front of us for make correction
				if (READ_B(walls_FLBR, FRONT))
						Driver_frontwall_correction();
				Driver_turn_right(0,88, speed);
				Driver_go_straight(90, speed);

					

				current_direction_global = next_position;
				
			// Case 3: Next position is at the left	
			} else if (next_position == LEFT_DIRECT(current_direction_global)){
				
					Driver_go_straight(90, speed);
					// Now we are in the center of 1 cell. Check if there is a wall in front of us for make correction
					if (READ_B(walls_FLBR, FRONT))
						Driver_frontwall_correction();
					Driver_turn_left(0,90, speed);
					Driver_go_straight(90, speed);
				
				
				current_direction_global = next_position;
		
			// Case 4: Next position is in the back. Make a U turn
			} else if (next_position == BACK_DIRECT(current_direction_global )){
					
				Driver_go_straight(90, speed);
				// Now we are in the center of 1 cell. Check if there is a wall in front of us for make correction
				if (READ_B(walls_FLBR, FRONT))
						Driver_frontwall_correction();	
				Driver_turn_left(0,90, speed);
				// Now we are in the center of 1 cell. Check if there is a wall in front of us for make correction
				if (READ_B(walls_FLBR, FRONT))
					Driver_frontwall_correction();		
				Driver_turn_left(0,90, speed);
				if (READ_B(walls_FLBR, FRONT))
					Driver_frontwall_correction();			
				Driver_go_straight(90, speed);
				
				
				current_direction_global = BACK_DIRECT(current_direction_global);
			} 
			else {
				while (1){
					delay_ms(200);
					LED3_ON;
					delay_ms(200);
					LED3_OFF;
				}
			}
								
			
			// Update the corrent position global
			if (current_direction_global == EAST)
				current_position_global[COLUMN_INDEX]++;

			else if (current_direction_global == SOUTH)
				current_position_global[ROW_INDEX]++;

			else if (current_direction_global == WEST)
				current_position_global[COLUMN_INDEX]--;

			else if (current_direction_global == NORTH)
				current_position_global[ROW_INDEX]--;

		} // While loop
			
			row_Dest = 0; 
			column_Dest = 0;
			if((current_position_global[ROW_INDEX] == row_Dest) && (current_position_global[COLUMN_INDEX] == column_Dest)){
				 stop = 1;
			}
			maze_initialize(row_Dest,column_Dest);
			CLR_B(maze_array_global[row_Dest][column_Dest], VISITED);
	}
	
	/*
	 ===========================================================
		for (i=0;i<MAZE_SIZE; i++)
			for (j=0; j<MAZE_SIZE; j++){
				path_1_global[i*MAZE_SIZE +j]= maze_array_global[i][j];
				path_2_global[i*MAZE_SIZE +j] = maze_dist_array_global[i][j];
		}
			
		
		Driver_go_straight(0, 0);
  	delay_ms(1000);
==============================================================
	*/
	//Driver_go_straight(0,0);
	//delay_ms(1000);
	
	Driver_go_straight(90,speed);
	Driver_go_straight(0,0);
	
	//delay_ms(1000);
	
	//CLR_B(maze_array_global[row_Dest][column_Dest], VISITED);
	//maze_dist_array_global[row_Dest][column_Dest] = 0;
	//maze_floodfill();
	store_path();

	
} // End method
	
void Runner_run(int speed){

		byte path_count;
	
	  //path index is the size of the path_run_array
		for(path_count = 0; path_count < path_index; path_count ++){
			switch(path_run_global[path_count]){
				case FRONT:
					Driver_go_straight(180, speed);
					break;
				case RIGHT:
					Driver_turn_right(0, 90, speed);
					Driver_go_straight(180, speed);
					break;
				case LEFT:
					Driver_turn_left(0, 90, speed);
					Driver_go_straight(180, speed);
					break;
			}
		}
		
		Driver_go_straight(0, 0);
		
}



