#include <ADCTouchSTM.h>

#if defined(ARDUINO_GENERIC_STM32F103C)
# define SACRIFICIAL_PIN -1
#else
# define SACRIFICIAL_PIN PA3
#endif

ADCTouch button0 = ADCTouch(PA0, SACRIFICIAL_PIN); 
ADCTouch button1 = ADCTouch(PA1, SACRIFICIAL_PIN);

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
