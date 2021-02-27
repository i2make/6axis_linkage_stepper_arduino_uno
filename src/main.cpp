
#include <LiquidCrystal_I2C.h>
#include <KeyPad4X4.hpp>
#include "isr.cpp"
#include <StepperWorld.hpp>
#include "dir_pulse.cpp"


volatile uint8_t pressedKey = 0;

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

volatile uint8_t oldPressedKey = 67;
String buttonName = "S16";

// ============ generate instance ============================
SettingStepper setting1(A_DIR_PIN, A_STEP_PIN, 300, 5);
StepperBody Stepper1(&setting1, aDir, aStep);

SettingStepper setting2(C_DIR_PIN, C_STEP_PIN, 300, 5);
StepperBody Stepper2(&setting2, cDir, cStep);

StepperWorld World(&Stepper1, &Stepper2);


void display() {
    lcd.setCursor(3, 0);
    lcd.print("       ");
    lcd.setCursor(3, 0);
    lcd.print(World.body[0]->stepPosition * (360.0 / (A_PPR * A_GEAR_RATIO)), 3);
    //lcd.print(World.body[0]->stepPosition);
    lcd.setCursor(3, 1);
    lcd.print("       ");
    lcd.setCursor(3, 1);
    lcd.print(World.body[1]->stepPosition * (360.0 / (C_PPR * C_GEAR_RATIO)), 3);
    //lcd.print(World.body[1]->stepPosition);
    //delay(100);
}

void displayButton(const String& button) {
    lcd.setCursor(13, 1);
    lcd.print("   ");
    lcd.setCursor(13, 1);
    buttonName = button;
    lcd.print(buttonName);
}

void stepperGo(long A, long C) {
    if ((A * A_SPD) != World.body[0]->stepPosition)
        World.stepperAToDegree(A);
    if ((C * C_SPD) != World.body[1]->stepPosition)
        World.stepperCToDegree(C);
}

void setup() {
    World.timerInit();

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("A= ");
    lcd.setCursor(0, 1);
    lcd.print("C= ");

    keypadInit();

    World.runAndWait(display);

    lcd.setCursor(13, 0);
    lcd.print("   ");
    lcd.setCursor(13, 0);
    lcd.print("END");

    displayButton(buttonName);
}


void loop() {
    pressedKey = 0;
    if (GetKey() != 0) {
        if (pressedKey != oldPressedKey) {

            oldPressedKey = pressedKey;

            lcd.setCursor(13, 0);
            lcd.print("   ");
            lcd.setCursor(13, 0);
            lcd.print("RUN");

            switch (pressedKey) {
                case 'C':
                    stepperGo(A16, C16);
                    displayButton("S16");
                    World.runAndWait(display);
                    break;
                case '/':
                    stepperGo(A15, C15);
                    displayButton("S15");
                    World.runAndWait(display);
                    break;
                case '*':
                    stepperGo(A14, C14);
                    displayButton("S14");
                    World.runAndWait(display);
                    break;
                case '<':
                    stepperGo(A13, C13);
                    displayButton("S13");
                    World.runAndWait(display);
                    break;
                case '7':
                    stepperGo(A12, C12);
                    displayButton("S12");
                    World.runAndWait(display);
                    break;
                case '8':
                    stepperGo(A11, C11);
                    displayButton("S11");
                    World.runAndWait(display);
                    break;
                case '9':
                    stepperGo(A10, C10);
                    displayButton("S10");
                    World.runAndWait(display);
                    break;
                case '-':
                    stepperGo(A09, C09);
                    displayButton("S09");
                    World.runAndWait(display);
                    break;
                case '4':
                    stepperGo(A08, C08);
                    displayButton("S08");
                    World.runAndWait(display);
                    break;
                case '5':
                    stepperGo(A07, C07);
                    displayButton("S07");
                    World.runAndWait(display);
                    break;
                case '6':
                    stepperGo(A06, C06);
                    displayButton("S06");
                    World.runAndWait(display);
                    break;
                case '+':
                    stepperGo(A05, C05);
                    displayButton("S05");
                    World.runAndWait(display);
                    break;
                case '1':
                    stepperGo(A04, C04);
                    displayButton("S04");
                    World.runAndWait(display);
                    break;
                case '2':
                    stepperGo(A03, C03);
                    displayButton("S03");
                    World.runAndWait(display);
                    break;
                case '3':
                    stepperGo(A02, C02);
                    displayButton("S02");
                    World.runAndWait(display);
                    break;
                case '(':
                    stepperGo(A01, C01);
                    displayButton("S01");
                    World.runAndWait(display);
                    break;
                default:
                    break;
            } // end switch
        } // end if

        lcd.setCursor(13, 0);
        lcd.print("   ");
        lcd.setCursor(13, 0);
        lcd.print("END");
    }
} // end loop

