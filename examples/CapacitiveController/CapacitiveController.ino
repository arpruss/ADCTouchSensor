#include <ADCTouchSensor.h>                                  
                                                                                                                                                                                   
// 
// This requires an stm32f1 board compatible with the no-grounding-pin feature of ADCTouchSensor,
// and this branch of the stm32f1 core: https://github.com/arpruss/Arduino_STM32/tree/addMidiHID
//
// Either in Joystick, Keyboard+Mouse or Keyboard+Mouse+Joystick mode. In the last mode, ground PA10
// to set joystick mode.
// 

#define LED_BUILTIN PB12 // change to match your board
#define JOYSTICK_MODE PA10 // ground to set joystick mode

#define NUM_PINS  10
unsigned pins[NUM_PINS] = {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9};
#if defined(USB_HID_KM) || defined(USB_HID_KMJ)
# define SUPPORT_KEYS
unsigned keys[NUM_PINS] = {' ',KEY_UP_ARROW,KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,'w','a','s','d','f'}; // Makey-Makey also has 'g' and CLICK, but we don't have enough ADC channels
#endif
#if defined(USB_HID_KMJ) || defined(USB_HID_J)
# define SUPPORT_JOYSTICK
unsigned buttons[NUM_PINS] = { 1, 0, 0, 0, 0, 2, 3, 4, 5, 6 };
#endif
unsigned prev[NUM_PINS];

ADCTouchSensor* sensors[NUM_PINS];

#ifdef SUPPORT_JOYSTICK
int8_t dirX, dirY;
#endif

#ifdef SUPPORT_KEYS
void processPressKeyboard(int i) {
  Keyboard.press(keys[i]);
}

void processReleaseKeyboard(int i) {
  Keyboard.release(keys[i]);
}
#endif

#ifdef SUPPORT_JOYSTICK
void processPressJoystick(int i) {
  if (buttons[i]) 
    Joystick.button(buttons[i], 1);
}

void processReleaseJoystick(int i) {
  if (buttons[i]) 
    Joystick.button(buttons[i], 0);
}
#endif

#if defined(SUPPORT_KEYS) && defined(SUPPORT_JOYSTICK)
  void (*processPress)(int) = processPressKeyboard;
  void (*processRelease)(int) = processReleaseKeyboard;
  uint8_t joystickMode = 0;
#elif defined(SUPPORT_KEYS)
# define processPress processPressKeyboard
# define processRelease processReleaseKeyboard
# define joystickMode 0
#else 
# define processPress processPressJoystick
# define processRelease processReleaseJoystick
# define joystickMode 1
#endif


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

#ifdef SUPPORT_JOYSTICK
    Joystick.setManualReportMode(true);
#ifdef SUPPORT_KEYS
    pinMode(JOYSTICK_MODE, INPUT_PULLUP);
#endif    
#endif    
} 


void loop() 
{
    uint8_t pressed = 0;

#ifndef joystickMode
    if (!                 digitalRead(JOYSTICK_MODE) != joystickMode) {
      joystickMode = ! joystickMode;
      if (joystickMode) {
        processPress = processPressJoystick;
        processRelease = processReleaseJoystick;
      }
      else {
        processPress = processPressKeyboard;
        processRelease = processReleaseKeyboard;
      }
    }
#endif
#ifdef SUPPORT_JOYSTICK
    dirX = 0;
    dirY = 0;
#endif    
    
    for (int i=0; i<NUM_PINS; i++) {
      if (sensors[i]->read() > 25) {
         pressed = 1;
         if(!prev[i]) {
           processPress(i);
           prev[i] = 1;
         }
#ifdef SUPPORT_JOYSTICK         
         if (joystickMode && buttons[i] == 0) {
           if (i==1)
            dirY--;
           else if (i==2)
            dirX--;
           else if (i==3)
            dirY++;
           else 
            dirX++;
         }
#endif           
      }
      else {
         if(prev[i]) {
           processRelease(i);
           prev[i] = 0;
         }
      }
    }
    
#ifdef SUPPORT_JOYSTICK
    if (joystickMode) {
      if (dirX < 0) 
        Joystick.X(0);
      else if (dirX > 0) 
        Joystick.X(1023);
      else
        Joystick.X(512);
      if (dirY < 0)
        Joystick.Y(0);
      else if (dirY > 0) 
        Joystick.Y(1023);
      else
        Joystick.Y(512);
      Joystick.sendManualReport();
    }
#endif    

    digitalWrite(LED_BUILTIN, !pressed);
}

