#include <ADCTouchSensor.h>

#ifdef ADCTOUCH_INTERNAL_GROUNDING
# define GROUNDED_PIN -1
#endif

#if defined(ARDUINO_ARCH_AVR)
# define PIN1 A0
# define PIN2 A1
#else
# define PIN1 PA0
# define PIN2 PA1
# ifndef ADCTOUCH_INTERNAL_GROUNDING
#  define GROUNDED_PIN PA3
# endif
#endif

ADCTouchSensor button0 = ADCTouchSensor(PIN1, GROUNDED_PIN); 
ADCTouchSensor button1 = ADCTouchSensor(PIN2, GROUNDED_PIN);

void setup() 
{
    Serial.begin(9600);
    
    button0.begin();
    button1.begin();        
}

void loop() 
{
    int value0 = button0.read();   
    int value1 = button1.read();   

    Serial.print(value0 > 40);    //send (boolean) pressed or not pressed
    Serial.print("\t");           //use if(value > threshold) to get the state of a button

    Serial.print(value1 > 40);
    Serial.print("\t\t");

    Serial.print(value0);             //send actual reading
    Serial.print("\t");
	
    Serial.println(value1);
    delay(100);
}
