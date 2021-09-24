
#include "ESPSerialFlasher.h"
#include "SD.h"

/*
in extras folder is a binary file called roots.pem. You can add your own personal
root certificatesbtonthe end of that file then run the python script nullTermRootsPem
this will create a binary file that can be flashed to the wifi nina module without disrupting
the firmware. Its also considerably faster than reflashing the entire firmware over.
please put this binary file in root directory of SD card to use this example
easiest way is to drag and drop on a computer, but how you get it there is up to you. 

***WARNING***
*Trying to flash with a non-working binary or interupting the process
*mid-way will leave your wifi-module in a non functional state. 
*The only way to fix this (other than putting a proper binary on the sd card)
*will be the the IDE WIFI Firmware updater tool
*Having your board on an uninteruptible power supply is recommended for the 
*flashing process.

 */
 
void setup() {
Serial.begin(115200);         // Start communication With IDE to see whats going on
delay(5000);                  // wait 5 seconds before atarting
 if(SD.begin(10))             //Must begin SD CARD before trying to flash Wifi module, will fail if not connected
 {
 ESPFlasherInit(true, &Serial);//sets up Serial communication to wifi module, with debug messages, to Print Class of your choice
 //ESPFlasherInit(true);     //sets up communication to wifi module, sets printing debug statements to Serial 
 //ESPFlasherInit();          //sets up communication to wifi module, no debug messages
 ESPFlasherConnect();         //connects to wifi module
 ESPFlashCert("roots.bin");   //flashes "roots.bin" binary file from SD card to wifi module
 }
}

void loop() {
  // put your main code here, to run repeatedly:

}
