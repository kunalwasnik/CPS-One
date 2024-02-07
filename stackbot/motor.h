void motor_init(int M1A, int M1B, int M2A, int M2B);
void motor_on(int dirA, int dutyA, int dirB, int dutyB);
void motor_off(void);
enum { MOTOR_FORWARD, MOTOR_REVERSE };


// duty cycle

int motor_1_duty(int set_rpm);
void pid1(int rpm);

int motor_2_duty(int set_rpm);
void pid2(int rpm);