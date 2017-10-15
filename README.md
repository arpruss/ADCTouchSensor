## ADCTouch  
ADCTouchSTM is a library that allows users to create a capacitive sensor without ANY external hardware.
It is based on the ideas in https://github.com/martin2250/ADCTouch

## Purpose  
Most capacitive touch libraries require two pins and a large resistor to acquire precise readings. 
This library inspired by the ADCTouch library makes use of internal wiring to get decent 
resolution with just a single pin (plus on some MCUs one extra sacrificial pin, shared between allows
the touch sensors).

## Usage  
`ADCTouch touchSensor = ADCTouch(int analogPin, int sacrificialPin = -1);`
`touchSensor.begin();`
`value = touchSensor.read(int samples=5);`
`value = touchSensor.readRaw(int samples=5);`

* analogPin: The pin to read data from

* sacrificialPin is grounded (so beware of shorts) and used for internal purposes. A single 
    sacrificialPin can be used for many ADCTouch instances. On the stm32f103c, you can omit 
    the sacrificial pin, and an internal grounded ADC channel
    can be used. If your MCU has such a channel -- likely, the same one, namely 15 -- you can
    edit the library to support it (and email me)

* samples: number of samples to take

* begin(): reads a sample reference piece of data

* read(): reads a value and corrects it via the reference data; the value can in principle range
    from -1024 to 1023, but in practice tends to be in the single digits (plus or minus) if there
    is no touch and at least 100 if there is a touch. Returns -10000 if the setup is wrong (i.e.,
    no sacrificialPin and wrong MCU).

* rawRead(): reads but does not correct via reference data; value can range from 0 to 1023.
     Returns -10000 if the setup is wrong (i.e., no sacrificialPin and wrong MCU).