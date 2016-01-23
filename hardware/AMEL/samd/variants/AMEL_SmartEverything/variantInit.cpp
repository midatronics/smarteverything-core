/*
Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
Copyright (c) 2015 Amel-Tech (a division of Amel Srl). All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Arduino.h>
#include "internalI2C.h"

static const uint32_t TWI_CLOCK_SME = 100000;

static void resetComponent() {
    
}

void initVariant() {
    
    // initialization the two Yellow Led
    LED_YELLOW_ONE_INIT;
    LED_YELLOW_TWO_INIT;
    
    // reset the I/O Extender
    pinMode(PIN_IO_EXT_RST, OUTPUT);
    digitalWrite(PIN_IO_EXT_RST, LOW);
    delay(10); // just wait a while
    digitalWrite(PIN_IO_EXT_RST, HIGH);

    // initialize The EXT_PWR Pin as input
    // it will be HIGH when the battery is connected
    pinMode(PIN_EXT_PWR, INPUT);
    // initialize the SetpUp to Disable
    pinMode(PIN_REG_ON, OUTPUT);
    setStepUp(LOW);

    // initialize the IO_Extender
    resetComponent();
}
