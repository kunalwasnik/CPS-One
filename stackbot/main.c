#include "myled.h"
#include <stdint.h>

#include "bsp.h"

int direction=0;
int set_rpm=0;
volatile int encoder1 , encoder2;
volatile int duty_cycle1,duty_cycle2;
extern float current_rpm1 , current_rpm2;
extern int pidtime;

void radio_read_callback(const char *buf, unsigned int n)
{
	char welcome[]="a4a4.";
    int i;

   
   
    if (buf[0] == '1'){
        printf("Received 1\n");
			direction=1;
		
			
		}
    if (buf[0] == '2'){
        //led_display(picB);
				printf("Received 2\n");
			direction=2;
		;
			
		}

    return;
}



/* Test UART interrupt mode */
void uart_read_handler(char ch)
{
    uart_putc('[');
    uart_putc(ch);
    uart_putc(']');
}

void encoder1_callback(void){
    encoder1++;

}

void encoder2_callback(void){
    encoder2++;

}

 
// for callback
int tou = 2;
int touch_flag=1;
int result=0;
static const int T = TOUCH;

int main(void)
{
    uart_init(UART_RX, UART_TX);
    timer_init();
		touchInit(T);
		buttonInit(buttonA);
		buttonInit(buttonB);
		LSM303AGR_Init(I2C_SCL, I2C_SDA);

    radio_init(radio_read_callback);

	
	
  
	  motor_init(P0, P1, P2, P16);
		gpio_inten(P4, 0 ,GPIO_RISINGEDGE,encoder1_callback);
	  gpio_inten(P10, 1 ,GPIO_RISINGEDGE,encoder2_callback);
	
		set_rpm=150;
		
		duty_cycle1 = motor_1_duty(set_rpm);
		duty_cycle2 = motor_2_duty(set_rpm);
		
	
	
	
		while(1){
				if(touchGet(TOUCH)){
				break;
			}
				if(button_pressed(buttonB)){
				break;
				}
			
		}
			
		motor_on(MOTOR_FORWARD, duty_cycle2, MOTOR_REVERSE,duty_cycle1);
		
		//pid
		
		
		
		
		for(int i=0;i <200;i++){
		//	while(1){
			
		pid1(set_rpm);
		pid2(set_rpm);
	
		
		if(direction==1)
		motor_on(MOTOR_FORWARD, duty_cycle2, MOTOR_REVERSE,duty_cycle1 );
		
		if(direction==2)
			motor_on(MOTOR_REVERSE, duty_cycle2, MOTOR_FORWARD,duty_cycle1 );
			
		
		timer_delay(pidtime);
		
 }
		motor_off();
    return 0;
}