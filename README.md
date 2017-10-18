## ADCTouchSensor  
ADCTouchSensorSTM is a library that allows users to create a capacitive sensor without ANY external hardware.
It is based on the ideas and code in https://github.com/martin2250/ADCTouch but extended to work on hardware
other than AVR.

## Purpose  
Most capacitive touch libraries require two pins and a large resistor to acquire precise readings. 
This library makes use of internal wiring to get decent resolution with just a single pin (plus on 
some MCUs one extra sacrificial pin, shared between allows the touch sensors).

## Usage  
* `ADCTouchSensor touchSensor = ADCTouchSensor(int analogPin, int sacrificialPin = -1, unsigned delaytime = [device dependent]);`

* `touchSensor.begin(unsigned samples=500);`

* `value = touchSensor.read(unsigned samples=5);`

* `value = touchSensor.readRaw(unsigned samples=5);`

* `analogPin`: The pin to read data from

* `sacrificialPin` is grounded and used for internal purposes. It shouldn't
    be connected to anything (beware of shorts). A single 
    sacrificialPin can be used for many ADCTouchSensor instances. On the stm32f103c, you can omit 
    the sacrificial pin, and an internal grounded ADC channel
    can be used. If your STM32F1 MCU has such a channel -- likely, the same one, namely 15 -- you can
    edit the library to support it (and email me)
    
* `delayTime` is the amount of extra time to allow for charging up the touch pin; the default value is
    device dependent

* `samples`: number of samples to take

* `begin()`: reads reference data

* `read()`: reads a value and corrects it via the reference data; the value can in principle range
    from -1024 to 1023, but in practice tends to be in the single digits (plus or minus) if there
    is no touch and at least 100 if there is a touch. Returns -10000 if the setup is wrong (i.e.,
    no sacrificialPin and wrong MCU).

* `readRaw()`: reads but does not correct via reference data; value can range from 0 to 1023.
     Returns -10000 if the setup is wrong (e.g., no sacrificialPin and MCU that requires it).

## Useful define
* ADCTOUCH_INTERNAL_GROUNDING: If this is defined, you don't need a sacrificial pin on your board.