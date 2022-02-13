/*
    Common fib class for modular synth of project Fibration.
    Copyright (C) 2020 Lukas Neverauskis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "system.hpp"

#include "resources.hpp"
#include "logger.hpp"
extern "C"
{
#include "FreeRTOS.h"
}

#include "thread.hpp"
#include "timer.hpp"
#include "ticks.hpp"

#include <cmath>

#include "periodicTimerApp.hpp"
#include "dsp/oscillator.hpp"
#include "streams/i2sDuplexStream.hpp"