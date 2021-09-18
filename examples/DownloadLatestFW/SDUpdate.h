 #ifndef _SDUPDATE_
 #define _SDUPDATE_
 
 #include "arduino_secrets.h"
 #include "SD.h"


 File updateFile;
 File root;

 bool gotUpdate = false;
 unsigned long downloadTime = 120000;


 void getUpdate();
 bool downloadToSD( Stream &client, unsigned long contentLengthk, const char* filename);
 void printDirectory(File dir, int numTabs);
 bool setupSD();
 void reboot();

 
 bool downloadToSD( Stream &client, unsigned long contentLengthk, const char* filename ){
  if(DEBUG){  
      debugPort->println("Download to SD");
    }
    
  File UpdateFile;
  if(!setupSD()) return false;
  if(SD.exists(filename)) SD.remove(filename);
  UpdateFile = SD.open(filename, FILE_WRITE);
  if (UpdateFile) {
    if(DEBUG) {debugPort->print("Writing to ");debugPort->print(filename); debugPort->print(" ...");}
    unsigned long downloadTimer = millis() + downloadTime;
    unsigned long downloadProgress = 0;
    unsigned long kbDownloaded = 0;
    while((downloadTimer > millis()) && (downloadProgress < contentLengthk )){ 
    while(client.available()){
      UpdateFile.write(client.read());
      downloadProgress++;
    }
    if((downloadProgress / 1024) > kbDownloaded){
      kbDownloaded = downloadProgress / 1024;
      if(DEBUG) {debugPort->print(kbDownloaded);debugPort->print(" kb, ");}
    }
    }
   
    //UpdateFile.print(response);
    // close the file:
    UpdateFile.close();
    if(DEBUG){debugPort->print("done in: "); debugPort->println(downloadTime - (downloadTimer - millis()));}
  } else {
    // if the file didn't open, print an error:
    if(DEBUG) debugPort->println("error opening UPDATE.BIN");
  }
   UpdateFile = SD.open(filename, FILE_READ);
   unsigned long fileSize = UpdateFile.size();
   if(DEBUG){
     debugPort->print("fileSize :");
     debugPort->println(fileSize);
     debugPort->print("contentLength :");
     debugPort->println(contentLengthk);
   }
  UpdateFile.close();
  //client.stop();

if(fileSize != contentLengthk){
  SD.remove(filename);
  if(DEBUG) debugPort->println("download failed");
  return false;
} else {
  if(DEBUG) debugPort->println("download successful");
  gotUpdate = true;
  return true;
}

if(DEBUG){
  root = SD.open("/");
  printDirectory(root, 0);
  root.close();
}
  
}


 
 bool setupSD(){
    if(DEBUG){  
      debugPort->println("SD begin");
    }
    
    bool SDCardPresent = SD.begin(SDCARD_SS_PIN);
    
    if(DEBUG){  
      debugPort->print("SD status: ");
      debugPort->println(SDCardPresent);
      debugPort->println("Sd card Root:");
      root = SD.open("/");
      printDirectory(root, 0);
      root.close();
    }

    return SDCardPresent;
 }


void printDirectory(File dir, int numTabs) {

 
  debugPort->println("Print SD Directory");
  while (true) {

    File entry =  dir.openNextFile();

    if (! entry) {

      // no more files

      break;

    }

    for (uint8_t i = 0; i < numTabs; i++) {

      debugPort->print('\t');

    }

    debugPort->print(entry.name());

    if (entry.isDirectory()) {

      debugPort->println("/");

      printDirectory(entry, numTabs + 1);

    } else {

      // files have sizes, directories do not

      debugPort->print("\t\t");

      debugPort->println(entry.size(), DEC);

    }

    entry.close();

  }
}


#endif
