#include "ESPSerialFlasher.h"

#include "serial_io.h"
#include "serial_comm.h"
#include "esp_loader.h"


static int32_t s_time_end;

 


void ESPFlasherInit(){
SerialNina.begin(115200);
pinMode(NINA_RESETN, OUTPUT);
pinMode(NINA_GPIO0, OUTPUT);

}

void ESPFlasherConnect(){
	
	connect_to_target(115200);
	
}

void ESPFlashBin(const char* binFilename){
	Serial.println("WARNING! DO NOT INTERRUPT OR WIFI-MODULE WILL BE CORRUPT");
	File UpdateFile = SD.open(binFilename, FILE_READ);
	size_t size = UpdateFile.size();
	flash_binary(UpdateFile,  size,  0x0);
	UpdateFile.close();
    loader_port_reset_target();
	
}


esp_loader_error_t loader_port_serial_write(const uint8_t *data, uint16_t size, uint32_t timeout)
{
    

   size_t err = SerialNina.write((const char *)data, size);

    if (err == size) {
        return ESP_LOADER_SUCCESS;
    } else 
        return ESP_LOADER_ERROR_FAIL;
    
}


esp_loader_error_t loader_port_serial_read(uint8_t *data, uint16_t size, uint32_t timeout)
{
	SerialNina.setTimeout(timeout);
    int read = SerialNina.readBytes( data, size);

    if (read < 0) {
        return ESP_LOADER_ERROR_FAIL;
    } else if (read < size) {
        return ESP_LOADER_ERROR_TIMEOUT;
    } else {
        return ESP_LOADER_SUCCESS;
    }
}


// Set GPIO0 LOW, then
// assert reset pin for 50 milliseconds.
void loader_port_enter_bootloader(void)
{
    digitalWrite(NINA_GPIO0, 0);
    loader_port_reset_target();
    loader_port_delay_ms(50);
    digitalWrite(NINA_GPIO0, 1);
}


void loader_port_reset_target(void)
{
    digitalWrite(NINA_RESETN, 0);
    loader_port_delay_ms(50);
    digitalWrite(NINA_RESETN, 1);
}


void loader_port_delay_ms(uint32_t ms)
{
    delay(ms );
}


void loader_port_start_timer(uint32_t ms)
{
    s_time_end = millis() + ms;
}


uint32_t loader_port_remaining_time(void)
{
    int32_t remaining = (s_time_end - millis()) ;
    return (remaining > 0) ? (uint32_t)remaining : 0;
}


void loader_port_debug_print(const char *str)
{
    Serial.print( str);
}

esp_loader_error_t loader_port_change_baudrate(uint32_t baudrate)
{
    SerialNina.begin(baudrate);
    int err = SerialNina;
    return (err == true) ? ESP_LOADER_SUCCESS : ESP_LOADER_ERROR_FAIL;
}

esp_loader_error_t connect_to_target(uint32_t higher_baudrate)
{
 esp_loader_connect_args_t connect_config = ESP_LOADER_CONNECT_DEFAULT();

    esp_loader_error_t err = esp_loader_connect(&connect_config);
    if (err != ESP_LOADER_SUCCESS) {
        Serial.print("Cannot connect to target. Error: %u\n");Serial.print(err);
        return err;
    }
    Serial.print("Connected to target\n");
    Serial.println(esp_loader_get_target());

    if (higher_baudrate && esp_loader_get_target() != ESP8266_CHIP) {
        err = esp_loader_change_baudrate(higher_baudrate);
        if (err == ESP_LOADER_ERROR_UNSUPPORTED_FUNC) {
            Serial.print("ESP8266 does not support change baudrate command.");
            return err;
        } else if (err != ESP_LOADER_SUCCESS) {
            Serial.print("Unable to change baud rate on target.");
            return err;
        } else {
            err = loader_port_change_baudrate(higher_baudrate);
            if (err != ESP_LOADER_SUCCESS) {
                Serial.print("Unable to change baud rate.");
                return err;
            }
            printf("Baudrate changed\n");
        }
    }

    return ESP_LOADER_SUCCESS;
}


esp_loader_error_t flash_binary(File file, size_t size, size_t address)
{
		
	
    esp_loader_error_t err;
    uint8_t payload[1024];
    
    Serial.print("Erasing flash (this may take a while)...\n");
    err = esp_loader_flash_start(address, size, sizeof(payload));
    if (err != ESP_LOADER_SUCCESS) {
        Serial.print("Erasing flash failed with error : ");Serial.println(err);
        return err;
    }
    Serial.print("Start programming\n");
	Serial.print("\rProgress: ");
    size_t binary_size = size;
    size_t written = 0;
    int previousProgress = -1;
    while (size > 0) {
        size_t to_read = MIN(size, sizeof(payload));
        file.read(payload,to_read);
        err = esp_loader_flash_write(payload, to_read);
        if (err != ESP_LOADER_SUCCESS) {
            Serial.print("\nPacket could not be written! Error : ");Serial.println( err);
            return err;
        }

        size -= to_read;
        written += to_read;

        int progress = (int)(((float)written / binary_size) * 100);
        if(previousProgress != progress)
        {
		previousProgress = progress;
        Serial.print(progress);Serial.print(",");
       	}
    };

    Serial.print("\nFinished programming\n");

#if MD5_ENABLED
    err = esp_loader_flash_verify();
    if (err == ESP_LOADER_ERROR_UNSUPPORTED_FUNC) {
        Serial.print("ESP8266 does not support flash verify command.");
        return err;
    } else if (err != ESP_LOADER_SUCCESS) {
        Serial.print("MD5 does not match. err: %d\n");Serial.print( err);
        return err;
    }
    Serial.print("Flash verified\n");
#endif

    return ESP_LOADER_SUCCESS;
}
