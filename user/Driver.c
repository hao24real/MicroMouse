#include "Driver.h"
#include "delay.h"

#define COUNT_ERR 

void Driver_turn_left(int distance, int angle, int speed){
	// Declaration of local variables
	int left_cnt, right_cnt;
	int left_spd, right_spd;
	int radius = distance;
	
	// Rotate at the same position
	if (distance < 0){
		Controller_run(0,0,0,0);
		return;
	}
	
	// Go straight untill the turn position if distance is bigger than 1 tile
	if (distance > 90){
		Controller_run((distance-90)*LW_MM2COUNT_RATIO, (distance-90)*RW_MM2COUNT_RATIO, speed, speed);
		radius = 90;
	} else {
		radius = distance;
	}
	
	// Calculate left distance and right distance in counts
	left_cnt = (radius*2 - MOUSE_WIDTH)*M_PI/E_4*LW_MM2COUNT_RATIO*angle/360;
	right_cnt = (radius*2 + MOUSE_WIDTH)*M_PI/E_4*RW_MM2COUNT_RATIO*angle/360;

	// Make sure that left and right distance (coutns)is always POSITIVE
	if (left_cnt < 0) left_cnt = -left_cnt;
	if (right_cnt < 0) right_cnt = -right_cnt;

	// If Distance passed is 0, rotate the mice
	if (radius != 0){
		left_spd = (2*radius - MOUSE_WIDTH)*speed/2/radius;
		right_spd = (2*radius + MOUSE_WIDTH)*speed/2/radius;
	} else {
		left_spd = -speed;
		right_spd = speed;
	}
	
	// Cal propriate Controller method
	Controller_run(left_cnt, right_cnt, left_spd, right_spd);
	
	// Go straingt before return
	Controller_run(0, 0, speed, speed);
	
}

void Driver_turn_right(int distance, int angle, int speed) {
// Declaration of local variables
	int left_cnt, right_cnt;
	int left_spd, right_spd;
	int radius = distance;
	
	// Rotate at the same position
	if (distance < 0){
		Controller_run(0,0,0,0);
		return;
	}
	
	// Go straight untill the turn position if distance is bigger than 1 tile
	if (distance > 90){
		Controller_run((distance-90)*LW_MM2COUNT_RATIO, (distance-90)*RW_MM2COUNT_RATIO, speed, speed);
		radius = 90;
	} else {
		radius = distance;
	}
	

	
	left_cnt = (radius*2 + MOUSE_WIDTH)*M_PI/E_4*LW_MM2COUNT_RATIO*angle/360;
	right_cnt = (radius*2 - MOUSE_WIDTH)*M_PI/E_4*RW_MM2COUNT_RATIO*angle/360;			

	if (radius != 0){
		left_spd = (2*radius + MOUSE_WIDTH)*speed/2/radius;
		right_spd = (2*radius - MOUSE_WIDTH)*speed/2/radius;
	} else {
		left_spd = speed;
		right_spd = -speed;
	}
	
	// COntroller
	Controller_run(left_cnt, right_cnt, left_spd, right_spd);
	
	
	// Go straingt before return
	Controller_run(0, 0, speed, speed);
	
}


void Driver_turn_right_onpost(int angle, int speed){
	
	while (!Controller_check_right_wall()){
		Controller_run(0, 0, speed, speed);
	}
	while (Controller_check_right_wall()){

		Controller_run(0, 0, speed, speed);
	}
	
	Controller_run(30*LW_MM2COUNT_RATIO, 30*RW_MM2COUNT_RATIO, speed, speed);
	Driver_turn_right(90, angle, speed);
}



void Driver_turn_left_onpost(int angle, int speed){
	
	while (!Controller_check_right_wall()){
		Controller_run(0, 0, speed, speed);
	}
	while (Controller_check_right_wall()){

		Controller_run(0, 0, speed, speed);
	}
	
	Controller_run(30*LW_MM2COUNT_RATIO, 30*RW_MM2COUNT_RATIO, speed, speed);
	Driver_turn_left(90, angle, speed);
}




/* return whether successful or not */
void Driver_go_straight(int distance, int speed) {
	Controller_run(distance*LW_MM2COUNT_RATIO, distance*RW_MM2COUNT_RATIO, speed, speed);
}


void Driver_frontwall_correction(){
	Controller_frontwall_corecttion();
}


byte Driver_check_walls(void){
	return Controller_check_walls();
}




