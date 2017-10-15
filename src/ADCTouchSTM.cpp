/*
  ADCTouchSTM.cpp - Library for Capacittive touch sensors using only one ADC PIN
  Created by Alexander Pruss. MIT license.
*/

#include "Arduino.h"
#include "ADCTouchSTM.h"

ADCTouch::ADCTouch(int pin, int sacrificialPin, int delayTimeMicroseconds) {
    touchPin = pin;
    reference = 0;
    groundedPin = sacrificialPin;
    delayTime = delayTimeMicroseconds;
    if (sacrificialPin < 0) {
#if defined(ARDUINO_GENERIC_STM32F103C) 
        groundedPinADCDevice = ADC1;
        groundedPinChannel = 15;
        valid = true;
#else  
        valid = false;
#endif    
    }
    else {
        valid = true;
    }
}

int ADCTouch::readRaw(int samples) {
    if (!valid)
        return -10000;
    int32_t total = 0;
    for (int i=0; i<samples; i++) {
        pinMode(touchPin, INPUT_PULLUP);
        if (groundedPin >= 0) {
            pinMode(groundedPin, OUTPUT);
            digitalWrite(groundedPin, 0);
            delayMicroseconds(delayTime);
            analogRead(groundedPin);
        }
        else {
            delayMicroseconds(delayTime);
            adc_read(groundedPinADCDevice, groundedPinChannel);
        }
        pinMode(touchPin, INPUT_ANALOG);
        total += analogRead(touchPin);
    }
    return total / (4 * samples);
}

bool ADCTouch::begin(int samples) {
    if (valid) {
        reference = readRaw(samples);
        return true;
    }
    else
        return false;
}

int ADCTouch::read(int samples) {
    if (valid)
        return readRaw(samples) - reference;
    else
        return -10000;
}
