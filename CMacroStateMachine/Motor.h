#ifndef MOTOR_H
#define MOTOR_H

#include "StateMachine.h"

typedef struct 
{
    int currentSpeed;
} Motor;

typedef struct
{
    int speed;
} MotorData;

EVENT_DECLARE(MTR_SetSpeed, MotorData);
EVENT_DECLARE(MTR_Halt, NoEventData);

#endif