[![Arduino Badge](https://github.com/winner10920/ESPSerialFlasher/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/winner10920/ESPSerialFlasher/actions/workflows/check-arduino.yml)
[![Compile Badge](https://github.com/winner10920/ESPSerialFlasher/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/winner10920/ESPSerialFlasher/actions/workflows/compile-examples.yml)
# Esp Serial flasher

## Overview
Provides the ability to flash the ESP32 that is onboard the Arduino nano 33 iot board

## Limitations
This library is made by myself with limited knowledge of the underlying ESP Flasher code, it's basically an Arduino port of the espressif flasher (https://github.com/espressif/esp-serial-flasher) . It's worked well for me but there are no guaruntees and I welcome other people to help contribute to make this more refined and more usable for the general public

## Requirements
An Arduino nano 33 iot with an sd card module attached to standard SPI pins. Uses the SD library included with the arduino IDE.
The binary file ive tested it with is made from the combine.py script in the nina-fw repo. (https://github.com/arduino/nina-fw)

## Example
Below is a screenshot from the Download and flash example
Demonstrates updating from Nina firmware version 1.4.6 to 1.4.7 in the same program
<img src="img\DownloadAndFlash.png" width="auto" height="auto"> 


## Dependencies
  - [SD](https://github.com/arduino-libraries/SD)

## Licence

Code is distributed under Apache 2.0 license.

## Credits
This Library is based off of code from (https://github.com/espressif/esp-serial-flasher)

## Contact Me
If you have any Questions, Comments, Concerns please email me rjfedor@hotmail.com
