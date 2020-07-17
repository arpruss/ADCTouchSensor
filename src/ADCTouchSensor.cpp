/*
  ADCTouchSTM.cpp - Library for Capacittive touch sensors using only one ADC PIN
  Created by Alexander Pruss. MIT license.
*/

#include "Arduino.h"
#include "ADCTouchSensor.h"

ADCTouchSensor::ADCTouchSensor(int pin, int sacrificialPin, unsigned delayTimeMicroseconds) {
    touchPin = pin;
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	touchDigitalPin = pgm_read_byte(analog_pin_to_digital_pin + pin);
	if ((uint8_t)touchDigitalPin == (uint8_t)NOT_A_PIN) {
        valid=false;
        return;
    }
#else
    touchDigitalPin = pin;
#endif    
    reference = 0;
    groundedPin = sacrificialPin;
    delayTime = delayTimeMicroseconds;
    if (sacrificialPin < 0) {
#if defined(ADCTOUCH_STM32_GROUND_CHANNEL) || defined(ARDUINO_ARCH_AVR)
        valid = true;
#else
        valid = false;
#endif    
    }
    else {
        valid = true;
    }
}

// connect the ADC input and the internal sample and hold capacitor to ground to discharge it
// also, delay to allow for charging of the touch pin
inline void ADCTouchSensor::groundPortable() {
    pinMode(groundedPin, OUTPUT); // HARDWARE ISSUE: ensure no short occurs
    digitalWrite(groundedPin, 0);
    if (delayTime > 0)
        delayMicroseconds(delayTime);
    analogRead(groundedPin);
}

#if defined(ADCTOUCH_STM32_GROUND_CHANNEL)
inline void ADCTouchSensor::ground() {
    if (delayTime>0) 
        delayMicroseconds(delayTime);
    adc_read(ADCTOUCH_STM32_ADC, ADCTOUCH_STM32_GROUND_CHANNEL);
}

#elif defined(ARDUINO_ARCH_AVR)

inline void ADCTouchSensor::ground() {
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = (ADMUX & 0xF0) | 0b1101;
#else
    ADMUX |=   0b11111;
#endif
    if (delayTime>0)
        delayMicroseconds(delayTime);

    ADCSRA |= (1 << ADSC);
    // ADSC is cleared when the conversion finishes
    while((ADCSRA & (1 << ADSC))) ;
}
#endif

int ADCTouchSensor::readRaw(unsigned samples) {
    if (!valid)
        return -10000;
    int32_t total = 0;
    for (unsigned i=0; i<samples; i++) {
        pinMode(touchDigitalPin, INPUT_PULLUP);
        if (groundedPin >= 0) {
            groundPortable();
        }
#if defined(ADCTOUCH_STM32_GROUND_CHANNEL)
        else {
            ground();
        }
#endif        
        pinMode(touchDigitalPin, INPUT_ANALOG);
        total += analogRead(touchPin);
    }
    return total / ADCTOUCH_DIVIDER / samples;
}

bool ADCTouchSensor::begin(unsigned samples) {
    if (valid) {
        reference = readRaw(samples);
        return true;
    }
    else
        return false;
}

int ADCTouchSensor::read(unsigned samples) {
    if (valid)
        return readRaw(samples) - reference;
    else
        return -10000;
}
