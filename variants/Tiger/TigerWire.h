/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@bug.st>.
 * All rights reserved.
 *
 * Copyright (c) 2015 Amel-Tech (a division of Amel Srl). All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TIGER_WIRE_IO_EXT_h
#define TIGER_WIRE_IO_EXT_h


#include "Wire.h"
#include "variant.h"

#include "SERCOM.h"
#include "RingBuffer.h"


class TigerWire : public Stream
{
    public:
    TigerWire(SERCOM *s);
    void begin();

    void beginTransmission(uint8_t);
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission(void);

    uint8_t requestFrom(uint8_t address, size_t quantity, bool stopBit);
    uint8_t requestFrom(uint8_t address, size_t quantity);

    size_t write(uint8_t data);
    size_t write(const uint8_t * data, size_t quantity);

    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
	void onReceive(void(*function)(int));
	void onRequest(void(*function)(void));
	void onService(void);


    using Print::write;

    private:
    SERCOM * sercom;
    bool transmissionBegun;

    // RX Buffer
    RingBuffer rxBuffer;

    //TX buffer
    RingBuffer txBuffer;
    uint8_t txAddress;

    // Callback user functions
    void (*onRequestCallback)(void);
    void (*onReceiveCallback)(int);

    // TWI clock frequency
    static const uint32_t TWI_CLOCK = 100000;

};


extern Wire WireTiger;

#endif
