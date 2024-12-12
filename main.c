#include "gpio.h"
#include "encoder.h"
unsigned long speed1;
unsigned long speed2;

#include "motor.h"
#include "gpio.h"

//#define P0 2 //M1A
#define P1 3 //M1B
#define P2 4 //M2A
#define P3 31
#define P0 2
//#define P16 34 //M2B
//#define P09 9
//#define P11 23
//#define P14 1
#define P4 28
#define P5 14	
#define P6 37
#define P7 11

#define M1A P1
#define M1B P0
#define M2A P2
#define M2B P3
#define ENC11 P4
#define ENC12 P5
#define ENC21 P6
#define ENC22 P7

#define Kp 2.0 

#define TARGET_SPEED 50 // Adjust according to your units (e.g., RPM)

unsigned long speed1 = 0; // Measured speed of motor 1
unsigned long speed2 = 0; // Measured speed of motor 2


void update_motor_speed(int motor1_speed, int motor2_speed) {
    // Clamp motor speeds to valid range (0-100 for PWM duty cycle)
    if (motor1_speed > 100) motor1_speed = 100;
    if (motor1_speed < 0) motor1_speed = 0;
    if (motor2_speed > 100) motor2_speed = 100;
    if (motor2_speed < 0) motor2_speed = 0;

    motor_on(MOTOR_FORWARD, motor1_speed, MOTOR_FORWARD, motor2_speed);
}


int main(void) {
    motor_init(M1A, M1B, M2A, M2B);
	  //motor_on(MOTOR_FORWARD, 20, MOTOR_FORWARD, 0);

	  int pin1 = 37; 
    int pin2=28 ;   // Example pin number for GPIO interrupt
    //int edge = 1;        // Rising edge trigger (example: configure as needed)
    //int event = 0;       // Use GPIOTE channel 0 (example)
    encoder_init(pin1,pin2);
    // Configure the pin as an input with pull-up resistor
    //pinMode(pin, INPUT, 1); // 1 indicates pull-up (assume `1` is defined for pull-up)

    // Enable the interrupt on the specified pin and edge
    //igitalInterrupt1Enable(pin, edge, event);
	
	  int motor1_speed = 50; // Initial speed guess for motor 1
    int motor2_speed = 50; // Initial speed guess for motor 2
    update_motor_speed(motor1_speed, motor2_speed);

    // Infinite loop to keep the program running and waiting for interrupts
    while (1) {
        speed2=encoder_speed(pin1);
	      speed1=encoder_speed(pin2);	
			  int error=speed1 -speed2;
			  int delta=Kp*error;
			  update_motor_speed(motor1_speed, motor2_speed+delta);
         // Calculate speed errors
			/*
        int error1 = TARGET_SPEED - speed1;
        int error2 = TARGET_SPEED - speed2;

        // Proportional control calculation
			
        motor1_speed_delta = (int)(Kp * error1);
        motor2_speed_delta = (int)(Kp * error2);

        // Update motor speeds
        update_motor_speed(motor1_speed+motor1_speed_delta, motor2_speed+motor2_speed_delta);
			*/
			
    }

    return 0; // This line is unreachable but included for completeness
}
