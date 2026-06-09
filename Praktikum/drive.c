#include "drive.h"
#include "motor.h"
#include "sonic.h"
#include "CMPS14.h"

static uint16_t initialHeading;

void driveInit(void){
	
	initialHeading = CMPS14_GetHeading();
	
}

void move(uint16_t speed){
			
	
		
			Motor_DriveForward(speed);
			
			if ( 300 > SonicGetDistanceMiddle()){
				if(SonicGetDistanceLeft() < SonicGetDistanceRight()){
					Motor_TurnCenteredClockwise(20);
				}else{
					Motor_TurnCenteredCounterClockwise(20);
				}
			}
			
			
			
}
	
	
	
	
	
void DriveHandler(EVENT_T event)
{
	
    switch (event.EventID)
    {
        case EVT_INIT_EVT:
            driveInit();
            break;

        case EVT_FAHREN_EVT:
						
					move(10);
						
            //Motor_DriveForward(100);
						//Motor_TurnCenteredClockwise(50);
						//Motor_TurnCenteredCounterClockwise(50);
            break; 
				case EVT_NOEVT:
					break;

        default:
            break;
    }
}