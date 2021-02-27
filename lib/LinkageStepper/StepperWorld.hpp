//
// Created by kjh on 2021-02-07.
//

#ifndef STEPPER_MOTOR_STEPPERWORLD_HPP
#define STEPPER_MOTOR_STEPPERWORLD_HPP

#include <StepperBody.hpp>
#include <LiquidCrystal_I2C.h>

class StepperWorld
{
public:

    explicit StepperWorld(StepperBody* _body0);

    StepperWorld(StepperBody* _body0, StepperBody* _body1);

    StepperWorld(StepperBody* _body0, StepperBody* _body1, StepperBody* _body2);

    StepperWorld(StepperBody* _body0, StepperBody* _body1, StepperBody* _body2,
                 StepperBody* _body3);

    StepperWorld(StepperBody* _body0, StepperBody* _body1, StepperBody* _body2,
                 StepperBody* _body3, StepperBody* _body4);

    StepperWorld(StepperBody* _body0, StepperBody* _body1, StepperBody* _body2,
                 StepperBody* _body3, StepperBody* _body4, StepperBody* _body5);

    void timerInit();

    void test(unsigned int _0, unsigned int _1);

    // =================================================
    volatile unsigned short remainingSteppersFlag;
    volatile unsigned short nextStepperFlag;

    StepperBody* body[6];

    //========================================================
    void setNextInterruptInterval();

    void resetStepper(volatile unsigned short si);

    float
    getDurationOfAcceleration(volatile unsigned short s, unsigned int numSteps);

    void stepToDrgree(unsigned short _stepper, float degree);

    void stepperAToDegree(float degree);

    void stepperCToDegree(float degree);

    void prepareMovement(int whichMotor, long steps);

    void adjustSpeedScales();

    void runAndWait(void(* func)(), bool linkage = true);

};

extern StepperWorld World;

#endif