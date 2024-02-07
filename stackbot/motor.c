#include <stdint.h>
#include "nrf52833.h"
#include "motor.h"
#include "printf.h"

static int period;

#define PWM         NRF_PWM3
#define PWM_CLK     1000000
#define PWM_FREQ    500

#define  max_rpm_1  206
#define  max_rpm_2  206

#define one_rotation_pulse 140
extern int timer_ticks;
extern int encoder1,encoder2;
extern int duty_cycle1 ,duty_cycle2;
int u=0;
volatile float delta_time;
//pid 1

volatile int current_encoder1 = 0, prev_encoder1=0 , current_time1, num_pulses1=0;
volatile int prev_time1=0;
volatile float  current_rpm1=0;
volatile float current_error1=0 , prev_error1=0 , edt1=0 , I1=0;
// I1 is integration term , ed1 : differentiation of error wrt to time;

//pid 2


volatile int current_encoder2 = 0, prev_encoder2=0 , current_time2, num_pulses2;
volatile float prev_time2=0 , current_rpm2=0;
volatile float current_error2 , prev_error2 , edt2 , I2=0;
// I1 is integration term , ed1 : differentiation of error wrt to time;

//

volatile int pidtime = 100;

void motor_init(int pin1A, int pin2A, int pin1B, int pin2B)
{
    /* Set prescaler divisor = 16 for 1 MHz clock */
    PWM->PRESCALER = 4;     // 16 = 1 << 4

    /* Set period */
    period = PWM_CLK / PWM_FREQ;
    PWM->COUNTERTOP = period;

    /* Connect output pins */
    PWM->PSEL.OUT[0] = pin1A;
    PWM->PSEL.OUT[1] = pin2A;
    PWM->PSEL.OUT[2] = pin1B;
    PWM->PSEL.OUT[3] = pin2B;

    /* Individual duty cycles for each channel */
    PWM->DECODER = 2;

    PWM->ENABLE = 1;
}

void motor_on(int dirA, int dutyA, int dirB, int dutyB)
{
    static uint16_t s_sequence[4];

    switch (dirA)
    {
        case MOTOR_FORWARD:
            s_sequence[0] = (period * dutyA) / 100;
            s_sequence[1] = 0;
            break;

        case MOTOR_REVERSE:
            s_sequence[0] = 0;
            s_sequence[1] = (period * dutyA) / 100;
            break;

        default:
            break;
    }

    switch (dirB)
    {
        case MOTOR_FORWARD:
            s_sequence[2] = (period * dutyB) / 100;
            s_sequence[3] = 0;
            break;

        case MOTOR_REVERSE:
            s_sequence[2] = 0;
            s_sequence[3] = (period * dutyB) / 100;
            break;

        default:
            break;
    }

    PWM->SEQ[0].PTR = (uint32_t) s_sequence;
    PWM->SEQ[0].CNT = 4;            // one value per channel
    PWM->SEQ[0].REFRESH = 0;        // continuous

    PWM->TASKS_SEQSTART[0] = 1;
    while (PWM->EVENTS_SEQSTARTED[0] == 0)
        ;

    return;
}

void motor_off(void)
{
    PWM->TASKS_STOP = 1;

    while (PWM->EVENTS_STOPPED == 0)
        ;

    PWM->EVENTS_STOPPED = 0;

    return;
}


int motor_1_duty(int set_rpm){
		
		int dutycycle;
		dutycycle= (set_rpm*100)/max_rpm_1;
		return dutycycle;
}	


int motor_2_duty(int set_rpm){
		
		int dutycycle;
		dutycycle= (set_rpm*100)/max_rpm_2;
		return dutycycle;
}	



int temp=0;

void pid1(int setrpm){
float Kp = 0.3 , Ki= 0.05, Kd=0.02;
	
current_encoder1 = encoder1;
current_time1 = timer_ticks;

num_pulses1 = current_encoder1 - prev_encoder1;

//delta_time = (current_time1 - prev_time1)/1000;
//  in seconds
delta_time = pidtime/1000.0;
		
// current rotation = no of pulses /( pulses in 1 rotation*time) , time in ms
current_rpm1 = (num_pulses1 * 60 )/(delta_time*one_rotation_pulse);


// pid	

current_error1 = setrpm - current_rpm1;

I1 = I1 + current_error1*delta_time;
edt1 = (current_error1 - prev_error1)/delta_time;

u = Kp*current_error1 + Ki*I1 + Kd*edt1;

duty_cycle1 = u;	

if(duty_cycle1<=0){
	duty_cycle1= 25;
}
if(duty_cycle1>=100){
	duty_cycle1=100;
}
	
//printf("%d %d\n" ,setrpm, current_rpm1);	

// for pid tuning
//printf("%f %f %f %f %d\n" , current_rpm1,delta_time , edt1 , I1 , u);	
	
prev_encoder1 = current_encoder1;	
}

// pid2

void pid2(int setrpm){
float Kp = 0.3 , Ki= 0.05, Kd=0.02;
	
	
current_encoder2 = encoder2;
current_time2 = timer_ticks;

num_pulses2 = current_encoder2 - prev_encoder2;

delta_time = pidtime/1000.0;
	
// current rotation = no of pulses /( pulses in 1 rotation*time) , time in ms
current_rpm2 = (num_pulses2 * 60 )/(delta_time*one_rotation_pulse);

	
// pid	

current_error2 = setrpm - current_rpm2;
if(current_error2==0) return;
	
I2 = I2 + current_error2*delta_time;
edt2 = (current_error2 - prev_error2)/delta_time;

u = Kp*current_error2 + Ki*I2 + Kd*edt2;
	
duty_cycle2 = u;

if(duty_cycle2<=0){
	duty_cycle2=25;
}
if(duty_cycle2>100){
	duty_cycle2=100;
}
	

prev_encoder2 = current_encoder2;	

//printf("%f %f %f %f %d\n" , current_rpm2,delta_time , edt2 , I2 , u);	
// for pid tuning
printf("%f %f\n" , current_rpm1,current_rpm2);	
}



