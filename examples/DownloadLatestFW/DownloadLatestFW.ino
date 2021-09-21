
#include "ESPSerialFlasher.h"
#include "SD.h"

#include "JustHTTPClient.h"
#include "SDUpdate.h"
#include "wifi_functions.h"

#include <SPI.h>
#include <WiFiNINA.h>

/*
This sketch and included files Updates the wifinina firmware to the latest firmware 
according to your version of the WiFiNina library. 
Please adjust settings in arduino_secrets

This is a very complicated example but it gives you alot of flexibility, for example I use it to Monitor
my debug statements over a changeable debug port. E.g Serial to a computer, Serial1 to a bluetooth Serial Module 
(,or even (untested) logged to an sd card)

This is just a crude working example to get you started. the included files are not fully tested
but have worked well for me. Id anyone has a more elegant solution feel free to drop a suggestion or pull request
https://github.com/winner10920/ESPSerialFlasher

***WARNING***
*Trying to flash with a non-working binary or interupting the process
*mid-way will leave your wifi-module in a non functional state. 
*The only way to fix this (other than putting a proper binary on the sd card)
*will be the the IDE WIFI Firmware updater tool
*Having your board on an uninteruptible power supply is recommended for the 
*flashing process.

 */

char downloadUrl[128] ;
 
void setup() {
 Serial.begin(115200);         // Start communication With IDE to see whats going on, the below functions use "Serial" 
 Serial1.begin(115200);
 delay(5000);                  // wait 5 seconds before atarting
 
 checkWifi();                  //function onnwofi functions page to check wifi connection to APspecified in secrets
 WiFiClient client;            //create instamce of wifi client to use with requesting patest firmware url and downloading
 debugPort->print("Current WiFiNina firmware: ");
 debugPort->println(WiFi.firmwareVersion());   //print firmware version
//connect to arduino.cc to find out what the url of thr latest firmware according to library versionncurrently in use
bool  connected = client.connectSSL("downloads.arduino.cc", 443);
if(connected)
  {
  long contentLength = HTTPGETRequest("downloads.arduino.cc","/arduino-fwuploader/boards/module_firmware_index.json", client );
  if(contentLength > 0){
    bool foundBoard = client.find("\"fqbn\": \"arduino:samd:nano_33_iot\",");
      if(foundBoard){
        debugPort->println("found Board");
        bool foundVersion = client.find("\"version\": \""WIFI_FIRMWARE_LATEST_VERSION"\",");
        //bool foundVersion = client.find("\"version\": \"1.4.6\","); //could looknfor specific version like this
          if(foundVersion){
            debugPort->println("found Version");
            bool foundUrl = client.find("\"url\": \"");
              if(foundUrl){
                debugPort->println("found Url");
                client.readBytesUntil('"',downloadUrl,sizeof(downloadUrl));
                debugPort->println(downloadUrl);
              }//end found url
          }//end found version
      }//end found board
  }//end content length
emptyRequestBuffer(client, false); //empties rest of the http body
}//end if connected
else debugPort->println("Failed to connect to arduino");
client.stop();

//connect to arduino.cc to download the binary file to SD card
connected = client.connectSSL("downloads.arduino.cc", 443);
if(connected)
  {
  long contentLength = HTTPOTARequest("downloads.arduino.cc",downloadUrl, client );
  if(contentLength > 0){
      if(downloadToSD(client, contentLength,"NINAFW1.BIN")){
      emptyRequestBuffer(client, false);
      client.stop();
      WiFi.end();
      ESPFlasherInit(DEBUG, debugPort); //sets up communication to wifi module, prints Debug message to the debug port specified in secrets
    //ESPFlasherInit(DEBUG);            //sets up communication to wifi module, sets printing debug statements to Serial if DEBUG is true
    //ESPFlasherInit();                 //sets up communication to wifi module, no debug messages
      ESPFlasherConnect();              //connects to wifi module
      ESPFlashBin("NINAFW1.BIN");       //flashes "NINAFW.BIN" binary file from SD card to wifi module

    }//end if succeasful download
  }//end content length
}//end connected
                           
checkWifi();                  //flash procedure done, time to reconnect to wifi
debugPort->print("Current WiFiNina firmware: ");
debugPort->println(WiFi.firmwareVersion());   //print firmware version

}

void loop() {
  // put your main code here, to run repeatedly:
delay(60000);                 //check wifi again every minute
checkWifi();
Serial.println("loop");
//debugPort->println(WiFi.firmwareVersion());   //print firmware version

}
