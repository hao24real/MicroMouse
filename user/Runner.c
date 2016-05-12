#include "Runner.h"
#include "Driver.h"
#include "main.h"
#include "delay.h"
#include "usart.h"
#include <stdlib.h>


/*
 * Maze is 16x16 blocks
 * Every Block is 180mm x 180 mm
 * Wall is 12mm thick
 * So the "road" is 180mm - (12mm/2)*2 = 168mm
 */
 
#define TURN_LEFT_ANGLE 90
#define TURN_RIGHT_ANGLE 88
#define FIRST_RUN 1
#define SECOND_RUN 2
#define THIRD_RUN 3
#define RETURN_RUN 5
#define GOOD_PATH (MAZE_SIZE*MAZE_SIZE - 5)


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


/*
void BFS_floodfill(){

	//columns are for indicating the position of cell
	byte stack_cell1[MAZE_SIZE * MAZE_SIZE][2];
	byte stack_cell2[MAZE_SIZE * MAZE_SIZE][2];

	byte index_stack1 = 0;
	byte index_stack2 = 0;

	byte count_stack1 = 0;
	byte count_stack2 = 0;

	bool reach_origin = 0;

	//only terminate when simulation of MM reach 
	while(!reach_origin){

		while(count_stack1 != 0 && !reach_origin){
			

		}
		while(count_stack2 != 0 && !reach_origin){

		}

	}

// 	#define CLR_B(p,n) ((p) &= ~((1) << (n)))
// 	SET_B(maze_array_global, FLOODED);
// 	#define READ_B(p,n) ((p) & ((1) << (n)))


}
*/

void maze_floodfill(){
	
	byte row, column, counter;
	byte min_neighbor;
	
	
	// MAGIC 256 TIMES CHANGING..
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
							
						if (min_neighbor < MAZE_SIZE * MAZE_SIZE - 1)
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
	



/* 
 * This Method find and store the shortest path from begining to center
 * Return the number of cell need to inoder to go to center 
 */
byte store_path(){
	
	byte index, current_dist, next_position_local, current_position_local[2];
	byte east_neighbor_dist, west_neighbor_dist, south_neighbor_dist, north_neighbor_dist;
	byte current_direction_local = EAST;
	
	LED1_ON;

	// 1. Initialize the maze.
	maze_initialize(MAZE_SIZE - 1, MAZE_SIZE -1);

	// 2. Floodfill the whole maze
	// a. Set ultimate destination VISITED bit 0 so that floodfill will not change this.
	// b. Then set it to visited so that store path can reach
	// POST REQUIREMENT: The ultimate destination (the center) is visited
	// so we will not worry about storing the old value of this position
	// It will be set to true anyway
	CLR_B(maze_array_global[MAZE_SIZE - 1][MAZE_SIZE -1], VISITED);
	maze_floodfill();	
	SET_B(maze_array_global[MAZE_SIZE - 1][MAZE_SIZE -1], VISITED);

	// 3. Find the shortest path to the ultimate destination
	// a. use counter to keep track so we can use the path_1_global as a stack
	path_index = 0;
	
	// b. initialize the array for turning 
	for(index = 0; index < MAZE_SIZE * MAZE_SIZE; index++)
		path_run_global[index] = 0;

	// c. Assume we start from origin
	current_position_local[ROW_INDEX] = 0;
	current_position_local[COLUMN_INDEX] = 0;
	
  // d. While loop until we find the center or we can not reach to the center
	//    because of the ifinite loop causing by unvisited cell.
	//    if path_index >= MAZE_SIZE*MAZE_SIZE - 1 -> that means there is another shorter way need to explore
	while(((current_position_local[ROW_INDEX] != MAZE_SIZE -1)||
				(current_position_local[COLUMN_INDEX] != MAZE_SIZE - 1 ))&&
				(path_index < MAZE_SIZE*MAZE_SIZE - 2)){
		
		current_dist = maze_dist_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]];	
		
		// Check EAST neighbor
		if (!READ_B(maze_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]], EAST)){		
			east_neighbor_dist = maze_dist_array_global[current_position_global[ROW_INDEX]][current_position_global[COLUMN_INDEX]+1];
			if ((current_dist == (east_neighbor_dist + 1) )
				&& READ_B(maze_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX] + 1] ,VISITED)) 
			{
				next_position_local = EAST;
			}
		}
		
		// Check NORTH neighbor
		if (!READ_B(maze_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]], NORTH)){
			north_neighbor_dist = maze_dist_array_global[current_position_local[ROW_INDEX]-1][current_position_local[COLUMN_INDEX]];
			if ((current_dist == (north_neighbor_dist + 1)) 
				 && READ_B(maze_array_global[current_position_local[ROW_INDEX]-1][current_position_local[COLUMN_INDEX]] ,VISITED))
			{
				next_position_local = NORTH;
			}
		}
		
		// Check WEST neighbor
		if (!READ_B(maze_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]], WEST)){
			west_neighbor_dist = maze_dist_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]-1];
			if ((current_dist == (west_neighbor_dist + 1)) 
				&& READ_B(maze_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]-1] ,VISITED))
			{
				next_position_local = WEST;
			}
		}

		// Check SOUTH neighbor
		if (!READ_B(maze_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]], SOUTH)){
		  south_neighbor_dist = maze_dist_array_global[current_position_local[ROW_INDEX]+1][current_position_local[COLUMN_INDEX]];
			if ((current_dist == (south_neighbor_dist + 1))
				&& READ_B(maze_array_global[current_position_local[ROW_INDEX]+1][current_position_local[COLUMN_INDEX]] ,VISITED))
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
			current_position_local[COLUMN_INDEX]++;

		else if (current_direction_local == SOUTH)
			current_position_local[ROW_INDEX]++;

		else if (current_direction_local == WEST)
			current_position_local[COLUMN_INDEX]--;

		else if (current_direction_local == NORTH)
			current_position_local[ROW_INDEX]--;
		
		path_index ++;
	}
	// turn off indicator
	LED1_OFF;
	
	// return number of cell
	return path_index;
}

