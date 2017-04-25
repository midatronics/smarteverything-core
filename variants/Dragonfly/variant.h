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

#ifndef _VARIANT_AMEL_SMARTEVERYTHING_
#define _VARIANT_AMEL_SMARTEVERYTHING_

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/
#define ASME3_REVISION


/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		(32768ul)

/** Master clock frequency */
#define VARIANT_MCK			  (48000000ul)

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"
#include <stdbool.h>

#ifdef __cplusplus
#include "SERCOM.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (26u)
#define NUM_DIGITAL_PINS     (14u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (1u)

#define digitalPinToPort(P)        ( &(PORT->Group[g_APinDescription[P].ulPort]) )
#define digitalPinToBitMask(P)     ( 1 << g_APinDescription[P].ulPin )
//#define analogInPinToBit(P)        ( )
#define portOutputRegister(port)   ( &(port->OUT.reg) )
#define portInputRegister(port)    ( &(port->IN.reg) )
#define portModeRegister(port)     ( &(port->DIR.reg) )
#define digitalPinHasPWM(P)        ( g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER )

/*
 * digitalPinToTimer(..) is AVR-specific and is not defined for SAMD
 * architecture. If you need to check if a pin supports PWM you must
 * use digitalPinHasPWM(..).
 *
 * https://github.com/arduino/Arduino/issues/1833
 */
// #define digitalPinToTimer(P)

// Interrupts
#define digitalPinToInterrupt(P)   ( g_APinDescription[P].ulExtInt )

// LEDs
#define PIN_LED_13           (13u)
#define PIN_LED_RXL          (26u)
#define PIN_LED_TXL          (27u)
#define PIN_LED              PIN_LED_13
#define PIN_LED2             PIN_LED_RXL
#define PIN_LED3             PIN_LED_TXL
#define LED_BUILTIN          PIN_LED_13

/*
 * Analog pins
 */
#define PIN_A0               (14ul)
#define PIN_A1               (15ul)
#define PIN_A2               (16ul)
#define PIN_A3               (17ul)
#define PIN_A4               (18ul)
#define PIN_A5               (19ul)

static const uint8_t A0  = PIN_A0 ;
static const uint8_t A1  = PIN_A1 ;
static const uint8_t A2  = PIN_A2 ;
static const uint8_t A3  = PIN_A3 ;
static const uint8_t A4  = PIN_A4 ;
static const uint8_t A5  = PIN_A5 ;
#define ADC_RESOLUTION		12

// Other pins
#define PIN_ATN              (50ul)
static const uint8_t ATN = PIN_ATN;

/*
 * Serial interfaces
 */
// Serial1
#define PIN_SERIAL1_RX       (0ul)
#define PIN_SERIAL1_TX       (1ul)
#define PAD_SERIAL1_TX       (UART_TX_PAD_2)
#define PAD_SERIAL1_RX       (SERCOM_RX_PAD_3)

// DUST
#define PIN_DUST_TX           (31ul)
#define PIN_DUST_RX           (32ul)
#define PAD_DUST_TX           (UART_TX_PAD_0)
#define PAD_DUST_RX           (SERCOM_RX_PAD_1)
#define DUST_RESET_PIN        (33ul)
#define DUST_TIM_EN_PIN       (34ul)

// SEVE
#define PIN_DUST_CTS          (PIN_A2)  // FAKE PIN!!!!!
#define USER_BUTTON           (PIN_A3)  // FAKE PIN

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 2

#define PIN_SPI_MISO         (22u)
#define PIN_SPI_MOSI         (23u)
#define PIN_SPI_SCK          (24u)
#define PERIPH_SPI           sercom1
#define PAD_SPI_TX           SPI_PAD_0_SCK_1
#define PAD_SPI_RX           SERCOM_RX_PAD_3

#define PIN_SPI_SS           (25u)

static const uint8_t SS	  = PIN_SPI_SS ;
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;

// SPI1: Connected to Sigfox and WiFi
#define USE_SPI1
#define PIN_SPI1_MISO (35u)
#define PIN_SPI1_MOSI (36u)
#define PIN_SPI1_SCK  (37u)
#define PERIPH_SPI1   sercom4
#define PAD_SPI1_TX   SPI_PAD_2_SCK_3
#define PAD_SPI1_RX   SERCOM_RX_PAD_0

// SigFox
#define SIGFOX_SPI           SPI1
#define SIGFOX_RES_PIN       (39ul)
#define SIGFOX_PWRON_PIN     (40ul)
#define SIGFOX_EVENT_PIN     (41ul)
#define SIGFOX_SS_PIN        (38ul)
#define SIGFOX_RFPWR_PIN     (42ul)

// WiFi Module
#define WIFI_SS_PIN        (43ul) // PB11
#define WIFI_IRQN_PIN      (44ul) // PB30
#define WIFI_WAKE_PIN      (45ul) // PB31
#define WIFI_CHIP_EN_PIN   (46ul) // PB0
#define WIFI_RES_PIN       (47ul) // PB1

// Needed for WINC1501B (WiFi101) library
// --------------------------------------
#define WINC1501_RESET_PIN     WIFI_RES_PIN
#define WINC1501_CHIP_EN_PIN   WIFI_CHIP_EN_PIN
#define WINC1501_INTN_PIN      WIFI_IRQN_PIN
#define WINC1501_SPI           SPI1
#define WINC1501_SPI_CS_PIN    WIFI_SS_PIN

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (20u)
#define PIN_WIRE_SCL         (21u)
#define PERIPH_WIRE          sercom3
#define WIRE_IT_HANDLER      SERCOM3_Handler

/*
 * USB
 */
#define PIN_USB_HOST_ENABLE  (28ul)
#define PIN_USB_DM           (29ul)
#define PIN_USB_DP           (30ul)



/*
    Yellow Led wrapper function
    These functions has been created for a more comfortable use 
      because internally wrap the inversion of the HIGH, LOW meaning

    Using these function it remain the same Arduino User Experience to light a led
    HIGH = Light ON
    LOW  = Light OFF

 */
void ledYellowOneLight(uint32_t value);
void ledYellowTwoLight(uint32_t value);



/*
 * The function resets all the component mounted on the base 
 *      SigFox
 *      DUST
 *      WiFi
 * The reset is executed by a LOW signal.
 * The function move LOW the sighttps://support.microsoft.com/en-us/help/2978092nal for a while and than move up again.
 */
void resetBaseComponent(void);


#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/


#ifdef __cplusplus

/*	=========================
 *	===== SERCOM DEFINITION
 *	=========================
 */
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4;
extern SERCOM sercom5;

extern Uart Serial1;
extern Uart SerialDust;
#define SIGFOX_SPI SPI1;

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_USBVIRTUAL      SerialUSB
#define SERIAL_PORT_MONITOR         SerialUSB
// Serial has no physical pins broken out, so it's not listed as HARDWARE port
#define SERIAL_PORT_HARDWARE        Serial1
#define SERIAL_PORT_HARDWARE_OPEN   Serial1


#define LED_YELLOW_TWO_INIT  pinMode(PIN_LED_RXL, OUTPUT)
#define LED_YELLOW_ONE_INIT  pinMode(PIN_LED_TXL, OUTPUT)


extern uint8_t smeInitError;

#define IOEXT_ERR       0b10000000
#define IOEXT_CONF_ERR  0b01000000
#define IOEXT_INIT_ERR  0b00100000

// Alias Serial to SerialUSB
#define Serial                      SerialUSB

#endif /* _VARIANT_AMEL_SMARTEVERYTHING_ */

