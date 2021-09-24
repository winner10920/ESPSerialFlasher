
#include "ESPSerialFlasher.h"
#include "Certs.h"

/*
This example allows you to upload new certificates to your wifinina module, without the sd card!
ofcourse adding alotnofncertificates will take uo a tonnofnfoash space on your arduino board however so use with caution.
the certificates included in Certs.h are from the wifinina firmware, there are alot. if your only connecting to a few sites you
can simply put just what you need in there and be able to download new ones with a more simple ota update to your arduino board board.
i dont imagine this example finding much use but i wanted to include it just because it is possible to do it this way
and a long time ago i wamted to do this exact thing until i realized updating the entire firmware would prove more useful.
for those who want ssl connections to self signed certs this may be very useful.
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
 Serial.println("hello");
 ESPFlasherInit(true, &Serial);//sets up Serial communication to wifi module, with debug messages, to Print Class of your choice
 //ESPFlasherInit(true);     //sets up communication to wifi module, sets printing debug statements to Serial 
 //ESPFlasherInit();          //sets up communication to wifi module, no debug messages
 ESPFlasherConnect();         //connects to wifi module
 ESPFlashCertFromMemory(Certificates, sizeof(Certificates));   //flashes "roots.bin" binary file from SD card to wifi module


}

void loop() {
  // put your main code here, to run repeatedly:

}
