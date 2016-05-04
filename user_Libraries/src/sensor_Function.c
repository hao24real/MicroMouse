//#include "global.h"
#include "sensor_Function.h"
#include "adc.h"
#include "delay.h"
#include "pwm.h"
#include "matrixDisplay.h"
#include "led.h"
#include <stdio.h>
#include "encoder.h"

int reflectionRate = 1000;//which is 1.000 (converted to ingeter)

int32_t volMeter = 0;
int32_t voltage = 0;
int32_t FLSensor = 0;
int32_t FRSensor = 0;
int32_t DLSensor = 0;
int32_t DRSensor = 0;
int32_t Outz = 0;
int32_t aSpeed = 0;//angular velocity
int32_t angle = 0; 


/*read IR sensors*/
void readSensor(void)
{
	u32 curt;
//read DC value		
	
	FLSensor = read_LF_Sensor;	
	FRSensor = read_RF_Sensor;	
	DLSensor = read_DL_Sensor;
	DRSensor = read_DR_Sensor;	
	
	curt = micros();
	
//left front sensor
	FH_EM_ON;
	elapseMicros(50,curt);
	FLSensor = read_LF_Sensor - FLSensor;
	FH_EM_OFF;
	if(FLSensor < 0)//error check
		FLSensor = 0;
 	elapseMicros(150,curt);
//right front sensor	
	FH_EM_ON;
	elapseMicros(200,curt);	
	FRSensor = read_RF_Sensor - FRSensor;
	FH_EM_OFF;
	if(FRSensor < 0)
		FRSensor = 0;
 	elapseMicros(300,curt);
// left diagonal sensors
	LD_EM_ON;
	elapseMicros(350,curt);	
	DLSensor = read_DL_Sensor - DLSensor;
	LD_EM_OFF;
	if(DLSensor < 0)
		DLSensor = 0;
	elapseMicros(400,curt);
// right diagonal sensors
	RD_EM_ON;
	elapseMicros(450,curt);	
	DRSensor = read_DR_Sensor - DRSensor;
	RD_EM_OFF;
	if(DRSensor < 0)
		DRSensor = 0;
	elapseMicros(550,curt);	

	FLSensor = FLSensor*reflectionRate/1000;
	FRSensor = FRSensor*reflectionRate/1000;
	DLSensor = DLSensor*reflectionRate/1000;
	DRSensor = DRSensor*reflectionRate/1000;
}


/*read gyro*/
void readGyro(void)
{	                      //k=19791(sum for sample in 1 second)    101376287 for 50 seconds with 5000 samples
	int i;
	int sampleNum = 20;
	aSpeed = 0;
	for(i=0;i<sampleNum;i++)
	aSpeed += read_Outz;
  aSpeed *= 50000/sampleNum;
	aSpeed -= 92980000;
	aSpeed /= 50000;	
	aSpeed /= 4;
	angle += aSpeed; 
}


/*read voltage meter*/
void readVolMeter(void)
{          //3240 = 7.85V
	volMeter = read_Vol_Meter;//raw value
	voltage = volMeter*809/3248;//actual voltage value  ex) 8.2V = 8200
}


void lowBatCheck(void)
{
  if(voltage < 700) //alert when battery Voltage lower than 7V
	{	
		
		setLeftPwm(0);
		setRightPwm(0);
		
		while(1)
		{
			displayMatrix("Lbat");
			//beep_on;      
			ALL_LED_OFF;
			delay_ms(200);

			clearScreen();
			//beep_off;      
			ALL_LED_ON;
			delay_ms(200);			
		}
	}
  else {
		  displayInt(voltage);
		  delay_ms(1000);
	}		
}


