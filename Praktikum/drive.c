#include "drive.h"
#include "motor.h"
#include "sonic.h"
#include "CMPS14.h"

static uint16_t initialHeading;

static uint8_t avoiding = 0;
static uint32_t avoidTimer = 0;
static uint8_t avoidDirection = 0; // 0 = links, 1 = rechts

void driveInit(void){
	
	initialHeading = CMPS14_GetHeading();
	
}

void move(uint16_t speed)
{
    // Hindernis wird aktuell umfahren
    if(avoiding)
    {
        avoidTimer++;

        if(avoidDirection == 0)
        {
            Motor_TurnCenteredCounterClockwise(20);
        }
        else
        {
            Motor_TurnCenteredClockwise(20);
        }

        // Nach ca. 2 Sekunden wieder gerade fahren
        if(avoidTimer > 200)
        {
            avoiding = 0;
            avoidTimer = 0;

            Motor_DriveForward(speed);
        }

        return;
    }

    // Normalfahrt
    Motor_DriveForward(speed);

    if(SonicGetDistanceMiddle() < 300)
    {
        avoiding = 1;

        if(SonicGetDistanceLeft() < SonicGetDistanceRight())
        {
            avoidDirection = 1; // rechts drehen
        }
        else
        {
            avoidDirection = 0; // links drehen
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