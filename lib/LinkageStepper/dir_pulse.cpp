
#include <Arduino.h>
#include "StepperMotorDefine.hpp"

/// A axis 1 stepping pulse generation
void aStep() {
    A_STEP_HIGH
            delayMicroseconds(2);
    A_STEP_LOW
}

/// A axis direction selection
void aDir(int dir) {
    digitalWrite(A_DIR_PIN, dir);
}

/// C axis 1 stepping pulse generation
void cStep() {
    C_STEP_HIGH
            delayMicroseconds(2);
    C_STEP_LOW
}

/// C axis direction selection
void cDir(int dir) {
    digitalWrite(C_DIR_PIN, dir);
}