/*
  ADCTouch.h - Library for Capacittive touch sensors using only one ADC PIN
  Created by martin2250, April 23, 2014.
  Released into the public domain.
*/
#ifndef ADCTOUCHSTM_h
#define ADCTOUCHSTM_h

#include "Arduino.h"

class ADCTouch
{
    private:
        bool valid;
        const adc_dev* groundedPinADCDevice;
        int groundedPinChannel;
        int groundedPin;
        int touchPin;
        int reference;
        int delayTime;
    
	public:
        ADCTouch(int pin, int sacrificialPin=-1, int delayTimeMicroseconds=20);
        bool begin(int samples=500);
        int read(int samples = 5);
        int readRaw(int samples = 5);
};

#endif
