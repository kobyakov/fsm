/*
 * https://www.codeproject.com/Articles/1275479/State-Machine-Design-in-C
 */
#include "StateMachine.h"
#include "Motor.h"

// Define motor objects
static Motor motorObj1;
static Motor motorObj2;

// Define two public Motor state machine instances
SM_DEFINE(Motor1SM, &motorObj1)
SM_DEFINE(Motor2SM, &motorObj2)

int main()
{
    MotorData* data;
    
    // Create event data
    data = SM_XAlloc(sizeof(MotorData));
    data->speed = 100;

    // Call MTR_SetSpeed event function to start motor
    SM_Event(Motor1SM, MTR_SetSpeed, data);

    // Call MTR_SetSpeed event function to change motor speed
    data = SM_XAlloc(sizeof(MotorData));
    data->speed = 200;
    SM_Event(Motor1SM, MTR_SetSpeed, data);

    // Stop the motor
    SM_Event(Motor1SM, MTR_Halt, NULL);

    // Stop motor again will be ignored
    SM_Event(Motor1SM, MTR_Halt, NULL);

    // Motor2SM example
    data = SM_XAlloc(sizeof(MotorData));
    data->speed = 300;
    SM_Event(Motor2SM, MTR_SetSpeed, data);
    SM_Event(Motor2SM, MTR_Halt, NULL);

    return 0;
}
