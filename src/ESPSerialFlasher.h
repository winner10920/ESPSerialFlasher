#ifndef ESP_FLASHER_H
#define ESP_FLASHER_H

#include "serial_io.h"

#include "Arduino.h"
#include "SD.h"



extern Print *ESPDebugPort; 
extern bool _ESPDebug;


void ESPFlasherInit(bool _debug = false, Print *_debugPort = &Serial );
void ESPFlasherConnect();
void ESPFlashBin(const char* binFilename);
void ESPFlashCert(const char* certFilename);
void ESPFlashCertFromMemory(const char* Certificates, unsigned long size);

esp_loader_error_t connect_to_target(uint32_t higher_baudrate);
esp_loader_error_t flash_binary(File file, size_t size, size_t address);
esp_loader_error_t flash_binary_from_memory(const uint8_t *bin, size_t size, size_t address);


#endif
