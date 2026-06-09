#include "drive.h"
#include "motor.h"
#include "sonic.h"
#include "CMPS14.h"

static uint16_t initialHeading;
static uint8_t avoiding = 0;
static uint32_t homingTimer = 0;
uint16_t speed = 0;

static uint16_t initialHeading;

void driveInit(void){
	
	initialHeading = CMPS14_GetHeading();
	
}

uint16_t fixedOrientaion(uint16_t desiredOrientation){
	uint16_t lasthead = ((initialHeading + desiredOrientation )%360);
	return lasthead;
}

void homing(uint16_t richtung){
		richtung = richtung / 10;
		if(richtung >= fixedOrientaion(90) && richtung <= fixedOrientaion(270)){
			homingTimer++;
			
			if(homingTimer > 200){
				if(richtung>fixedOrientaion(180)){
					Motor_TurnCenteredCounterClockwise(speed+20);
				}else{
					Motor_TurnCenteredClockwise(speed+20);
				}

			}
		}
		if(richtung == initialHeading){ 
					homingTimer = 0; 
					Motor_Stop();
					return;
				}
}

void move(uint16_t speed){
			
	
		
			//Motor_DriveForward(speed);
			
	/*		if ( 300 > SonicGetDistanceMiddle()){
				if(SonicGetDistanceLeft() < SonicGetDistanceRight()){
					Motor_TurnCenteredClockwise(speed+20);
				}else{
					Motor_TurnCenteredCounterClockwise(speed+20);
				}
			}*/
			homing(CMPS14_GetHeading());
			
			
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