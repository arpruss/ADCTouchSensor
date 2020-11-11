#include <ADCTouchSensor.h>                                  
                                                                                                                                                                                   
// 
// This requires an stm32f1 board compatible with the no-grounding-pin feature of ADCTouchSensor.
//

#define NUM_PINS  10
unsigned pins[NUM_PINS] = {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1};
unsigned prev[NUM_PINS];

ADCTouchSensor* sensors[NUM_PINS];


void setup() 
{
    Serial.begin();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);     

    for (int i=0; i<NUM_PINS; i++) {
        sensors[i] = new ADCTouchSensor(pins[i]);
        sensors[i]->begin();
        prev[i] = 0;
    }

    digitalWrite(LED_BUILTIN, 1);
} 


void loop() 
{
    uint8_t pressed = 0;

    for (int i=0; i<NUM_PINS; i++) {
      if (sensors[i]->read() > 35) {
         pressed = 1;
         if(!prev[i]) {
           prev[i] = 1;
           Serial.println(String("Press: ")+i);
         }
      }
      else {
         if(prev[i]) {
           Serial.println(String("Release: ")+i);
           prev[i] = 0;
         }
      }
    }
    
    digitalWrite(LED_BUILTIN, !pressed);
}