/* This is another method_ Disavange: change alot global variable. unoptimize
void store_path(){
	
	byte index, index2, current_dist, next_position_local, current_position_local[2];
	byte east_neighbor_dist, west_neighbor_dist, south_neighbor_dist, north_neighbor_dist;
	byte current_direction_local = EAST;
	
	LED1_ON;

	// 1. Initialize the maze.
	// a. Assume all cells are 255 far from destination.
	// b. Then set ultimate destination to 0
	for (index = 0; index< MAZE_SIZE; index++){
		for (index2 = 0; index2 < MAZE_SIZE; index2++){
				maze_dist_array_global[index][index2] = MAZE_SIZE*MAZE_SIZE - 1;
		}
	}
	maze_dist_array_global[MAZE_SIZE - 1][MAZE_SIZE - 1] = 0;

	// 2. Floodfill the whole maze
	// a. Set ultimate destination VISITED bit 0 so that floodfill will not change this.
	// b. Then set it to visited so that store path can reach
	CLR_B(maze_array_global[MAZE_SIZE - 1][MAZE_SIZE -1], VISITED);
	maze_floodfill();	
	SET_B(maze_array_global[MAZE_SIZE - 1][MAZE_SIZE -1], VISITED);

	// 3. Find the shortest path to the ultimate destination
	//use counter to keep track so we can use the path_1_global as a stack
	path_index = 0;
	
	//initialize the array for turning 
	for(index = 0; index < MAZE_SIZE * MAZE_SIZE; index++)
		path_run_global[index] = 0;

	//initialization from origin again
	current_position_local[ROW_INDEX] = 0;
	current_position_local[COLUMN_INDEX] = 0;
	

	while((current_position_local[ROW_INDEX] != MAZE_SIZE -1)||
				(current_position_local[COLUMN_INDEX] != MAZE_SIZE - 1 )) {
		
		current_dist = maze_dist_array_global[current_position_local[ROW_INDEX]][current_position_local[COLUMN_INDEX]];	
		
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

*/


/* 
 * This is 1 of 2 main finction in Runner class
 * This function navigate the mice to explore unexplored cell in the maze
 * Recode all wall information, direction and position.
 */
