#include "mte220.c"
 
//SERVO_LEFT_STOP	(uns16)3520 stop speed left
//SERVO_RIGHT_STOP (uns16)3525  stop speed right

#define magnet_high 0xa3  // 0xa3  0x99
#define magnet_low 0x5b  // 0x5b  0x66

#define line_high 0x7f    //0xa3   0x8f
#define line_low  0x69    //0x5b   0x59

#define servoL1 3495
#define servoL2 3445
#define servoL3 3370
#define servoL4 3270
#define servoL5 3120
#define servoL6 2920
//2916
#define servoR1 3550 //+25
#define servoR2 3600 //+50
#define servoR3 3675 //+75
#define servoR4 3775 //+100
#define servoR5 3925 //+150
#define servoR6 4100 //+200

void ramp_down(){
	SetLeft(servoL6);
	SetRight(servoR6);
	LongDelay(1);
	SetLeft(servoL5);
	SetRight(servoR5);
	LongDelay(1);
	SetLeft(servoL4);
	SetRight(servoR4);
	LongDelay(1);
	SetLeft(servoL3);
	SetRight(servoR3);
	LongDelay(1);
	SetLeft(servoL2);
	SetRight(servoR2);
	LongDelay(1);
	SetLeft(servoL1);
	SetRight(servoR1);
	LongDelay(1);
	BothServosOff
}

void ramp_up(){
	SetLeft(servoL1);
    SetRight(servoR1);
    LongDelay(1);
    SetLeft(servoL2);
    SetRight(servoR2);
    LongDelay(1);
    SetLeft(servoL3);
    SetRight(servoR3);
    LongDelay(1);
    SetLeft(servoL4);
    SetRight(servoR4);
    LongDelay(1);
	SetLeft(servoL5);
    SetRight(servoR5);
    LongDelay(1);
	SetLeft(servoL6);
    SetRight(servoR6);
    LongDelay(1);
}


void main(void)
{
    uns8 line_value;  // current ADC value
    uns8 magnet_value;  // current ADC value
    uns8 detected = 0;
    //uns16 getAway1 = 500;
    //uns16 getAway2 = 65000;
    

//4116
    Initialization();
    
    WaitForButton();

    // initially both servos are on
    UseServos         // (syntax of "call" is correct without () or ;)
    
	ramp_up();

    while (1)  // loop forever
    {
        line_value = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp

        if ( line_value < line_low )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
        {
            // left servo only
            LeftServoOn
            RightServoOff
        }
        else if ( line_value > line_high )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
        {
            // right servo only
            RightServoOn
            LeftServoOff
        }
        else
        {
            // both servos on
            BothServosOn
        }
        
        // get analog value from the Hall effect sensor
        magnet_value = AnalogConvert(ADC_HALL_EFFECT);
		uns8 time = 0;
		
		if (magnet_value > magnet_high || magnet_value < magnet_low){
			LongDelay(1);
			magnet_value = AnalogConvert(ADC_HALL_EFFECT);
			if (magnet_value > magnet_high || magnet_value < magnet_low){
				detected = 1;

				ramp_down();
				
				if (magnet_value > magnet_high){
					OnLED
		        	LongDelay(50);
					OffLED
				}
				
				if (magnet_value < magnet_low){
					while (time < 13){
						OnLED
			        	LongDelay(2);
			        	OffLED
			        	LongDelay(2);
			        	time++;
		        	}
				}
				OffLED
				
				ramp_up();
				
				/*
				
				int time2 = 0;
				
				while(time2 < getAway1)
				{
				
					int time3 = 0;
					
					
					while(time3 < getAway2)
					{
					*/
					while (detected == 1){
						
				    	line_value = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp
				
				        if ( line_value < line_low )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
				        {
				            // left servo only
				            LeftServoOn
				            RightServoOff
				        }
				        else if ( line_value > line_high )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
				        {
				            // right servo only
				            RightServoOn
				            LeftServoOff
				        }
				        else
				        {
				            // both servos on
				            BothServosOn
				        }
				        
				        magnet_value = AnalogConvert(ADC_HALL_EFFECT);
				        
				        if (magnet_value < magnet_high || magnet_value > magnet_low)
				        {
				        	detected = 0;
						}
				        
				        /*
				        time3++;
				        
			        }
			        
			        time2++;
			        */
				}
			}
	    }
	    
	}
}
