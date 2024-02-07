#ifndef _MYPRINTF_H
#define _MYPRINTF_H

/* define putchar as the function that transmits a character */
#define putchar uart_putc

int printf(char *fmt, ...);
void putchar(char c);
int puts(const char s[]);

#endif /*_MYPRINTF_H */
