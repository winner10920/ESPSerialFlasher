#ifndef _JUSTHTTP_
#define _JUSTHTTP_
#include "arduino_secrets.h"




const byte GET_METHOD = 1;
const byte POST_METHOD = 2;
const byte OTA_METHOD = 3;

long HTTPGETRequest(const char* host, const char* path, Stream &client );
long HTTPPOSTRequest(const char* host, const char* path, const char* postData, Stream &client  );
long HTTPOTARequest(const char* host, const char* path, Stream &client );

long HTTPRequest(byte method, const char* host, const char* path, const char* postData, Stream &client );
void emptyRequestBuffer(Stream &client , bool print = true);



long HTTPRequest(byte method, const char* host, const char* path, const char* postData, Stream &client  ){
 char pathHeader[] = "%s %s HTTP/1.1";
 char requestHeader[128];
 char postContentHeader[64];

    emptyRequestBuffer(client);


    if((method == GET_METHOD) || (method == OTA_METHOD)){
      snprintf(requestHeader, sizeof(requestHeader), pathHeader, "GET", path);
     }
    else if(method == POST_METHOD) {
    snprintf(requestHeader, sizeof(requestHeader), pathHeader,"POST", path);
    char INSERT_LENGTH[] = "Content-Length: %i";
    snprintf(postContentHeader,sizeof(postContentHeader),INSERT_LENGTH,strlen(postData));
    }
    else return -2;

    char INSERT_HOST[] = "Host: %s";
    char hostHeader[64];
    sprintf(hostHeader,INSERT_HOST,host);

    char INSERT_USER_AGENT[] = "User-Agent: %s ";
    char UserAgentHeader[sizeof(INSERT_USER_AGENT)+sizeof(DEVICE)+2];
    snprintf(UserAgentHeader, sizeof(UserAgentHeader), INSERT_USER_AGENT, DEVICE);

    client.println(requestHeader);
    client.println(hostHeader);
    client.println(UserAgentHeader);
    client.println("Connection: keep-alive");
    if(DEBUG){
       debugPort->println(requestHeader);
       debugPort->println(hostHeader);
       debugPort->println(UserAgentHeader);
       debugPort->println("Connection: keep-alive");

    }

    if((method == GET_METHOD) || (method == OTA_METHOD)){
      client.println();
      if(DEBUG) debugPort->println();
    } else
    if(method == POST_METHOD){
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println(postContentHeader);
      client.println();
      client.print(postData);
      if(DEBUG){
        debugPort->println("Content-Type: application/x-www-form-urlencoded");
        debugPort->println(postContentHeader);
        debugPort->println();
        debugPort->print(postData);
        }
    }

    
    
// handle response
  boolean eoh = false;
  bool foundHTTP = false;
  int statusCode = 0;
  bool foundContentLength = false;
  char contentType[64];
  int contentTypeCount = 0;
  bool binaryFileFound = 0;
  bool jsonFileFound = 0;
  bool otherFileFound = 0;
  long contentLength = -1;
  String searchBuffer;
  unsigned long ResponseDelayTimer = millis();
  
  while((millis() - ResponseDelayTimer < HTTPResponseTime) || client.available()){
   if(!eoh){
    if(!foundHTTP) foundHTTP = client.find("HTTP/1.1 ");
    else if(!statusCode) statusCode = client.parseInt();
   if(statusCode){
   searchBuffer = client.readStringUntil(':');
   if(DEBUG) debugPort->println(searchBuffer);
   if(searchBuffer.equals("Content-Length") )
    {
    char c = client.peek(); if(c == ' '){client.read();} 
    contentLength = client.parseInt(SKIP_NONE);
    c = client.peek();
    while((c == '\r') || (c == '\r')){ client.read();c = client.peek();}
    }
   else
   if(searchBuffer.equals("Content-Type"))
   {
    char c = client.peek(); if(c == ' '){client.read();} 
    contentTypeCount = client.readBytesUntil('\r',contentType,sizeof(contentType)); contentType[contentTypeCount] = '\0';
    if(strcmp(contentType,"application/octet-stream") ==0) binaryFileFound = true; else
    if(strcmp(contentType,"application/json") ==0) jsonFileFound = true; else
    otherFileFound= true;
    client.read();
    }
   else
   client.readStringUntil('\n');
   char c = client.peek();
   if(c == '\r'){eoh = client.find("\n");}

   
   }// end eoh
  if(eoh) break;
   }
  }// end while avaible
if(DEBUG ){
    debugPort->println("found end of header");
    debugPort->println();
    debugPort->print("foundHTTP: ");debugPort->println(foundHTTP);
    debugPort->print("statusCode: ");debugPort->println(statusCode);
    debugPort->print("foundContentLength: ");debugPort->println(contentLength);
    debugPort->print("contentTypeCount: ");debugPort->println(contentTypeCount);
    debugPort->print("contentType: ");debugPort->println(contentType);
    debugPort->print("binaryFileFound: ");debugPort->println(binaryFileFound);
    debugPort->print("jsonFileFound: ");debugPort->println(jsonFileFound);
    debugPort->print("otherFileFound: ");debugPort->println(otherFileFound);
    debugPort->print("found end-of-header: ");debugPort->println(eoh);
    debugPort->println();
}

    if((method == OTA_METHOD) && (!binaryFileFound)) contentLength = -2;

  return contentLength;
}




long HTTPGETRequest(const char* host, const char* path, Stream &client){
  
  long contentLength = HTTPRequest( GET_METHOD, host,  path,  "",client);
  
  return contentLength;
}


long HTTPPOSTRequest(const char* host, const char* path, const char* postData, Stream &client){

  long contentLength = HTTPRequest( POST_METHOD, host,  path,  postData,client);

  return contentLength;
}

long HTTPOTARequest(const char* host, const char* path, Stream &client){
  
  long contentLength = HTTPRequest( OTA_METHOD, host,  path, "",client  );

  return contentLength;

  
}



void emptyRequestBuffer(Stream &client, bool print){
  unsigned long emptyBufferTime = millis() + 250;
  while( client.available() || (emptyBufferTime > millis()))
      {
        if(client.available())
        {
          if(DEBUG && print) debugPort->write(client.read());
          else client.read();
        }
      }
}



















#endif
