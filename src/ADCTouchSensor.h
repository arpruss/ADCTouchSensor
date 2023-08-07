/*
  ADCTouchSensor.h - Library for Capacitive touch sensors using only one ADC PIN, with a second unconnected pin shared between sensors on some devices
  Based on code that was created by martin2250, April 23, 2014 and released into the public domain.
*/
#ifndef ADCTOUCHSENSOR_h
#define ADCTOUCHSENSOR_h

#include "Arduino.h"

#if defined(ARDUINO_GENERIC_STM32F103C) 
# define ADCTOUCH_STM32_GROUND_CHANNEL  15 
# define ADCTOUCH_STM32_ADC             ADC1
# define ADCTOUCH_INTERNAL_GROUNDING
#endif

#if defined(ARDUINO_ARCH_AVR)
# define ADCTOUCH_INTERNAL_GROUNDING
# define INPUT_ANALOG INPUT
#endif

#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F2) || defined(ARDUINO_ARCH_STM32F3) || defined(ARDUINO_ARCH_STM32F4)
# define ADCTOUCH_DEFAULT_DELAY 20
# define ADCTOUCH_DIVIDER        4
#elif defined(ARDUINO_ARCH_AVR)
# define ADCTOUCH_DEFAULT_DELAY 0
# define ADCTOUCH_DIVIDER       1
#elif defined(ARDUINO_ARCH_STM32)
# define ADCTOUCH_DEFAULT_DELAY 0
# define ADCTOUCH_DIVIDER       4
#endif

class ADCTouchSensor
{
    private:
        int32_t reference;
        int groundedPin;
        int touchPin;
        int touchDigitalPin;
        unsigned delayTime;
        bool valid;

    private:
        void groundPortable();
#ifdef ADCTOUCH_INTERNAL_GROUNDING        
        void ground();
#endif
    
	public:
        ADCTouchSensor(int pin, int sacrificialPin=-1, unsigned delayTimeMicroseconds=ADCTOUCH_DEFAULT_DELAY);
        bool begin(unsigned samples=500);
        int read(unsigned samples = 5);
        int readRaw(unsigned samples = 5);
};
#endif
