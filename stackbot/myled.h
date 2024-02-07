#ifndef MYLED_H
#define MYLED_H
#include <stdint.h>
#include "microbit.h"

static inline void mygpio_in(uint32_t pin, uint32_t pull);
#define   PULL_NONE     0
#define   PULLDOWN      (1 << 2)
#define   PULLUP        (3 << 2)

static inline void mygpio_out(uint32_t pin, uint32_t drive);
#define DRIVE_STANDARD  0
#define DRIVE_HIGH      (3 << 8)

static inline void mygpio_write(uint32_t pin, uint32_t val);
static inline void mygpio_set(uint32_t pin);
static inline void mygpio_clear(uint32_t pin);
static inline uint32_t mygpio_read(uint32_t pin);

#ifndef IOREG32
#define IOREG32(addr)   (*((volatile long *) (addr)))
#endif

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
#define GPIO_PINCFG (0x700)

#define PORT(pin)   (((pin) < 32) ? (GPIO0) : (GPIO1))
#define PIN(pin)    (((pin) < 32) ? (pin) : (pin - 32))

/* Set the specific pin as a digital input pin. */
static inline void mygpio_in(uint32_t pin, uint32_t pull)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);

    IOREG32(baseaddr + GPIO_PINCFG + pin*4) = pull;
    /* all other bits (input direction, pins connected, standard drive,
       no sense) in the register have zero bit-values. */
}

/* Set the specific pin as a digital output pin. */
static inline void mygpio_out(uint32_t pin, uint32_t drive)
{
    uint32_t baseaddr;
    uint32_t dir = 1;   // output

    baseaddr = PORT(pin);
    pin = PIN(pin);

    IOREG32(baseaddr + GPIO_DIR) |= (1 << pin);

    IOREG32(baseaddr + GPIO_PINCFG + pin*4) = (dir | drive);
    /* all other bits (no pull, pins connected, no sense) in the register
       have zero bit-values. */
}

/* Write the specified value to the specified pin. */
static inline void mygpio_write(uint32_t pin, uint32_t val)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);

    if (val == 1)
        IOREG32(baseaddr + GPIO_OUTSET) = (1 << pin);
    else
        IOREG32(baseaddr + GPIO_OUTCLR) = (1 << pin);
}

/* Set the specified pin (digital high). */
static inline void mygpio_set(uint32_t pin)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);

    IOREG32(baseaddr + GPIO_OUTSET) = (1 << pin);
}

/* Clear the specified pin (digital low). */
static inline void mygpio_clear(uint32_t pin)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);

    IOREG32(baseaddr + GPIO_OUTCLR) = (1 << pin);
}

/* Read the specified pin: 1 for high, 0 for low. */
static inline uint32_t mygpio_read(uint32_t pin)
{
    uint32_t baseaddr;

    baseaddr = PORT(pin);
    pin = PIN(pin);

    return ((IOREG32(baseaddr + GPIO_IN) >> pin) & 1UL);
}

static const uint8_t led_rows[] =
    { LED_ROW0, LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4 };
static const uint8_t led_cols[] =
    { LED_COL0, LED_COL1, LED_COL2, LED_COL3, LED_COL4 };

static inline void myled_on(int r, int c)
{
    extern int printf(char *fmt, ...);

    mygpio_out(led_rows[r], 0);
    mygpio_out(led_cols[c], 0);
    mygpio_set(led_rows[r]);
    mygpio_clear(led_cols[c]);
}

static inline void mydelay(volatile int n)
{
    n *= 1000;
    while (n > 0)
        n--;
}

static inline void myled_blink(int r, int c, int count)
{
    mygpio_clear(led_cols[c]);
    while ((count == -1) || (count > 0))
    {
        mygpio_set(led_rows[r]);
        mydelay(100);
        mygpio_clear(led_rows[r]);
        mydelay(100);
        if (count != -1)
            count--;
    }
    mygpio_set(led_cols[c]);
}

#endif  /* MYLED_H */
