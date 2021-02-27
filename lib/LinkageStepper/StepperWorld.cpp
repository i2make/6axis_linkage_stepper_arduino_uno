//
// Created by kjh on 2021-02-07.
//

#include "StepperWorld.hpp"

StepperWorld::StepperWorld(StepperBody* _body0) {
    body[0] = _body0;

    remainingSteppersFlag = 0;
    nextStepperFlag = 0;
}

StepperWorld::StepperWorld(StepperBody* _body0, StepperBody* _body1) {
    body[0] = _body0;
    body[1] = _body1;

    remainingSteppersFlag = 0;
    nextStepperFlag = 0;
}

StepperWorld::StepperWorld(StepperBody* _body0, StepperBody* _body1, StepperBody* _body2) {
    body[0] = _body0;
    body[1] = _body1;
    body[2] = _body2;

    remainingSteppersFlag = 0;
    nextStepperFlag = 0;
}

StepperWorld::StepperWorld(StepperBody* _body0, StepperBody* _body1,
                           StepperBody* _body2, StepperBody* _body3) {
    body[0] = _body0;
    body[1] = _body1;
    body[2] = _body2;
    body[3] = _body3;

    remainingSteppersFlag = 0;
    nextStepperFlag = 0;
}

StepperWorld::StepperWorld(StepperBody* _body0, StepperBody* _body1,
                           StepperBody* _body2, StepperBody* _body3,
                           StepperBody* _body4) {
    body[0] = _body0;
    body[1] = _body1;
    body[2] = _body2;
    body[3] = _body3;
    body[4] = _body4;

    remainingSteppersFlag = 0;
    nextStepperFlag = 0;
}

StepperWorld::StepperWorld(StepperBody* _body0, StepperBody* _body1,
                           StepperBody* _body2, StepperBody* _body3,
                           StepperBody* _body4, StepperBody* _body5) {
    body[0] = _body0;
    body[1] = _body1;
    body[2] = _body2;
    body[3] = _body3;
    body[4] = _body4;
    body[5] = _body5;

    remainingSteppersFlag = 0;
    nextStepperFlag = 0;
}

void StepperWorld::test(unsigned int _0, unsigned int _1) {
    for (uint16_t i = 0; i < _0; i++) {
        body[0]->pulse();
        delay(10);
    }
    for (uint16_t i = 0; i < _1; i++) {
        body[1]->pulse();
        delay(10);
    }
}

void StepperWorld::timerInit() {
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    OCR1A = 1000;                             // compare value
    TCCR1B |= (1 << WGM12);                   // CTC mode OCR1A(TOP)
    TCCR1B |= ((1 << CS11) | (1 << CS10));    // clk/64 prescaler
    interrupts();
}

// =============================================================================
// =============================================================================
// =============================================================================


/// 여러개의 스텝모터 중 다음에 펄스를 줘야 할 스텝모터를 선택한다
/// 선택된 스템모터의 딜레이 값을 OCR1A 값으로 설정
void StepperWorld::setNextInterruptInterval() {

    unsigned long minNextDelay = 999999; // 임시 변수
    // =========================================================================
    /// 이동해야 할 스텝모터 중 가장 작은 di 값을 찾는다
    /// 가장 먼저 움직여야 할 스텝모터의 di를 minimumDelay로 설정
    for (int i = 0; i < NUM_STEPPERS; i++) {
        if ( ((1 << i) & remainingSteppersFlag) && body[i]->di < minNextDelay ) {
            minNextDelay = body[i]->di;
        }
    } // end-for
    // =========================================================================
    nextStepperFlag = 0; // 다음에 가장 먼저 움직여야 할 스텝모터 플래그
    // ====================================================================
    /// 다음에 가장 먼저 움직여야 할 스텝모터를 플래그에 set한다
    for (int i = 0; i < NUM_STEPPERS; i++) {
        /// 스텝모터 중 minimumDelay값으로 설정된 스텝모터를 찾아
        /// nextStepperFlag를 set한다
        if ( ((1 << i) & remainingSteppersFlag) && body[i]->di == minNextDelay )
            nextStepperFlag |= (1 << i);
    } // end-for
    // =========================================================================
    if (remainingSteppersFlag == 0) {
        TIMER1_INTERRUPTS_OFF
                OCR1A = 65500;
    }

    OCR1A = minNextDelay; // 가장 먼저 움직여야 할 스텝모터의 딜레이 값
} // end-setNextInterruptInterval()

