# sme-arduino-core
Arduino core for Smart Everything board

v2.0.0 Third Release 09-May-2016
---------------------------------
* Fixed SPI error on Fox family
* End support for FOX unit
* Added new Fox3 Unit
* Added new Lion Unit

v1.2.0 Second Release 09-May-2016
---------------------------------
* Added SmartTutto unit .
* Internal change to move the board from AMEL-Tech to Arrow in the BoardManager.
* minor change in variantInit to make the 2 unit compatible.

v1.1.0 Second Release 18-Dec-2015
---------------------------------
* Add new function to flash the RGB LED light for X milliseconds<br>
    `void flashRGBLed(uint32_t color, uint32_t time_in_ms);`<br><br>
* Added new GPS & SFX commands<br>
    `gpsForceOn() = send an HW wakeup to GPS`<br>
    `sfxSleep()   = put SFX in sleep`<br>
    `sfxWakeup()  = send an HW wakeup to SFX`<br>

v1.0.0 First Release
--------------------
