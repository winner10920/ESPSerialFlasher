#ifndef _SECRETS_
#define _SECRETS_
#define SECRET_SSID ""         			      //enter Wifi Name here
#define SECRET_PASS ""   			            //enter Wifi password here

bool DEBUG = false;                       //change to false to not get any debug messages from the additional pages
Print *debugPort = &Serial;               //change to change where debug messages go, could be any Print class inheritor, e.g Serial, Serial1, WifiClient, (*untested* Sd file)



unsigned long HTTPResponseTime = 10000;   //timer for how long to wait for the http response

 #define SDCARD_SS_PIN 10                 //pin hooked up to the sd card


const char DEVICE[]= "Arduino NANO 33 IoT";//inserted as user agent for the http request





#endif
