/**
 * ----------------------------------------------------------------
 * Copyright (C) 2020  Luis Acosta <zerfoinder@gmail.com>
 *
 * Speedometer library, This is a library to emulte Back to the
 * Future Delorean digital speedometer with a 7-Segment 4 digit display
 * to run from 0 to 88mph. It uses a TM1637 display.
 *
 * www.github.com/Zerfoinder/TimeMachine_Speedometer (github as default source provider)
 *
 * This file is part of Speedometer library.
 *
 * Speedometer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Speedometer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------
 */

/**
 * name:         Speedometer
 * version:      0.1.1
 * Author:       Luis Acosta <zerfoinder@gmail.com>
 * Date:         2020-07-23
 * Description:  Speedometer is a library to emulte Back to the Future
 *               Delorean time machine digital speedometer using a 7-Segment 4
 *               digit display to run from 0 to 88mph. It uses TM1637 display.
 */

#ifndef TIMEMACHINE_SPEEDOMETER_H
#define TIMEMACHINE_SPEEDOMETER_H

#include <Arduino.h>
#include <TM1637Display.h>

namespace bttf::timemachine {
    struct SpeedoPins {
        byte clock;
        byte data;
    };

    enum SpeedoBrightLevel {
        low = 0,
        medium = 2,
        high = 5
    };

    enum SpeedoState {
        off      = 0,
        on       = 1,
        bright   = 2,
        blinking = 3,
        moving   = 4
    };

    /**
     * @brief      This class describes a Speedometer.
     *
     *             The Speedometer is displayed in a 7-Segment 4 digit display.
     *
     *             It moves from 0 to 88mph.
     *
     *             In order to keep major screen accuracy. We only use first
     *             three digits.
     */
    class Speedometer {
        TM1637Display tm_display;

        static const int maxSpeedometerValue = 880;
        static const int minSpeedometerValue = 0;
        static const int blinkIntervals[][11];

      public:

        /**
         * @brief      Constructs a new instance.
         */
        Speedometer(SpeedoPins speedoPins);

        /**
         * @brief      Initializes the object.
         * @warning    It should ALWAYS run in setup() section.
         */
        void init(void);

        /**
         * @brief      Turns on Speedometer.
         */
        void on(void);

        /**
         * @brief      Turns on Speedometer brighter.
         *
         * @param[in]  duration  milliseconds duration of bright.
         *
         * @note       after duration, display returns to previous state.
         */
        void brighter(unsigned long duration);

        /**
         * @brief      Turns on Speedometer blinking.
         */
        void blink(void);

        /**
         * @brief      Turns off Speedometer.
         */
        void off(void);

        /**
         * @brief      Get the current state of Speedometer.
         *
         * @return     SpeedoState
         */
        SpeedoState state(void);

        /**
         * @brief      Sets the value to display.
         *
         * @param[in]  value  The value to display
         */
        void setValue(int value);

        /**
         * @brief      Gets the speedometer value.
         *
         * @return     The current value.
         */
        int getValue(void);

        /**
         * @brief      Moves from current value to new value
         *
         * @param[in]  newValue  The new value
         * @param[in]  duration  Milliseconds to move to new value.
         */
        void moveTo(int newValue, unsigned long duration);

        /**
         * @brief      function used to manage concurrent actions
         *
         * @warning    it should be ALWAYS added in the end of sketch loop() method.
         */
        void loop(void);

      protected:

      private:
        /************ METHODS ***************/

        /**
         * @brief      Updates Speedometer state.
         */
        void _update(void);

        /**
         * @brief      Clears the display.
         */
        void _clear(void);

        /**
         * @brief      Validate new value
         *
         * @param[in]  newValue  The new value
         *
         * @return     validated new value.
         */
        int _validateNewValue(int newValue);

        /**
         * @brief      
         *
         * @param[in]  blinkstep  The blinkstep
         */
        void _displayBlink(byte blinkstep);

        /************ VARIABLES ***************/

        /**
         * Speedometer state
         */
        SpeedoState _state;

        /**
         * current Speedometer value
         */
        int _value;

        /**
         * new Speedometer value
         */
        int _newValue;

        int _moveToIncrement;
        int _moveToInterval;
        byte _blinkIntervalIndex;

        /**
         * local variable used to save last loop millis()
         */
        unsigned long _lastLoopMillis;

        /**
         * local variable used to brighter() method
         */
        unsigned long _brightInterval;
    }; // end class Speedometer

} // end namespace bttf::timemachine

#endif // TIMEMACHINE_SPEEDOMETER_H
