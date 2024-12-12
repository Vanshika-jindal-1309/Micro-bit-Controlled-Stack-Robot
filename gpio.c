#include "gpio.h"
#include "encoder.h"
#define IOREG(addr)   (*((volatile long *) (addr)))
#define IOREG32(addr) (*((volatile unsigned long *) (addr)))
/* Base addresses of GPIO port 0 and GPIO port 1 */
#define GPIO0       0x50000000UL  // pins 0 to 31
#define GPIO1       0x50000300UL  // pins 32 to 40

/* Register offsets to be added to GPIO0/1 base address */
#define GPIO_OUT    0x504   // write all 32-bits to a port
#define GPIO_IN     0x510   // read a port
#define GPIO_DIR    0x514   // set directions of all 32 pins of a port
                            //   0 input, 1 output

#define GPIO_PINCONFIG(pin) (0x700 + (pin) * 4)

#define GPIO_BIT(pin)     (((pin) < 32) ? (pin) : (pin) - 32)
#define GPIO_PORT(pin)    (((pin) < 32) ? (GPIO0) : (GPIO1))
volatile unsigned long enc1_count = 0;
volatile unsigned long enc2_count = 0;

void pinMode(int pin, int direction, int pull)
{
  unsigned long port, bit;

  port = GPIO_PORT(pin);
  bit = GPIO_BIT(pin);

  if (direction == OUTPUT)
  {
    IOREG(port + GPIO_DIR) |= (1UL << bit);
    IOREG(port + GPIO_PINCONFIG(bit)) = 1;    // output, standard drive
  }
  else
  {
    IOREG(port + GPIO_DIR) &= ~(1UL << bit);  // set direction
    IOREG(port + GPIO_PINCONFIG(bit)) = pull; // input, connect, pull as specified, sense disabled
  }

  return;
}

void digitalWrite(int pin, int value)
{
  unsigned long port, bit;

  port = GPIO_PORT(pin);
  bit = GPIO_BIT(pin);

  if (value)
    IOREG(port + GPIO_OUT) |= (1UL << bit);
  else
    IOREG(port + GPIO_OUT) &= ~(1UL << bit);

  return;
}

int digitalRead(int pin)
{
  int port, bit;

  port = GPIO_PORT(pin);
  bit = GPIO_BIT(pin);

  return (IOREG(port + GPIO_IN) >> bit) & 0x01;
}

/* GPIOTE registers */
#define GPIOTE_EVENTSIN(i)  IOREG32(0x40006100 + 4*(i))
#define GPIOTE_INTENSET     IOREG32(0x40006304)
#define GPIOTE_CONFIG(i)    IOREG32(0x40006510 + 4*(i))
#define GPIOTE_MODEEVENT    (1)

/* NVIC registers */
#define NVIC_ISER           IOREG32(0xE000E100)
#define GPIOTE_ID           6               // peripheral ID

void digitalInterrupt1Enable(int pin, int edge, int event)
{
     /* GPIOTE has 8 registers, each can be configured for event i (i = 0 to 7) 
      * along with the pin number and event type associated with the event.
      */
    GPIOTE_CONFIG(event)
        = ((GPIOTE_MODEEVENT) | (pin << 8) | (edge << 16));

    /* Generate an interrupt when the specified event occurs. */
    GPIOTE_INTENSET |= (1 << event);

    /* Enable GPIOTE interrupts in the interrupt controller */
    NVIC_ISER |= (1 << GPIOTE_ID);
}

void GPIOTE_IRQHandler(void)
{
	if( GPIOTE_EVENTSIN(0))
	{
		//count1++;
		enc1_count++; 
		//myprintf("count1 = %d\n",count1);
		encoder_update(0);
		GPIOTE_EVENTSIN(0) = 0;
	}

	if( GPIOTE_EVENTSIN(1))
	{
		//count2++;
		enc1_count++; // Increment encoder 1 count
		//myprintf("count2 = %d\n",count2);
		encoder_update(1);
		GPIOTE_EVENTSIN(1) = 0;
	}
}
