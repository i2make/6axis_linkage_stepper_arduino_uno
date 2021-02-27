
#ifndef STEPPER_MOTOR_STEPPERMOTORDEFINE_HPP
#define STEPPER_MOTOR_STEPPERMOTORDEFINE_HPP

#define NUM_STEPPERS 2

// S01
#define A01                 90
#define C01                 100
// S02
#define A02                 0
#define C02                 215
// S03
#define A03                 100
#define C03                 270
// S04
#define A04                 15
#define C04                 180
// S05
#define A05                 75
#define C05                 100
// S06
#define A06                 60
#define C06                 45
// S07
#define A07                 45
#define C07                 30
// S08
#define A08                 30
#define C08                 360
// S09
#define A09                 0
#define C09                 350
// S10
#define A10                 -10
#define C10                 300
// S11
#define A11                 90
#define C11                 250
// S12
#define A12                 80
#define C12                 200
// S13
#define A13                 20
#define C13                 20
// S14
#define A14                 20
#define C14                 10
// S15
#define A15                 10
#define C15                 10
// S16
#define A16                 0
#define C16                 0

#define A_PPR               (long)2000 // pulse per rotation for stepper motor
#define C_PPR               (long)2000 // pulse per rotation for stepper motor
#define A_GEAR_RATIO        (long)100 // A axis gear ratio
#define C_GEAR_RATIO        (long)50 // C axis gear ration

#define A_SPD               float(A_PPR * A_GEAR_RATIO / 360) // steps per dgree
#define C_SPD               float(C_PPR * C_GEAR_RATIO / 360) // steps per dgree

#define A_DIR_PIN          2   // PD2(audiono digital pin 2)
#define A_STEP_PIN         3   // PD3(audiono digital pin 3)

#define C_DIR_PIN          4   // PD4(audiono digital pin 4)
#define C_STEP_PIN         5   // PD5(audiono digital pin 5)

#define A_STEP_HIGH             PORTD |=  0b00001000; // PD3 pin HIGH
#define A_STEP_LOW              PORTD &= ~0b00001000; // PD3 pin LOW

#define C_STEP_HIGH             PORTD |=  0b00100000; // PD5 pin HIGH
#define C_STEP_LOW              PORTD &= ~0b00100000; // PD5 pin LOW

// 타이머 1 인터럽트
#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);

#endif //STEPPER_MOTOR_STEPPERMOTORDEFINE_HPP
