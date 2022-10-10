#ifndef ESP_FLASHER_H
#define ESP_FLASHER_H

#include "serial_io.h"

#include "Arduino.h"
#include "SdFat.h"
#include "sdios.h"

#ifndef SD_FAT_TYPE
#define SD_FAT_TYPE 0
#endif
#ifndef SD_CS_PIN
#define SD_CS_PIN 10
#endif
#ifndef SPI_CLOCK
#define SPI_CLOCK SD_SCK_MHZ(50)
#endif
#ifndef SD_CONFIG
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#endif

extern SdFat SDcard;

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
