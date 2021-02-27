//
// Created by kjh on 2021-02-07.
//
#include <Arduino.h>
#include "StepperBody.hpp"

StepperBody::StepperBody(SettingStepper* _setting, void(*_dir)(int), void(*_pulse)()) {
    settingStepper = _setting;
    //dirPin = _setting->dirPin;
    //pulsePin = _setting->pulsePin;
    direction = _dir;
    pulse = _pulse;

    /// 최대 속도에서의 delay 값, 값이 작을 수록 빠르다
    delayValueAtMaxSpeed = _setting->delayValueAtMaxSpeed;

    delayCounter0 = _setting->delayCounter0;      // 가속 시작의 초기 delay 값
    stepPosition = 0;       // 스텝모터의 현재 위치

    dirValue = 0;           // stepPosition 값을 증가,감소 시키는 값
    totalSteps = 0;         // 움질일 총 스텝수
    movementDone = false;   // 이동 완료 플래그

    /// 가속이 끝날 때까지 스텝수. 가속이 끝날 때까지 0으로 유지됨
    rampUpStepCount = 0;

    /// 최대 속도에 도달할 때까지의 추정 스텝수
    float a = (delayValueAtMaxSpeed / (float) delayCounter0) * 0.676;
    float m = ((a * a - 1) / (-2 * a));
    estimatedStepsToMaxSpeed = m * m; // 최대 속도에 도달할 때까지의 추정 스텝수

    /// 이동이 완료될 때까지의 추정 시간(interrupt tick)
    estimatedTimeForMove = 0;
    rampUpStepTime = 0;

    /// 이 모터를 감속하여 다른 모터와 조정된 움직임을 만드는 데 사용됩니다.
    speedScale = 1;

    // per iteration variables (potentially changed every interrupt)
    delayCounterN = 0; // delayInterval 값을 구하기 위한 카운터 값
    delayInterval = delayCounter0; // 계산된 delay 값
    di = delayInterval; // 먼저 이동한 스텝모터에 의해 잘려진 변수
    stepCount = 0;  // 이동한 스텝수
}

SettingStepper::SettingStepper(unsigned int _dirPin, unsigned int _pulsePin,
                               unsigned int _delayCounter0,
                               unsigned long _delayValueAtMaxSpeed) {
    dirPin = _dirPin;
    pulsePin = _pulsePin;
    delayValueAtMaxSpeed = _delayValueAtMaxSpeed;
    delayCounter0 = _delayCounter0;

    pinMode(dirPin, OUTPUT);
    pinMode(pulsePin, OUTPUT);
}
