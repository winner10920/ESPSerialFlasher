#ifndef ESP_FLASHER_H
#define ESP_FLASHER_H

#include "serial_io.h"

#include "Arduino.h"
#include "SD.h"




void ESPFlasherInit();
void ESPFlasherConnect();
void ESPFlashBin(const char* binFilename);



esp_loader_error_t connect_to_target(uint32_t higher_baudrate);
esp_loader_error_t flash_binary(File file, size_t size, size_t address);

#endif
