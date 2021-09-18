#ifndef _WIFIFUNCTIONS_
#define _WIFIFUNCTIONS_
#include "arduino_secrets.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"


int status = WL_IDLE_STATUS;     // the Wifi radio's status
int retries = 5;


void printMacAddress(byte mac[]);

void printWifiData();
void checkWifi();





void printMacAddress(byte mac[]) {


  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      debugPort->print("0");
    }
    debugPort->print(mac[i], HEX);
    if (i > 0) {
      debugPort->print(":");
    }
  }
  debugPort->println();
}



void printWifiData() {
  debugPort->println("Wifi data");

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  debugPort->print("IP Address: ");
  debugPort->println(ip);

    IPAddress gatewayip = WiFi.gatewayIP();
  debugPort->print("Gateway Address: ");
  debugPort->println(gatewayip);


  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  debugPort->print("MAC address: ");
  printMacAddress(mac);
  debugPort->println("Firmware version");
  debugPort->println(WiFi.firmwareVersion());

}

void printCurrentNet() {
  debugPort->println("Current Net");

  // print the SSID of the network you're attached to:
  debugPort->print("SSID: ");
  debugPort->println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  debugPort->print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  debugPort->print("signal strength (RSSI):");
  debugPort->println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  debugPort->print("Encryption Type:");
  debugPort->println(encryption, HEX);
  debugPort->println();
}


void checkWifi(){

  status = WiFi.status();
  if(DEBUG)  debugPort->print("checking wifi- current status:");
  if(DEBUG)  debugPort->println(status);
  retries = 5;
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    if(DEBUG)  debugPort->println("Communication with WiFi module failed!");
    delay(10000);
    
  }




 // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    if(DEBUG)  debugPort->println("Attempting to connect to WPA SSID: ");
    if(DEBUG)  debugPort->println(SECRET_SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
    if(retries < 0){
      if(DEBUG)  debugPort->println("max retries met, returning to loop");
      return;
    }
    retries --;


    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) if(DEBUG)   debugPort->println("Please upgrade the firmware");


  }// end while not connected


  if (status == WL_CONNECT_FAILED  || status == WL_DISCONNECTED || status == WL_CONNECTION_LOST){

    if(DEBUG)  debugPort->println("Connection to Wifi not made");
    delay(1000);
    retries = 5;
    }//end if connection not made

  // you're connected now, so print out the data:
  if(WiFi.status() == WL_CONNECTED){
  if(DEBUG)  debugPort->println("You're connected to the network");
  if(DEBUG)  printCurrentNet();
  if(DEBUG)  printWifiData();
  retries = 5;
  }

}


  #endif
