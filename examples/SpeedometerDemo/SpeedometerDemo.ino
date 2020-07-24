/*
 * Speedometer DEMO
 *
 * The circuit:
 * Connect TM1637 7-segment display
 *   - VCC to 5v
 *   - GND to GND
 *   - CLK (clock) to any digital pin (in this examples is pin 11)
 *   - DIO (data) to any other digital pin (in this examples is pin 12)
 */

#include <Speedometer.h>  // Add include to Speedometer.h

using namespace bttf::timemachine;  // Add using namespace

//Create SpeedoPins struct and initialize them with pin numbers
SpeedoPins speedoPins = { 11, 12}; // { clock, data };

// Create Speedometer object, pass speedoPins as parameter.
Speedometer speedo(speedoPins);

/**
 * Variable created for the purposes of this example
 * it is used to execute some task in 8 seconds loop.
 */
unsigned long _previousExecutionMillis = 0;


void setup() {
    speedo.init();   // Call init() method
}

int loops = 0;
void loop() {
    // Enter every 8 seconds
    if (millis() >= _previousExecutionMillis + 8000) {
        _previousExecutionMillis += 8000;

        loops++;

        switch (loops) {
        case 1:
            speedo.on(); // turns on.
            break;
        case 2:
            speedo.off(); // turns off.
            break;
        case 3:
            speedo.on(); // turns on again.
            break;
        case 4:
            speedo.setValue(10); // set value to 10.0
            break;
        case 5:
            speedo.moveTo(88, 4000);  // moves from current value (10) to 88 in 4 seconds.
            break;
        case 6:
            speedo.setValue(15); // set value to 15.0
            break;
        case 7:
            speedo.brighter(500); // turns on brither during 500 milliseconds.
            break;
        case 8:
            speedo.blink(); // turns on brither during 500 milliseconds.
            break;
        case 9:
            speedo.setValue(88); // set value to 88.0
            break;
        case 10:
            speedo.moveTo(0, 3000);  // moves from current value (88) to 0 in 6 seconds.
            break;
        case 11:
            speedo.setValue(88); // set value to 88.0
            loops = 0;
            break;
        }
    }

    speedo.loop(); // Call tree.loop() ALWAYS in the end of sketch loop().
}
