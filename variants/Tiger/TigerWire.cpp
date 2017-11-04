/*
 * TWI/I2C library for Arduino Zero
 * based on Copyright (c) 2011 Cristian Maglie <c.maglie@bug.st>.
 * Copyright (c) 2014 Arduino.
 * All rights reserved.
 *
 * Copyright (c) 2015 Amel-Tech (a division of Amel Srl). All right reserved.
 * 
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

extern "C" {
#include <string.h>
}

#include "TigerWire.h"
#include "variant.h"
#include "wiring_private.h"

TigerWire::TigerWire(SERCOM * s)
{
	this->sercom = s;
	transmissionBegun = false;
}

void TigerWire::begin(void) {
	//Master Mode
	sercom->initMasterWIRE(TWI_CLOCK);
	sercom->enableWIRE();

  pinPeripheral(PIN_WIRE_SDA, g_APinDescription[TIGER_PIN_WIRE_SDA].ulPinType);
  pinPeripheral(PIN_WIRE_SCL, g_APinDescription[TIGER_PIN_WIRE_SCL].ulPinType);
}

uint8_t TigerWire::requestFrom(uint8_t address, size_t quantity, bool stopBit)
{
  if(quantity == 0)
  {
    return 0;
  }


  size_t byteRead = 0;

  if(sercom->startTransmissionWIRE(address, WIRE_READ_FLAG))
  {

    // Read first data
    rxBuffer.store_char(sercom->readDataWIRE());

    // Connected to slave
    //while(toRead--)
    for(byteRead = 0; byteRead < quantity; ++byteRead)
    {
      if( byteRead == quantity - 1)  // Stop transmission
      {
        sercom->prepareNackBitWIRE(); // Prepare NACK to stop slave transmission
        //sercom->readDataWIRE(); // Clear data register to send NACK
        sercom->prepareCommandBitsWire(WIRE_MASTER_ACT_STOP); // Send Stop
      }
      else // Continue transmission
      {
        sercom->prepareAckBitWIRE();  // Prepare Acknowledge
        sercom->prepareCommandBitsWire(WIRE_MASTER_ACT_READ); // Prepare the ACK command for the slave
        rxBuffer.store_char( sercom->readDataWIRE() );  // Read data and send the ACK
      }
    }
  }

  return byteRead;
}

uint8_t TigerWire::requestFrom(uint8_t address, size_t quantity)
{
	return requestFrom(address, quantity, true);
}

void TigerWire::beginTransmission(uint8_t address) {
	// save address of target and clear buffer
	txAddress = address;
	txBuffer.clear();

	transmissionBegun = true;
}

// Errors:
//	0 : Success
//	1 : Data too long
//	2 : NACK on transmit of address
//	3 : NACK on transmit of data
//	4 : Other error
uint8_t TigerWire::endTransmission(bool stopBit)
{
	transmissionBegun = false ;

	// Check if there are data to send
	if ( txBuffer.available() == 0)
  {
		return 4 ;
  }

	// Start I2C transmission
	if ( !sercom->startTransmissionWIRE( txAddress, WIRE_WRITE_FLAG ) )
  {
    sercom->prepareCommandBitsWire(WIRE_MASTER_ACT_STOP);
		return 2 ;	// Address error
  }

	// Send all buffer
	while( txBuffer.available() )
	{

		// Trying to send data
		if ( !sercom->sendDataMasterWIRE( txBuffer.read_char() ) )
    {
      sercom->prepareCommandBitsWire(WIRE_MASTER_ACT_STOP);
			return 3 ;	// Nack or error
    }


    if(txBuffer.available() == 0)
    {
      sercom->prepareCommandBitsWire(WIRE_MASTER_ACT_STOP);
    }
	}

	return 0;
}

uint8_t TigerWire::endTransmission()
{
	return endTransmission(true);
}

size_t TigerWire::write(uint8_t ucData)
{
	if(sercom->isMasterWIRE())
	{
		// No writing, without begun transmission or a full buffer
		if ( !transmissionBegun || txBuffer.isFull() )
    {
      return 0 ;
    }

		txBuffer.store_char( ucData ) ;

		return 1 ;
	}
	else
	{
		if(sercom->sendDataSlaveWIRE( ucData ))
    {
			return 1;
    }
	}

	return 0;
}

size_t TigerWire::write(const uint8_t *data, size_t quantity)
{
	//Try to store all data
	for(size_t i = 0; i < quantity; ++i)
	{
		//Return the number of data stored, when the buffer is full (if write return 0)
		if(!write(data[i]))
			return i;
	}

	//All data stored
	return quantity;
}

int TigerWire::available(void)
{
	return rxBuffer.available();
}

int TigerWire::read(void)
{
	return rxBuffer.read_char();
}

int TigerWire::peek(void)
{
	return rxBuffer.peek();
}

void TigerWire::flush(void)
{
	// Do nothing, use endTransmission(..) to force
	// data transfer.
}

void TigerWire::onReceive(void(*function)(int))
{
	onReceiveCallback = function;
}

void TigerWire::onRequest(void(*function)(void))
{
	onRequestCallback = function;
}

void TigerWire::onService(void)
{
	if ( sercom->isSlaveWIRE() )
	{
		if(sercom->isStopDetectedWIRE() ||
		(sercom->isAddressMatch() && sercom->isRestartDetectedWIRE() && !sercom->isMasterReadOperationWIRE())) //Stop or Restart detected
		{
			sercom->prepareAckBitWIRE();
			sercom->prepareCommandBitsWire(0x03);

			//Calling onReceiveCallback, if exists
			if(onReceiveCallback)
			{
				onReceiveCallback(available());
			}
			
			rxBuffer.clear();
		}
		else if(sercom->isAddressMatch())  //Address Match
		{
			sercom->prepareAckBitWIRE();
			sercom->prepareCommandBitsWire(0x03);

			if(sercom->isMasterReadOperationWIRE()) //Is a request ?
			{
				txBuffer.clear();

				transmissionBegun = true;

				//Calling onRequestCallback, if exists
				if(onRequestCallback)
				{
					onRequestCallback();
				}
			}
		}
		else if(sercom->isDataReadyWIRE())
		{
			if (sercom->isMasterReadOperationWIRE())
			{
				uint8_t c = 0xff;

				if( txBuffer.available() ) {
					c = txBuffer.read_char();
				}

				transmissionBegun = sercom->sendDataSlaveWIRE(c);
				} else { //Received data
				if (rxBuffer.isFull()) {
					sercom->prepareNackBitWIRE();
					} else {
					//Store data
					rxBuffer.store_char(sercom->readDataWIRE());

					sercom->prepareAckBitWIRE();
				}

				sercom->prepareCommandBitsWire(0x03);
			}
		}
	}
}

Wire WireTiger(&PERIPH_WIRE);

  void TIGER_WIRE_IT_HANDLER(void) {
	  WireTiger.onService();
  }
