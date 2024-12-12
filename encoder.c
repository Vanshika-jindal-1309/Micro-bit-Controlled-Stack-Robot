#include <stdint.h>
#include "nrf52833.h"
#include "encoder.h"
#include "timer32.h"
#include "gpio.h"

#define t (60000000L)     // microseconds in a minute
#define PPR 144             // pulse per revolution

unsigned long timer_m1, timer_m1_prev, timer_m1_diff;
unsigned long timer_m2, timer_m2_prev, timer_m2_diff;

int enc1_pin = 0, enc2_pin = 0;

void encoder_init(int pin1, int pin2)
{
    enc1_pin = pin1;
    pinMode(enc1_pin, INPUT, PULLDOWN);
    digitalInterrupt1Enable(enc1_pin, GPIO_RISINGEDGE, 0);
    
    enc2_pin = pin2;
    pinMode(enc2_pin, INPUT, PULLDOWN);
    digitalInterrupt1Enable(enc2_pin, GPIO_RISINGEDGE, 1);
    
    timer32_init();

    timer_m1 = 0; timer_m1_prev = 0; timer_m1_diff = 0;
    timer_m2 = 0; timer_m2_prev = 0; timer_m2_diff = 0;
}

float encoder_speed(int motor_enc_pin)
{
    if(motor_enc_pin == enc1_pin && timer_m1_diff != 0)
        return ((float)t / timer_m1_diff) / PPR;
    else if (motor_enc_pin == enc2_pin && timer_m2_diff != 0)
        return ((float)t / timer_m2_diff) / PPR;
    else
        return 0;
}

/* rev/min = (micro / min) / ((micro / pulse) * (pulse / rev)) */

void encoder_update(int event)
{
    if(event == 0)
    {
        timer_m1 = timer32_read();
        timer_m1_diff = timer_m1 - timer_m1_prev;
        timer_m1_prev = timer_m1;
    }   
    else if(event == 1)
    {
        timer_m2 = timer32_read();
        timer_m2_diff = timer_m2 - timer_m2_prev;
        timer_m2_prev = timer_m2;
    }
}
