#pragma once

/*
	Exception.h

	This uses the Analog DAC in the ESP32 to generate a voltage
	proportional to the GCode S value desired. Some spindle uses
	a 0-5V or 0-10V value to control the spindle. You would use
	an Op Amp type circuit to get from the 0.3.3V of the ESP32 to that voltage.

	Part of Grbl_ESP32
	2021 -	Piergiorgio Vagnozzi

	Grbl is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Grbl is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with Grbl.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Types.h"

class Exception {
private:
    
};