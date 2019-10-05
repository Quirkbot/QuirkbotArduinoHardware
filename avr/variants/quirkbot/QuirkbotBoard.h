#ifndef Board_h_
#define Board_h_

#define QB_BOARD true

// BOARD LOCATIONS -------------------------------------------------------------

// No Location
#define DISCONNECTED 999
#define NO_LOCATION	DISCONNECTED


// Built in LEDs
#define LM	106	// Left Mouth		(PD5)
#define RM	107	// Right Mouth		(PB0)
#define PLACE_LEFT_MOUTH LM
#define PLACE_RIGHT_MOUTH RM

#define LE	8	// Left Eye		 	(PB4)	ADC11
#define RE	A5	// Right Eye		(PF0)	ADC0
#define PLACE_LEFT_EYE LE
#define PLACE_RIGHT_EYE RE

// Limbs
#define LL	100	// Left Leg
#define RL	101	// Right Leg
#define RA	102	// Right Arm
#define H	103	// Horn
#define LA	105	// Left Arm
#define PLACE_LEFT_LEG LL
#define PLACE_RIGHT_LEG RL
#define PLACE_LEFT_ARM LA
#define PLACE_RIGHT_ARM RA
#define PLACE_HORN H

#define LLF	9	// Left Leg Front	(PB5)	ADC12	PWM (16BIT)
#define RLF	11	// Right Leg Front	(PB7)			PWM (8/16BIT)
#define RAF	5	// Right Arm Front	(PC6)			PWM (HS)
#define HF	13	// Horn Front		(PC7)			PWM (10BIT)
#define LAF	10	// Left Arm Front	(PB6)	ADC13	PWM (16BIT)
#define PLACE_LEFT_LEG_FRONT LLF
#define PLACE_RIGHT_LEG_FRONT RLF
#define PLACE_LEFT_ARM_FRONT LAF
#define PLACE_RIGHT_ARM_FRONT RAF
#define PLACE_HORN_FRONT HF

#define LLB	A0	// Left Leg Back	(PF7)	ADC7
#define RLB	A4	// Right Leg Back	(PF1)	ADC1
#define RAB	A3	// Right Arm Back	(PF4)	ADC4
#define HB	A2	// Horn Back		(PF5)	ADC5
#define LAB	A1	// Left Arm Back	(PF6)	ADC6
#define PLACE_LEFT_LEG_BACK LLB
#define PLACE_RIGHT_LEG_BACK RLB
#define PLACE_LEFT_ARM_BACK LAB
#define PLACE_RIGHT_ARM_BACK RAB
#define PLACE_HORN_BACK HB

// Backpack
#define BP1	A7	// Back Pack 1		(PD7)	ADC10	PWM (HS)	Uppmost left
#define BP2	A11	// Back Pack 2		(PD6)	ADC9
#define BP3	0	// Back Pack 3		(PD2)	RXD1
#define BP4	2	// Back Pack 4		(PD1)	SDA
#define BP5	3	// Back Pack 5		(PD0)	SCL
#define BP6	1 	// Back Pack 6		(PD3)	TXD1
#define PLACE_BP1 BP1
#define PLACE_BP2 BP2
#define PLACE_BP3 BP3
#define PLACE_BP4 BP4
#define PLACE_BP5 BP5
#define PLACE_BP6 BP6
#define SERVO_BP1 BP6
#define SERVO_BP2 BP3
#define SERVO_MOTOR_1 BP6
#define SERVO_MOTOR_2 BP3
#define PLACE_SERVO_MOTOR_1 BP6
#define PLACE_SERVO_MOTOR_2 BP3

// Pull up pin
#define PULL_UP_PIN 4

// UUID
#define QB_UUID_SIZE 16


// RX and TX -------------------------------------------------------------------
// Keep them off when doing serial communication
#undef TX_RX_LED_INIT
#undef TXLED0
#undef TXLED1
#undef RXLED0
#undef RXLED1
#define TX_RX_LED_INIT
#define TXLED0
#define TXLED1
#define RXLED0
#define RXLED1

#endif
