#ifndef GPIO_H
#define GPIO_H

// Direction definitions
#define INPUT 0
#define OUTPUT 1

// Pull-up/down resistor configurations
#define PULL_NONE     0
#define PULLDOWN      (1 << 2)
#define PULLUP        (3 << 2)

// Edge-trigger configurations for interrupts
#define GPIO_RISINGEDGE     1
#define GPIO_FALLINGEDGE    2
#define GPIO_BOTHEDGES      3

// Function prototypes
void pinMode(int pin, int direction, int pull);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
void digitalInterrupt1Enable(int pin, int edge, int event);

#endif  /* GPIO_H */
