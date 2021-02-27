//
// Created by kjh on 2021-02-07.
//

#ifndef STEPPER_MOTOR_STEPPERBODY_HPP
#define STEPPER_MOTOR_STEPPERBODY_HPP

#include <StepperMotorDefine.hpp>
#include <Arduino.h>

class SettingStepper
{
public:
    SettingStepper(unsigned int _dirPin, unsigned int _pulsePin,
                   unsigned int _delayCounter0,
                   unsigned long _delayValueAtMaxSpeed);
    

    unsigned short dirPin;
    unsigned short pulsePin;
    unsigned int delayCounter0;
    unsigned long delayValueAtMaxSpeed;
};


class StepperBody
{
public:
    StepperBody(SettingStepper* _setting, void(*_dir)(int), void(*_pulse)());

    void (* direction)(int);   // 방향 설정
    void (* pulse)();    // one pulse 생성

// ===============================================
    SettingStepper* settingStepper;
    //unsigned short dirPin;
    //unsigned short pulsePin;
    //unsigned int acceleration; // 가속도

    /// 최대 속도에서의 delay 값, 값이 작을 수록 빠르다
    volatile unsigned long delayValueAtMaxSpeed;

    // derived parameters
    unsigned int delayCounter0; // 가속 시작의 초기 delay 값
    long stepPosition;   // 스텝모터의 현재 위치

    // per movement variables (only changed once per movement)
    volatile int dirValue;   // stepPosition 값을 증가,감소 시키는 값
    volatile unsigned long totalSteps;        // 움질일 총 스텝수
    volatile bool movementDone = false;      // 이동 완료 플래그

    /// 가속이 끝날 때까지 스텝수. 가속이 끝날 때까지 0으로 유지됨
    volatile unsigned long rampUpStepCount;

    /// 최대 속도에 도달할 때까지의 추정 스텝수
    volatile unsigned long estimatedStepsToMaxSpeed;

    /// 이동이 완료될 때까지의 추정 시간(interrupt tick)
    volatile unsigned long estimatedTimeForMove;
    volatile unsigned long rampUpStepTime;

    /// 이 모터를 감속하여 다른 모터와 조정된 움직임을 만드는 데 사용됩니다.
    volatile float speedScale;

    // per iteration variables (potentially changed every interrupt)
    volatile unsigned int delayCounterN; // delayInterval 값을 구하기 위한 카운터 값
    volatile float delayInterval; // 계산된 delay 값
    volatile unsigned long di; // 먼저 이동한 스텝모터에 의해 잘려진 변수
    volatile unsigned long stepCount;  // 이동한 스텝수
};

//======================================================================
//
//  rampUpStepCount->|<-->|                |    |
//                   |    |                |    |
//                   |    /----------------\    |
//                   |   /                 |\   |
//                   |  /                  | \  |
//                   | /                   |  \ |
//            -------|/                    |   \|---------
//                   |-- stepCount -->>    |    |
//                   |<-rampDownStartStep->|    |
//                   |<------ totalSteps ------>|
//
//======================================================================
//
//                |         |               |
//                |        /|---------------|\ <----- delayValueAtMaxSpeed
//                |      /  |               |  \
//                |    /    |               |    \
//                |  /      |               |      \
//         -------|/        |               |        \|---------
//  delayCounter->|- 증가 ->|   변동 없음    |- 감소 ->|
//
//======================================================================

#endif //STEPPER_MOTOR_STEPPERBODY_HPP
