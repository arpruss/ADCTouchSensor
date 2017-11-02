#include <ADCTouchSensor.h>

// 
// This requires an stm32f1 board compatible with the no-grounding-pin feature of ADCTouchSensor,
// and this branch of the stm32f1 core: https://github.com/arpruss/Arduino_STM32/tree/addMidiHID
//

#define LED_BUILTIN PB12 // change to match your board
 
#define NUM_PINS  10
unsigned pins[NUM_PINS] = {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9};
unsigned keys[NUM_PINS] = {' ',KEY_UP_ARROW,KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,'w','a','s','d','f'}; // Makey-Makey also has 'g' and CLICK, but we don't have enough ADC channels
unsigned prev[NUM_PINS];

ADCTouchSensor* sensors[NUM_PINS];

void setup() 
{
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
      if (sensors[i]->read() > 25) {
         pressed = 1;
         if(!prev[i]) {
           Keyboard.press(keys[i]);
           prev[i] = 1;
         }
      }
      else {
         if(prev[i]) {
           Keyboard.release(keys[i]);
           prev[i] = 0;
         }
      }
    }
    
    digitalWrite(LED_BUILTIN, !pressed);
}

