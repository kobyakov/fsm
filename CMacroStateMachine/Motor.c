#include "StateMachine.h"
#include "Motor.h"
#include <stdio.h>

enum States
{
    ST_IDLE,
    ST_STOP,
    ST_START,
    ST_CHANGE_SPEED,
    ST_MAX_STATES
};

STATE_DECLARE(Idle, NoEventData);
STATE_DECLARE(Stop, NoEventData);
STATE_DECLARE(Start, MotorData);
STATE_DECLARE(ChangeSpeed, MotorData);

BEGIN_STATE_MAP(Motor)
    STATE_MAP_ENTRY(ST_Idle)
    STATE_MAP_ENTRY(ST_Stop)
    STATE_MAP_ENTRY(ST_Start)
    STATE_MAP_ENTRY(ST_ChangeSpeed)
END_STATE_MAP(Motor)

EVENT_DEFINE(MTR_SetSpeed, MotorData)
{
    BEGIN_TRANSITION_MAP                        // - CurrentState -
        TRANSITION_MAP_ENTRY(ST_START)          // ST_Idle
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_Stop
        TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED)   // ST_Start
        TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED)   // ST_ChangeSpeed
    END_TRANSITION_MAP(Motor, pEventData)
}

EVENT_DEFINE(MTR_Halt, NoEventData)
{
    BEGIN_TRANSITION_MAP                        // - CurrentState -
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_Idle
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_Stop
        TRANSITION_MAP_ENTRY(ST_STOP)           // ST_Start
        TRANSITION_MAP_ENTRY(ST_STOP)           // ST_ChangeSpeed
    END_TRANSITION_MAP(Motor, pEventData)
}

STATE_DEFINE(Idle, NoEventData)
{
    printf("%s ST_Idle", self->name);
}

STATE_DEFINE(Stop, NoEventData)
{
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = 0;
    printf("%s ST_Stop: %d\n", self->name, pInstance->currentSpeed);
    SM_InternalEvent(ST_IDLE, NULL);
}

STATE_DEFINE(Start, ModorData)
{
    ASSERT_TRUE(pEventData);
    Motor* pInstance = SM_GetInstance(Motor);
    printf("%s ST_Start: %d\n", self->name, pInstance->currentSpeed);
}

STATE_DEFINE(ChangeSpeed, MotorData)
{
    ASSERT_TRUE(pEventData);
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = pEventData->speed;
    printf("%s ST_ChangeSpeed: %d\n", self->name, pInstance->currentSpeed);
}
