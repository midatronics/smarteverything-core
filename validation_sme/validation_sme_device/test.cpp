/*
  Copyright (c) 2014 Arduino.  All right reserved.

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

#define ARDUINO_MAIN
#include "Arduino.h"
#include <Wire.h> 
#include "extEEPROM.h"


extEEPROM myEEPROM(kbits_256, 1, 64, 0x57);


void setup(void)
{
	Serial1.begin(115200);
	while (!Serial1) {
    	;
	}

    byte i2cStat = myEEPROM.begin(twiClock100kHz);
    if ( i2cStat != 0 ) {
        Serial1.println("I2C Problem");
    }

    Serial1.println("EEPROM Memory commands:  read:(a)(l)(r) , write:(a)(d)(w), next read data (n)");
    Serial1.println("- Commands TO PRESS:");
    Serial1.println("\t a : memory address to read / write");
    Serial1.println("\t d : data to write");
    Serial1.println("\t l : data to write");
    Serial1.println("\t r : read command");
    Serial1.println("\t w : write command");
}

unsigned long address = 0;
const unsigned int maxDataSize = 1024;  //0x8000;   // 32 k bytes (32768 = 0x8000) = 256 kbits

byte data[maxDataSize] = {'p','i','p', 'p', 'o'};
unsigned int dataSize = 5;

void eprom_read_write(bool write)
{
    byte i2cStat = 0;
    if (write) {
        i2cStat = myEEPROM.write(address, data, dataSize);
    } else {
        memset(data, 0, maxDataSize);
        i2cStat = myEEPROM.read(address, data, dataSize);
    }
    if ( i2cStat != 0 ) {
        //there was a problem
        Serial1.print("I2C Problem: ");
        if ( i2cStat == EEPROM_ADDR_ERR) {
            Serial1.println("Wrong address");
        } else {
            Serial1.print("I2C error: ");
            Serial1.print(i2cStat);
            Serial1.println("");
        }
    }
}


void parse(char inChar) 
{
    const char addr_len = 5;
    char       addr_char[addr_len] = "";
    const char data_len = 3;
    char       data_char[data_len] = "";
    char       size_char[data_len] = "";
    char       inc = 0, i = 0, j = 0;

    switch (inChar) {
    case 'a':
            Serial1.print("Insert Address as 4 Hex chars (without '0x'):  ");
 
            while (i < 4) {
                while (Serial1.available() <= 0)
                    ;
                inc = Serial1.read();

                if (inc == 'q') 
                    return;

                addr_char[i] = inc;
                ++i;
            }            
            address = (unsigned long)strtol(addr_char, NULL, 16);
            Serial1.println(address);
    break;

    case 'd':
            Serial1.print("Insert Hex data sequence (without '0x'), return to enter: ");
            memset(data, 0, maxDataSize);
            while (true) {
                while (Serial1.available() <= 0)
                ;
                inc = Serial1.read();
                if (inc == 'q')
                    return;
                if (inc == '\r' || inc == '\n')
                    break;

                if (inc >= 'a' && inc <= 'f')
                    data[j] += inc-'a'+10;
                else  if (inc >= 'A' && inc <= 'F')
                    data[j] += inc-'A'+10;
                else if (inc >= '0' && inc <= '9')
                    data[j] += inc-'0';
                else return;

                if (i%2) {
                    j++;
                } else {
                    data[j] = data[j] << 4;
                }
                i++;
            }
            dataSize = j;
            Serial1.println(dataSize);
            Serial1.println("");
    break;
    case 'l':
            Serial1.print("Insert data len as 2 Hex chars (without '0x'): ");
            while (i < 2) {
                while (Serial1.available() <= 0)
                ;
                inc = Serial1.read();
                if (inc == 'q')
                return;

                size_char[i] = inc;
                ++i;
                if (inc == '\n') {
                    return;
                }
            }

            dataSize = (unsigned int)strtol(size_char, NULL, 16);
            Serial1.println(dataSize);
    break;


    case 'n':
          address += dataSize;
          /* FALLTHROUGH */
    case 'r':  
            Serial1.print("reading address: ");
            Serial1.println(address, HEX);

            eprom_read_write(false);
            for (i = 0; i < dataSize ; ++i) {
                Serial1.print(data[i], HEX);
                Serial1.print(" ");
            }
            Serial1.println();

    break;

    case 'w':
            Serial1.print("writing at address: ");
            Serial1.print(address, HEX);
            Serial1.print(", len: ");
            Serial1.println(address, dataSize);
            for (i = 0; i < dataSize ; ++i) {
                Serial1.print(data[i], HEX);
                Serial1.print(" ");
            }
            eprom_read_write(true);
            Serial1.println();

    break;
    case 'T':
            Serial1.println("Memory test: writing and verifying the whole memory");
    break;

    default:
    break;
    }
}


void loop(void)
{
	if (Serial1.available() > 0) {
		char inChar = Serial1.read();
        Serial1.print(inChar);
		parse(inChar);
    }

    delay(10);
}

