#include "pid.h"

#define LKP 1100
#define LKI 17
#define LKD 28
#define RKP 1100
#define RKI 17
#define RKD 28

#define FL_SENSOR 2500
#define FR_SENSOR 2500


int ERR_LEFT_TO_PID;
int ERR_RIGHT_TO_PID;

void pid_motor_speed(void){
	
	int new_EN; // new value of encoder counts
	int lSp,rSp;	// Speed of left motor
	int err, derivative;
	// static value for left motor
	static int left_old_EN = 0;
	static int left_old_speed = 0, left_old_err = 0, left_intergral = 0; // remember the old speed to detemine whther new speed is set
	//static value for right motor
	static int right_old_EN = 0;
	static int right_old_speed = 0,right_old_err = 0, right_intergral = 0; // remember the old speed to detemine whther new speed is set

	// calculate speed for left motor -------------------------------------------
	// Becareful with this line of code
	// Watch out if you reset the encoder value else where
	// BECAREFULL: this line of code trying to deal with Encoder overflow
	// When other code reset encoder reg, they will set this flag to 1
	// However, if this line of code happen at the middle between reset encoder and set this the FLAG ton 1
	// We will get into trouble. This function is inside interupt handler so this can be execute at any time in 
	// our code.
	// That is the problem.
	// We can by pass 2 cycle after RESET flag is on, just for safe. But is slower.

	new_EN = getLeftEncCount();
	lSp = new_EN - left_old_EN;	
	left_old_EN = new_EN;
	err = (leftspeed_global + ERR_LEFT_TO_PID) - lSp;

		
	// PID is going on here every systick
	derivative = (err - left_old_err);
	left_intergral = left_intergral + err*100 ;
	lSp = (LKP*err + LKI*left_intergral + LKD*derivative);
	lSp = lSp/1000; // Scale down cz we need very small change of KI
	left_old_err = err; // Store the old error
	// reset derivative value
	if (left_old_speed != leftspeed_global){
		left_old_speed = leftspeed_global;
		left_intergral = 0;
		left_old_err = 0;
	}

	// calculate speed for right motor -------------------------------------------
	// Becareful with this line of code
	// Watch out if you reset the encoder value else where
	new_EN = getRightEncCount();
	rSp = new_EN - right_old_EN;
	right_old_EN = new_EN;
	err = (rightspeed_global + ERR_RIGHT_TO_PID) - rSp;
	
	// PID is going on here every systick
	derivative = (err - right_old_err);
	right_intergral = right_intergral + err*100 ;
	rSp = (RKP*err + RKI*right_intergral + RKD*derivative);
	rSp = rSp/1000; // Scale down cz we need very small change of KI
	left_old_err = err; // Store the old error

	// reset derivative value
	if (right_old_speed != rightspeed_global){
		right_old_speed = rightspeed_global;
		right_intergral = 0;
		right_old_err = 0;
	}
	// Set the output value
	setLeftPwm(lSp);
	setRightPwm(rSp);
}


// This PID use for correction position of the mouse 
void front_wall_pid(){
		
}




