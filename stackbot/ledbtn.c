#include <stdint.h>
#include "ledbtn.h"
#include "gpio.h"

/* LEDs, buttons, speaker and mic routines */

static const uint8_t led_rows[] =
    { LED_ROW0, LED_ROW1, LED_ROW2, LED_ROW3, LED_ROW4 };
static const uint8_t led_cols[] =
    { LED_COL0, LED_COL1, LED_COL2, LED_COL3, LED_COL4 };
static const uint8_t btn_list[] = { BUTTON_0, BUTTON_1 };

uint8_t frame_buffer[LED_NUM_ROWS][LED_NUM_COLS];

/* Initialize LED matrix.
 *
 * Start with a blank display.
 * Rows are held low and columns are held high to reverse bias all the LEDs.
 */
void leds_init(void)
{
    uint32_t r, c;

    /* Rows */
    for (r = 0; r < LED_NUM_ROWS; r++)
    {
        gpio_clear(led_rows[r]);                // row lines low
        gpio_out(led_rows[r], DRIVE_STANDARD);  // standard drive current
    }

    /* Columns */
    for (c = 0; c < LED_NUM_COLS; c++)
    {
        gpio_set(led_cols[c]);                  // column lines high
        gpio_out(led_cols[c], DRIVE_STANDARD);  // standard drive current
    }
}

/* Display the picture on the LED matrix. */
void led_display(uint8_t pic[LED_NUM_ROWS][LED_NUM_COLS])
{
    int r, c;

    /* Refresh rows one-by-one. */
    for (r = 0; r < LED_NUM_ROWS; r++)
        for (c = 0; c < LED_NUM_COLS; c++)
            frame_buffer[r][c] = pic[r][c];
}

/* Configure buttons pins */
void buttons_init(void)
{
    uint32_t i;

    for (i = 0; i < BUTTONS_NUMBER; ++i)
        gpio_in(btn_list[i], PULLUP);
}

/* Get button state */
uint32_t button_get(uint32_t button_idx)
{
    uint32_t pin_set = gpio_read(btn_list[button_idx]) ? 1 : 0;

    return (pin_set == (BUTTONS_ACTIVE_STATE ? 1 : 0));
}

/* Single nop instruction for 1 clock cycle delay */
static inline void nop() { __asm__("nop"); }

/* Delay in microseconds */
void delay_us(uint32_t usec)
{
    while (usec > 0)
    {
        /*
         * The processor runs at 64 MHz, which is 64 cycles per usec.
         * Overheads associated with a loop, say 4 cycles.
         * Hence, 60 nops below.
         */
        // 1   2      3      4      5      6      7      8
        nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); // 1
        nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); // 2
        nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); // 3
        nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); // 4
        nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); // 5
        nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); // 6
        nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); // 7
        nop(); nop(); nop(); nop();

        usec--;
    }
}

/* Delay in milliseconds */
void delay_ms(uint32_t ms)
{
    while (ms > 0)
    {
        delay_us(1000);
        ms--;
    }
}
/* Functions that help me debugging */

void clear_display()
{
    uint32_t r, c;

    for (r = 0; r < LED_NUM_ROWS; r++)
        gpio_clear(led_rows[r]);    // row leds low

    for (c = 0; c < LED_NUM_COLS; c++)
        gpio_set(led_cols[c]);      // column leds high
}

/* Blink forever, to stop the code flow or handle a fault. */
void led_blink(int r, int c, int count)
{
    gpio_clear(led_cols[c]);
    while ((count == BLINK_FOREVER) || (count > 0))
    {
        gpio_set(led_rows[r]);
        delay_ms(100);
        gpio_clear(led_rows[r]);
        delay_ms(100);
        if (count != BLINK_FOREVER)
            count--;
    }
    gpio_set(led_cols[c]);
}

/* This routine is called at a refresh rate of 5ms.
 * Each time this routine is called, it turns off pixels of the previous row
 * and displays the pixel in the next row.
 *
 * That way, we refresh 5 rows in 25ms, getting a persistent display at 40fps.
 */
void led_row_refresh(void)
{
    static uint32_t row = 0;
    int c;

    /* Deactivate the previous row. */
    gpio_clear(led_rows[row]);

    /* Compute the next row */
    row = (row + 1) % LED_NUM_ROWS;

    /* Set the pixels the next row - they will remain on until the
     * next callback.
     * (An LED turns on when the corresponding row signal is high and
     * column signal is low. We set their column values first and then
     * activate the row signal.
     */
    for (c = 0; c < LED_NUM_COLS; c++)
    {
        if (frame_buffer[row][c])   // is the pixel set?
            gpio_clear(led_cols[c]);    // column low turns on the LED
        else
            gpio_set(led_cols[c]);      // column high turns off the LED
    }

    /* Activate the current row. */
    gpio_set(led_rows[row]);
}
