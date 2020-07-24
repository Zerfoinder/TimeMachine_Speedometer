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

#include <Arduino.h>
#include "Speedometer.h"

namespace bttf::timemachine {

    const int Speedometer::blinkIntervals[2][11] = {
        {  50, 400, 100, 200, 140, 100, 200, 200, 150, 100, 100},
        {   0,   1,   0,   1,   2,   3,   0,   1,   2,   3,   2}
    };

    Speedometer::Speedometer(SpeedoPins speedoPins)
        : tm_display { TM1637Display(speedoPins.clock, speedoPins.data) } {
        _state = SpeedoState::off;
        _value = 0;
        _lastLoopMillis = 0;
        _brightInterval = 0;
    }

    void Speedometer::init(void) {
        tm_display.setBrightness(SpeedoBrightLevel::low);
        _clear();
    }

    void Speedometer::on(void) {
        tm_display.setBrightness(SpeedoBrightLevel::medium);
        _state = SpeedoState::on;
        tm_display.showNumberDecEx(_value, 0x40, true, 3, 0);
    }

    void Speedometer::brighter(unsigned long duration) {
        _lastLoopMillis = millis();
        tm_display.setBrightness(SpeedoBrightLevel::high);
        _state = SpeedoState::bright;
        _brightInterval = duration;
        tm_display.showNumberDecEx(_value, 0x40, true, 3, 0);
    }

    void Speedometer::blink(void) {
        _lastLoopMillis = millis();
        _state = SpeedoState::blinking;
        _blinkIntervalIndex = 0;
        _displayBlink(blinkIntervals[1][_blinkIntervalIndex]);
    }

    void Speedometer::off(void) {
        _clear();
        _state = SpeedoState::off;
        _value = 0;
        _lastLoopMillis = 0;
        _brightInterval = 0;
        _blinkIntervalIndex = 0;
    }

    SpeedoState Speedometer::state(void) {
        return _state;
    }

    void Speedometer::setValue(int value) {
        value *= 10;

        if (value >= 0 && value <= 880) {
            _value = value;
            tm_display.showNumberDecEx(_value, 0x40, true, 3, 0);
        }
    }

    int Speedometer::getValue(void) {
        return _value;
    }

    void Speedometer::moveTo(int newValue, unsigned long duration) {
        _lastLoopMillis = millis();

        int n = newValue * 10;  // new value validation
        _newValue = _validateNewValue(n);

        // calculate `_moveToIncrement` and `_moveToInterval` values
        // TODO: Improve this calculation.
        int diff = _newValue - _value;
        int direction = diff < 0 ? -1 : 1;

        long preInterval = duration / diff * direction;

        if (preInterval < 6) {
            _moveToIncrement = 9 * direction;
        } else if (preInterval < 10 ) {
            _moveToIncrement = 6 * direction;
        } else if (preInterval < 20 ) {
            _moveToIncrement = 4 * direction;
        } else {
            if (preInterval % 1000 == 0)
                _moveToIncrement = 1 * direction;
            else
                _moveToIncrement = 2 * direction;
        }
        _moveToInterval = duration / (diff / _moveToIncrement);
        // calculation ends

        _state = SpeedoState::moving; // change state
    }

    void Speedometer::loop(void) {
        switch (_state) {
        case SpeedoState::blinking:
            if (millis() >= _lastLoopMillis + blinkIntervals[0][_blinkIntervalIndex]) {
                _lastLoopMillis += blinkIntervals[0][_blinkIntervalIndex];
                _blinkIntervalIndex++;
                if (_blinkIntervalIndex == 11) {
                    _blinkIntervalIndex = 0;
                    on();
                } else {
                    _displayBlink(blinkIntervals[1][_blinkIntervalIndex]);
                }
            }
            break;

        case SpeedoState::moving:
            if (millis() >= _lastLoopMillis + _moveToInterval) {
                _lastLoopMillis += _moveToInterval;
                _update();
                tm_display.showNumberDecEx(_value, 0x40, true, 3, 0);
            }
            break;

        case SpeedoState::bright:
            if (millis() >= _lastLoopMillis + _brightInterval) {
                on();
            }
        }
    }

    void Speedometer:: _update(void) {
        // check if _newValue has been reached
        if ( ((_moveToIncrement > 0) && (( _value + _moveToIncrement) >= _newValue))
                || ((_moveToIncrement < 0) && (( _value + _moveToIncrement) <= _newValue))  ) {

            _moveToIncrement = 0;
            _moveToInterval = 0;
            _value = _newValue;

            on();
        } else
            _value += _moveToIncrement;
    }

    void Speedometer::_displayBlink(byte blinkstep) {
        switch (blinkstep) {
        case 0: // off
            _clear();
            break;
        case 1: // low
            tm_display.setBrightness(SpeedoBrightLevel::low);
            tm_display.showNumberDecEx(_value, 0x40, true, 3, 0);
            break;
        case 2: // medium
            tm_display.setBrightness(SpeedoBrightLevel::medium);
            tm_display.showNumberDecEx(_value, 0x40, true, 3, 0);
            break;
        case 3: // high
            tm_display.setBrightness(SpeedoBrightLevel::high);
            tm_display.showNumberDecEx(_value, 0x40, true, 3, 0);
            break;
        }
    }

    void Speedometer:: _clear(void) {
        uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
        tm_display.setSegments(data);
    }

    int Speedometer::_validateNewValue(int newValue) {
        if (newValue > maxSpeedometerValue)
            return maxSpeedometerValue;
        else if (newValue < minSpeedometerValue)
            return minSpeedometerValue;
        else
            return newValue;
    }
} // end namespace bttf::timemachine
