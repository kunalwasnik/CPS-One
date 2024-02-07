

static inline void buttonInit(int pin){
	pinMode(pin , INPUT);
}
	
static inline int button_pressed(int pin){

	return (digitalRead(pin)== LOW);
}

	
