#include <ADCTouchSensor.h>
#define USB_MIDI
#include <USBMIDI.h> // https://github.com/arpruss/USBHID_stm32f1

// 
// On the stm32f1, this requires this branch of the stm32f1 core: https://github.com/arpruss/Arduino_STM32/tree/addMidiHID
//

#ifdef ADCTOUCH_INTERNAL_GROUNDING
# define GROUNDED_PIN -1
#endif

#ifdef ARDUINO_ARCH_AVR
# define SLOW_SAMPLING
# undef FAST_SAMPLING
# undef MEDIUM_SAMPLING
// slow mode: 9 ms per scan of 8 buttons
// medium mode: 5 ms per scan of 8 buttons
// fast mode: 2 ms per scan of 8 buttons

#define LED_OFF        0

#if defined(PIN_A7)
int pins[] = {A0,A1,A2,A3,A4,A5,A6,A7};
#else
int pins[] = {A0,A1,A2,A3,A4,A5};
#endif

#else // STM32F1

#define LED_BUILTIN PB12 // adjust to your board
#define LED_OFF        1
int pins[] = {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7};

#ifndef ADCTOUCH_INTERNAL_GROUNDING
# define ADCTOUCH_INTERNAL_GROUNDING PA8
#endif

#endif

//                       C   D   E   F   G   A   B   C   C#  D#  F#  G#  A#
const uint8_t notes[] = {60, 62, 64, 65, 67, 69, 71, 72, 61, 63, 66, 68, 70};
const int numPins = sizeof(pins)/sizeof(*pins);
const uint8_t NOTE_ON = 0b10010000;
const uint8_t NOTE_OFF = 0b10000000;
int ref[numPins];
uint8_t prev[numPins];
ADCTouchSensor* sensors[numPins];

void setup() 
{
#if defined(ARDUINO_ARCH_AVR)
// default: divide clock by 128
#ifdef FAST_SAMPLING
   ADCSRA = (ADCSRA & ~0b111) | 0b100; // divide clock by 16
#else
#ifdef MEDIUM_SAMPLING
   ADCSRA = (ADCSRA & ~0b111) | 0b110; // divide clock by 64
#endif
#endif
#endif

#ifndef USB_MIDI
    Serial.begin(115200);
#else
	USBMIDI.begin();
#endif	

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1^LED_OFF);     

    for (int i=0; i<numPins; i++) {
        sensors[i] = new ADCTouchSensor(pins[i]);
        sensors[i]->begin();
        prev[i] = 0;
    }

    digitalWrite(LED_BUILTIN, 0^LED_OFF);

//    uint32_t t = micros();
//    ADCTouch.read(A0, 10000);
//    t = micros() -t;
//    Serial.println(t);
} 

void midiNote(uint8_t status, uint8_t note, uint8_t velocity) {
#ifdef USB_MIDI
  if (status == NOTE_ON)
      USBMIDI.sendNoteOn(0, note, velocity);
  else if (status == NOTE_OFF)
      USBMIDI.sendNoteOff(0, note, velocity);    
#else
  Serial.write(status);
  Serial.write(note);
  Serial.write(velocity);
#endif
}

void loop() 
{
    uint8_t pressed = 0;

    for (int i=0; i<numPins; i++) {
      if (sensors[i]->read() > 25) {
         pressed = 1;
         if(!prev[i]) {
           midiNote(NOTE_ON, notes[i], 127);
           prev[i] = 1;
         }
      }
      else {
         if(prev[i]) {
           midiNote(NOTE_OFF, notes[i], 127);
           prev[i] = 0;
         }
      }
    }
    digitalWrite(LED_BUILTIN, pressed^LED_OFF);
}

