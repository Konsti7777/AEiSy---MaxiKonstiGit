#include "drive.h"
#include "motor.h"
#include "sonic.h"
#include "CMPS14.h"
#include "delay.h"

static uint16_t initialHeading;
static uint8_t targetTimer = 0;
static uint32_t homingTimer = 0;
uint16_t speed = 0;
static _Bool inhoming = 0;
static uint16_t turnLeftCounter = 0;
static uint16_t turnRightCounter = 0;
static char driveModeIndicator = 'D';
static _Bool wiggle = 0;
static int wiggleTimer = 0;

void driveInit(void){
	
	initialHeading = CMPS14_GetHeading()/10;
	driveModeIndicator = 'D';
	
}

char DriveGetModeIndicator(void){
	return driveModeIndicator;
}

uint16_t fixedOrientaion(uint16_t desiredOrientation){
	uint16_t lasthead = ((initialHeading + desiredOrientation )%360);
	return lasthead;
}

uint16_t getInitialheading(void){
		return initialHeading;
}


static _Bool headingInRange(uint16_t heading, uint16_t start, uint16_t end){
		if(start <= end){
			return (heading >= start) && (heading <= end);
		}
		return (heading >= start) || (heading <= end);
}



void homing(uint16_t richtung){
		richtung = richtung / 10;
		if(headingInRange(richtung, fixedOrientaion(90), fixedOrientaion(270))){
			homingTimer++;
			
			if(homingTimer > 100){
				driveModeIndicator = 'H';
				inhoming = 1;
				if(richtung>fixedOrientaion(180)){
					Motor_TurnCenteredClockwise(30);
				}else{
					Motor_TurnCenteredCounterClockwise(30);
				}
				return;
			}
		}
		if(headingInRange(richtung, (initialHeading + 360 - 3) % 360, (initialHeading + 3) % 360)){
					homingTimer = 0; 
					//Motor_Stop();
					inhoming = 0;
					driveModeIndicator = 'D';
					return;
				}
}


void targeting(uint16_t richtung){
		richtung = richtung/10;
		targetTimer++;
		driveModeIndicator = 'T';
	if(SonicGetDistanceLeft() > 300 && SonicGetDistanceMiddle() > 300 && SonicGetDistanceRight() > 300 && targetTimer > 10){
		if(richtung < initialHeading-2 || richtung > initialHeading+2){
		if(richtung > initialHeading){
				Motor_TurnCenteredCounterClockwise(20);
				}else{
					Motor_TurnCenteredClockwise(20);
				}
			}
		targetTimer = 0;
		
	}
	
}



void move(uint16_t speed){
			if(!wiggle){
			if(!inhoming){
				//driveModeIndicator = 'D';
				Motor_DriveForward(speed);
			if ( SonicGetDistanceMiddle() < 300 ||SonicGetDistanceLeft() <= 150||SonicGetDistanceRight() <= 150){
					driveModeIndicator = 'O';
				if(SonicGetDistanceLeft() < SonicGetDistanceRight()){
					Motor_TurnCenteredClockwise(speed+20);
					//delayms(100);
					turnRightCounter++;
					targetTimer=0;
				}else{
					Motor_TurnCenteredCounterClockwise(speed+20);
					//delayms(100);
					turnLeftCounter++;
					targetTimer=0;
				}
			}
			targeting(CMPS14_GetHeading());
		}
			
			homing(CMPS14_GetHeading());
	}	
		
		if (turnLeftCounter >= 10 || turnRightCounter >= 10)
{
    uint16_t diff = (turnLeftCounter > turnRightCounter)
        ? (turnLeftCounter - turnRightCounter)
        : (turnRightCounter - turnLeftCounter);

    if (diff <= 4)
    {
        Motor_TurnCenteredCounterClockwise(100);
				wiggle = 1;
    }
		turnLeftCounter = 0;
		turnRightCounter = 0;
}

	if (wiggle){
		for(int i = 0; i<1000000; i++){
			driveModeIndicator = 'W';
			wiggleTimer++;
		}
		if (wiggleTimer >= 1000000){
			wiggle = 0;
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
						
					move(80);
						
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