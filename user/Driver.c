#include "Driver.h"



void Driver_turn_left(int distance, int angle, int speed) {
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
	
	left_cnt = (radius*2 - MOUSE_WIDTH)*M_PI/E_4*LW_MM2COUNT_RATIO*angle/360;
	right_cnt = (radius*2 + MOUSE_WIDTH)*M_PI/E_4*RW_MM2COUNT_RATIO*angle/360;			

	if (radius != 0){
		left_spd = (2*radius - MOUSE_WIDTH)*speed/2/radius;
		right_spd = (2*radius + MOUSE_WIDTH)*speed/2/radius;
	} else {
		left_spd = -speed;
		right_spd = speed;
	}
	
	Controller_run(left_cnt, right_cnt, left_spd, right_spd);
	Controller_run(90 - radius, 90 - radius, speed, speed);

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
	
	Controller_run(left_cnt, right_cnt, left_spd, right_spd);
	Controller_run(90 - radius, 90 - radius, speed, speed);
	
}

/* return whether successful or not */
void Driver_go_straight(int distance, int speed) {
	Controller_run(distance*LW_MM2COUNT_RATIO, distance*RW_MM2COUNT_RATIO, speed, speed);
}



byte Driver_check_walls(void){
	return Controller_check_walls();
}




