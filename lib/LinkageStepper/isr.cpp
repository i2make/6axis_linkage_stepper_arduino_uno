#include <StepperWorld.hpp>

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

/// OCR1A(TOP)에 도달하면 인터럽트 발생
ISR(TIMER1_COMPA_vect) {
    unsigned int tmpCtr = OCR1A; // 현재 OCR1A값을 tempCounter에 저장

    OCR1A = 65500; // 인터럽트가 걸리지 않도록 TOP을 뒤로 미룬다

    /// 스텝모터 수 만큼 반복: 전체를 반복하는 for문
    for (int i = 0; i < NUM_STEPPERS; i++) {

        /// 움직일 스텝모터가 아니면 넘어간다
        /// prepareMovement() 함수를 설정하지 않은 경우
        /// remainingSteppersFlag가 set되지 않음
        /// remainingSteppersFlag의 해당 스텝모터 비트가
        /// 0으로 설정되므로 넘어간다
        if (!((1 << i) & World.remainingSteppersFlag))
            continue; // 이동으로 설정되지 않은 스텝모터 스킵

        /// 딜레이 값이 가장 작은 스텝모터를 제외한 나머지 스텝모터들은
        /// 현재 움직이는 스텝모터의 di 값만 빼고 스킵한다
        /// 왜냐하면 움직일 것이 아니기 때문
        /// 딜레이 값이 가장 작은 스텝모터만 아래 펄스 생성, 가감속 제어를
        /// 실행한다
        /// 최소 딜레이 스텝모터는 setNextInterruptInterval()에 의해
        /// nextStepperFlag의 해당 스텝모터 비트가 set되었음
        if (!(World.nextStepperFlag & (1 << i))) {
            /// 움직일 스텝모터를 제외한 스텝모터는 OCR1A 값만 빼고 넘어간다
            World.body[i]->di -= tmpCtr;
            continue; // 스텝모터 스킵
        }

        // =====================================================================
        /// 펄스 생성
        /// 가감속 제어
        volatile StepperBody* s = World.body[i]; // 레퍼런스 생성

        /// 이동할 스텝수가 남아 있으면 펄스 생성
        if (s->stepCount < s->totalSteps) {
            /// 펄스 생성을 위해 함수포인터가 지정하는 함수를 실행(astep or cstep)
            s->pulse();
            s->stepCount++; // 진행된 스텝수 증가
            s->stepPosition += s->dirValue; // 현재 스텝모터 좌표 위치를 증감

            /// 이동이 완료되면
            if (s->stepCount >= s->totalSteps) {
                s->movementDone = true; // 이동 완료 설정
                /// 이동이 남아있는 스텝모터 플래그의 해당 비트 off
                World.remainingSteppersFlag &= ~(1 << i);
            }
        }

        // =========================================================
        /// 가속이 시작되면(움직이기 시작하면)
        if (s->rampUpStepCount == 0) {
            /// 다음 가속용 딜레이 값 계산을 위해 딜레이 카운터 증가
            s->delayCounterN++;
            /// 딜레이 값 계산
            s->delayInterval = s->delayInterval - (2 * s->delayInterval) /
                                                (4 * s->delayCounterN + 1);
            /// 최대 속도에 도달하면
            // delayInterval은 지연 시간이기 때문에
            // 저속에서 값이 크고 고속에서 값이 작다
            if (s->delayInterval <= s->delayValueAtMaxSpeed) {
                /// 최대 속도의 딜레이 값으로 고정
                s->delayInterval = s->delayValueAtMaxSpeed;
                /// 가속이 끝날 때까지의 스텝수를 저장
                s->rampUpStepCount = s->stepCount;
            }

            /// 가속중 최대속도에 도달하기 전에 감속해야 한다면
            // 이동 거리가 짧을 경우 등속 이동없이 감속으로 진입
            if (s->stepCount >= s->totalSteps / 2) {
                /// 가속이 끝날 때까지의 스텝수를 저장
                s->rampUpStepCount = s->stepCount;
            }
            s->rampUpStepTime += s->delayInterval;
        }

            /// 진행된 스텝수가 감속 시작 지점에 도달하면
        else if (s->stepCount >= s->totalSteps - s->rampUpStepCount) {
            /// 딜레이 값 계산
            s->delayInterval =
                    (s->delayInterval * (4 * s->delayCounterN + 1)) /
                    (4 * s->delayCounterN + 1 - 2);
            s->delayCounterN--; // 감속 딜레이 값 계산을 위해 카운더 감소
        } // end-else if

        s->di = s->delayInterval * s->speedScale; // 움직인 스텝모터
    } // end-for - 스텝모터 수만큼 반복

    World.setNextInterruptInterval(); // 다음에 이동할 스텝모터 선택, OCR1A설정
    TCNT1  = 0; // 카운터 다시 시작
} // end-ISR(TIMER1_COMPA_vect)