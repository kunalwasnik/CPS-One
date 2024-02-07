#ifndef LEDBTN_H
#define LEDBTN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "microbit.h"

/* LED */
extern uint8_t frame_buffer[LED_NUM_ROWS][LED_NUM_COLS];
void leds_init(void);
void led_display(uint8_t pic[LED_NUM_ROWS][LED_NUM_COLS]);
void led_row_refresh(void);

/* Turn on/off, glow or blink a specific LED for debugging. */
void led_on(int row, int col);
void led_off(int row, int col);
#define BLINK_FOREVER -1
void led_glow(int row, int col);
void led_blink(int row, int col, int count);

/* Buttons */
void buttons_init(void);
uint32_t button_get(uint32_t button_idx);

/* Delay */
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif // LEDBTN_H