void Runner_explore(int speed ){

	byte STOP_FLAG = 0;
	byte RUN = FIRST_RUN;
	byte path_count;
	byte row_Dest = MAZE_SIZE-1, column_Dest = MAZE_SIZE-1;
	
	
	// Stop and wait for 1 second
	Driver_go_straight(0, 0);
	delay_ms(1000);
	
	
	// Waitting for start signal
	while (!READ_B(Driver_check_walls(), FRONT));
	LED1_OFF;
	while (READ_B(Driver_check_walls(), FRONT));
	LED1_ON;
	delay_ms(2000);
	

	// Initialize the maze.  row_Dest and collumn_Dest are in main. ??? I move it here
	maze_initialize(row_Dest, column_Dest);
	
	
	// Read first position. This position is given.. we dont need to read sensor
	Driver_go_straight(90, speed);
	maze_array_global[0][0] = 0x1E;
	current_position_global[COLUMN_INDEX] ++;
	
	while(!STOP_FLAG) {
		
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
			next_dist = MAZE_SIZE * MAZE_SIZE -1;	
			
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
				else{
					Driver_go_straight(0,0);
				}
				delay_ms(100);
				Driver_turn_right(0,TURN_RIGHT_ANGLE, speed);
				Driver_go_straight(0,0);
				delay_ms(50);
				Driver_go_straight(90, speed);

					

				current_direction_global = next_position;
				
			// Case 3: Next position is at the left	
			} else if (next_position == LEFT_DIRECT(current_direction_global)){
				
					Driver_go_straight(90, speed);
					// Now we are in the center of 1 cell. Check if there is a wall in front of us for make correction
					if (READ_B(walls_FLBR, FRONT))
						Driver_frontwall_correction();
					else{
						Driver_go_straight(0,0);
					}
					delay_ms(100);
					
					Driver_turn_left(0, TURN_LEFT_ANGLE, speed);
					Driver_go_straight(0,0);
					delay_ms(50);
					Driver_go_straight(90, speed);
					
				
				
				current_direction_global = next_position;
		
			// Case 4: Next position is in the back. Make a U turn
			} else if (next_position == BACK_DIRECT(current_direction_global )){
					
				Driver_go_straight(90, speed);
				// Now we are in the center of 1 cell. Check if there is a wall in front of us for make correction
				if (READ_B(walls_FLBR, FRONT))
						Driver_frontwall_correction();	

				Driver_turn_left(0,TURN_LEFT_ANGLE, speed);
				// Now we are in the center of 1 cell. Check if there is a wall in front of us for make correction
				if (READ_B(walls_FLBR, FRONT))
					Driver_frontwall_correction();		
				Driver_turn_left(0,TURN_LEFT_ANGLE, speed);

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

		} // While loop for first run
		
		// After reach destination.. We need to get the path to the ultimate destination first
		path_count = store_path();
		
		// Case 1: We already got a good path
		if (path_count < GOOD_PATH){
			// Ask the mouse to return the start point
			row_Dest = 0; 
			column_Dest = 0;
			
			// reinitialize the maze
			maze_initialize(row_Dest, column_Dest);
			
			//set start point as unvisited
			CLR_B(maze_array_global[row_Dest][column_Dest], VISITED);
			
			RUN = RETURN_RUN;
			
		// Case 2: Finish first explore. Set up for second explore
		} else if (RUN == FIRST_RUN){
			// Ask the mouse to explore more to the start point
			row_Dest = 0; 
			column_Dest = 0;
			
			// reinitialize the maze
			maze_initialize(row_Dest, column_Dest);
			//set start point and finish point as visited
			CLR_B(maze_array_global[row_Dest][column_Dest], VISITED);
			
			RUN = SECOND_RUN;
			
		// Case 3: On second run already but can not find good path.
		// Pray..
		} else if (RUN == SECOND_RUN) {
			
			Driver_go_straight(0, 0);
			delay_ms(1000);
		
			// Make a u-turn
			Driver_go_straight(90,speed);
			Driver_turn_left(0, TURN_LEFT_ANGLE, speed);
			Driver_frontwall_correction();
			Driver_turn_left(0, TURN_LEFT_ANGLE, speed);
			Driver_go_straight(0,0);
				
			STOP_FLAG = 1;
			
		// Case RETURN_RUN	
		} else if (RUN == RETURN_RUN){
			
			Driver_go_straight(0, 0);
			delay_ms(1000);
		
			// Make a u-turn
			Driver_go_straight(90,speed);
			Driver_turn_left(0, TURN_LEFT_ANGLE, speed);
			Driver_frontwall_correction();
			Driver_turn_left(0, TURN_LEFT_ANGLE, speed);
			Driver_go_straight(0,0);
				
			STOP_FLAG = 1;
			
		}
	}
	
	
} // End method
	



void Runner_run(int speed){

		byte path_count;
	
	  //path index is the size of the path_run_array
	
		Driver_go_straight(0,0);
		// Waitting for start signal
		while (!READ_B(Driver_check_walls(), FRONT));
		LED2_OFF;
		while (READ_B(Driver_check_walls(), FRONT));
		LED2_ON;
		delay_ms(2000);
		
		for(path_count = 1; path_count < path_index; path_count ++){
			switch(path_run_global[path_count]){
				case FRONT:
					if(path_run_global[path_count+1] == RIGHT){
						Driver_go_straight(90,speed);
						Driver_turn_right(90, 90, speed);
						Driver_go_straight(90,speed);
						//Driver_turn_right_onpost(90, speed);
						path_count ++;
					}
					else if(path_run_global[path_count+1] == LEFT){
						Driver_go_straight(90,speed);
						Driver_turn_left(90, 90, speed);
						Driver_go_straight(90,speed);
						//Driver_turn_left_onpost(90, speed);
						path_count++;
					}
					else
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


void Runner_run_onpost(int speed){

		byte path_count;
	
	  //path index is the size of the path_run_array
		for(path_count = 0; path_count < path_index; path_count ++){
			switch(path_run_global[path_count]){
				case FRONT:
					if(path_run_global[path_count+1] == RIGHT){
						Driver_turn_right_onpost(90, speed);
						Driver_go_straight(90,speed);
						path_count ++;
					}
					else if(path_run_global[path_count+1] == LEFT){
						Driver_turn_left_onpost(90, speed);
						Driver_go_straight(90,speed);
						path_count++;
					}
					else
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

