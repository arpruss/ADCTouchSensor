Full instructions are here:
    https://www.instructables.com/id/Quick-Fruit-Piano/

Requires the ADCTouchSensor no-component capacitive touch library:
    https://github.com/arpruss/ADCTouchSensor

Attach A0-A7 to pads, fruit, cans, etc. Turn on Arduino with sketch. Wait for the LED to turn off. Then run:
    python piano.py com-port
on a connected computer to get polyphonic sound. (Make sure you have pygame installed. If not, python -m pip install pygame)

The Arduino sketch sends MIDI format stuff, so you can also use Hairless MIDI on the computer.