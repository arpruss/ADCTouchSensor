#include <ADCTouchSensor.h>                                  
#include <USBComposite.h> // https://github.com/arpruss/USBHID_stm32f1
                                                                                                                                                                                   
// 
// This requires an stm32f1 board compatible with the no-grounding-pin feature of ADCTouchSensor.
//

USBHID HID;
HIDJoystick Joystick(HID);
HIDKeyboard Keyboard(HID);

#define LED_BUILTIN PB12 // change to match your board
#define JOYSTICK_MODE PA10 // ground to set joystick mode

#define NUM_PINS  10
unsigned pins[NUM_PINS] = {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9};
unsigned keys[NUM_PINS] = {' ',KEY_UP_ARROW,KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,'w','a','s','d','f'}; // Makey-Makey also has 'g' and CLICK, but we don't have enough ADC channels
unsigned buttons[NUM_PINS] = { 1, 0, 0, 0, 0, 2, 3, 4, 5, 6 };
unsigned prev[NUM_PINS];

ADCTouchSensor* sensors[NUM_PINS];

int8_t dirX, dirY;

void processPressKeyboard(int i) {
  Keyboard.press(keys[i]);
}

void processReleaseKeyboard(int i) {
  Keyboard.release(keys[i]);
}

void processPressJoystick(int i) {
  if (buttons[i]) 
    Joystick.button(buttons[i], 1);
}

void processReleaseJoystick(int i) {
  if (buttons[i]) 
    Joystick.button(buttons[i], 0);
}

void (*processPress)(int) = processPressKeyboard;
void (*processRelease)(int) = processReleaseKeyboard;
uint8_t joystickMode = 0;


void setup() 
{
	  HID.begin(HID_KEYBOARD_JOYSTICK);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 0);     

    for (int i=0; i<NUM_PINS; i++) {
        sensors[i] = new ADCTouchSensor(pins[i]);
        sensors[i]->begin();
        prev[i] = 0;
    }

    digitalWrite(LED_BUILTIN, 1);

    Joystick.setManualReportMode(true);
    pinMode(JOYSTICK_MODE, INPUT_PULLUP);
} 


void loop() 
{
    uint8_t pressed = 0;

    if (! digitalRead(JOYSTICK_MODE) != joystickMode) {
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
    dirX = 0;
    dirY = 0;
    
    for (int i=0; i<NUM_PINS; i++) {
      if (sensors[i]->read() > 35) {
         pressed = 1;
         if(!prev[i]) {
           processPress(i);
           prev[i] = 1;
         }
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
      }
      else {
         if(prev[i]) {
           processRelease(i);
           prev[i] = 0;
         }
      }
    }
    
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
      Joystick.send();
    }

    digitalWrite(LED_BUILTIN, !pressed);
}