void StepperWorld::resetStepper(volatile unsigned short si) {
    body[si]->delayInterval = body[si]->delayCounter0;    // 현재 delay 값
    body[si]->di = body[si]->delayInterval;
    body[si]->stepCount = 0;                       // 진행된 스텝 수
    body[si]->delayCounterN = 0;                   // 가감속 진행된 delay 카운터 수(Cn)
    body[si]->rampUpStepCount = 0;                 // 가속이 끝났을 때의 스텝 수
    body[si]->movementDone = false;                // 이동 완료 플래그
    body[si]->speedScale = 1;

    float a = body[si]->delayValueAtMaxSpeed / (float)body[si]->delayCounter0;
    a *= 0.676;

    float m = ((a*a - 1) / (-2 * a));
    float n = m * m;

    body[si]->estimatedStepsToMaxSpeed = n; // 최대 속도까지 도달 예상 스텝
}

float StepperWorld::getDurationOfAcceleration(volatile unsigned short s, unsigned int numSteps) {
    float d = body[s]->delayCounter0;
    float totalDuration = 0;
    for (unsigned int n = 1; n < numSteps; n++) {
        d = d - (2 * d) / (4 * n + 1);
        totalDuration += d;
    }
    return totalDuration;
}

void StepperWorld::stepToDrgree(unsigned short _stepper, float degree) {
    long steps = degree * A_SPD;
    prepareMovement(0, steps);
}

void StepperWorld::stepperAToDegree(float degree) {
    long steps = degree * A_SPD;
    prepareMovement(0, steps);
}

void StepperWorld::stepperCToDegree(float degree) {
    long steps = degree * C_SPD;
    prepareMovement(1, steps);
}

/// 이동을 위한 모든 변수를 설정만 하는 함수
void StepperWorld::prepareMovement(int whichMotor, long _steps) {

    // 현재위치에서 원하는 위치로 이동하기 위한 스텝수
    long steps = _steps - body[whichMotor]->stepPosition;

    body[whichMotor]->direction( steps < 0 ? HIGH : LOW ); // 방향 설정 함수 호출
    body[whichMotor]->dirValue = steps > 0 ? 1 : -1; // 방향 값 설정
    body[whichMotor]->totalSteps = abs(steps); // 총 스텝수 설정
    resetStepper(whichMotor); // 스텝모터 변수 초기화 함수 호출

    remainingSteppersFlag |= (1 << whichMotor); // 움직일 스텝모터 추가

    unsigned long stepsAbs = abs(steps);

    if ((2 * body[whichMotor]->estimatedStepsToMaxSpeed) < stepsAbs ) {
        // there will be a period of time at full speed
        unsigned long stepsAtFullSpeed = stepsAbs - 2 * body[whichMotor]->estimatedStepsToMaxSpeed;
        float accelDecelTime = getDurationOfAcceleration(whichMotor, body[whichMotor]->estimatedStepsToMaxSpeed);
        body[whichMotor]->estimatedTimeForMove = 2 * accelDecelTime + stepsAtFullSpeed * body[whichMotor]->delayValueAtMaxSpeed;
    }
    else {
        // will not reach full speed before needing to slow down again
        float accelDecelTime = getDurationOfAcceleration(whichMotor, stepsAbs / 2 );
        body[whichMotor]->estimatedTimeForMove = 2 * accelDecelTime;
    }
}

void StepperWorld::adjustSpeedScales() {
    float maxTime = 0;

    for (int i = 0; i < NUM_STEPPERS; i++) {
        // 움직일 모터가 아니면 스킵
        if ( ! ((1 << i) & remainingSteppersFlag) )
            continue;
        // 스텝모터 중 가장 긴 시간이 걸리는 모터의 est..Move 값을 maxTime에 설정
        if (body[i]->estimatedTimeForMove > maxTime )
            maxTime = body[i]->estimatedTimeForMove;
    } // end-for

    if ( maxTime != 0 ) {
        for (int i = 0; i < NUM_STEPPERS; i++) {
            // 움직일 모터가 아니면 스킵
            if (!((1 << i) & remainingSteppersFlag) )
                continue;
            // 각 스텝모터의 speedScale을 최대 시간의 퍼센트로 설정
            body[i]->speedScale = maxTime / body[i]->estimatedTimeForMove;
        } // end-for
    } // end-if
}

void StepperWorld::runAndWait(void(*func)(), bool linkage) {
    if (linkage)
        adjustSpeedScales();
    setNextInterruptInterval();
    TIMER1_INTERRUPTS_ON
    while (remainingSteppersFlag) {
        func();
    }
    func();
    //remainingSteppersFlag = 0;
    //nextStepperFlag = 0;
    //TIMER1_INTERRUPTS_OFF
}


