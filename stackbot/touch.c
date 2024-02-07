#include "touch.h"
#include "stdint.h"

/* Base addresses of GPIO port 0 and GPIO port 1 */
#define GPIO0       0x50000000UL
#define GPIO1       0x50000300UL

/* Register offsets to be added to GPIO0/1 base address */
#define GPIO_OUT    0x504   // write all 32-bits to a port
#define GPIO_OUTSET 0x508   // set pins as per the mask
                            //   1 sets the pin, 0 no effect
#define GPIO_OUTCLR 0x50c   // clear pins as per the mask
                            //   1 clears the pin, 0 no effect
#define GPIO_IN     0x510   // read a port
#define GPIO_DIR    0x514   // set directions of all 32 pins of a port
                            //   0 input, 1 output

/* Configuration pin registers. There is one 32-bit register for each pin.
 * Provides many options, but we use it only for setting pull-up or pull-down
 * resister.
 */
#define GPIO_PINCFG 0x700
#define   PULL_NONE     (0)
#define   PULLDOWN      (1 << 2)
#define   PULLUP        (3 << 2)
#define   DRIVE_NORMAL  (0)
#define   DRIVE_HIGH    (3 << 8)

#define PORT(pin)   (((pin) < 32) ? (GPIO0) : (GPIO1))
#define PIN(pin)    (((pin) < 32) ? (pin) : (pin - 32))

/* Set the specific pin as a digital input pin. */
void pinMode(int pin, int mode)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);
  
    switch(mode)
    {
      case INPUT:
        IOREG32(baseaddr + GPIO_PINCFG + pin*4) = PULL_NONE;
        bitClear(IOREG32(baseaddr + GPIO_DIR), pin);
        break;

      case INPUT_PULLUP:
        IOREG32(baseaddr + GPIO_PINCFG + pin*4) = PULLUP;
        bitClear(IOREG32(baseaddr + GPIO_DIR), pin);
        break;
      
      case OUTPUT:
        IOREG32(baseaddr + GPIO_PINCFG + pin*4) = DRIVE_NORMAL;
        bitSet(IOREG32(baseaddr + GPIO_DIR), pin);
        break;

      default:
        break;    // leave the pin as it is
    }
    
    return;
}

void digitalWrite(int pin, int val)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);

    if (val == LOW)
        IOREG32(baseaddr + GPIO_OUTCLR) = (1UL << pin);
    else
        IOREG32(baseaddr + GPIO_OUTSET) = (1UL << pin);
}

/* Read the specified pin: 1 for high, 0 for low. */
int digitalRead(int pin)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);

    return (bitRead(IOREG32(baseaddr + GPIO_IN), pin));
}

void touchInit(int pin)
	//Initialize the specified pin as a button pin.
{
	pinMode(pin, INPUT);
}

int touchGet(int pin)
	//Get the status of the button connected to the pin.
{
	return (digitalRead(pin) == LOW);
}

