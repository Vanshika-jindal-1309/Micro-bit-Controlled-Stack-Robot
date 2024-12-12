#ifndef ENCODER_H
#define ENCODER_H

void encoder_init(int pin1, int pin2);
float encoder_speed(int pin);
void encoder_update(int event);

#endif /* ENCODER_H */