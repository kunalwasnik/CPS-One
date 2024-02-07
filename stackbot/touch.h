
#define TOUCH			36

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define IOREG32(addr)		(*((volatile unsigned long *) (addr)))
#define INPUT_PULLUP 0x2



#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

void pinMode(int pin, int mode);

void digitalWrite(int pin, int val);
	
int digitalRead(int pin);

void touchInit(int pin);

int touchGet(int pin);