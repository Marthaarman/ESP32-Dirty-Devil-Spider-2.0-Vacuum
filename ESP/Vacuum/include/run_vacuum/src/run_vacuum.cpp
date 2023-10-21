#include "run_vacuum.h"
#include "ESP32_Vacuum.h"
#include "ESP32_MQTT.h"
#include "ESP32_WiFi.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"

extern "C" {
    void run_vacuum() {
        ESP32_Vacuum *esp32_vacuum = new ESP32_Vacuum();
        ESP32_WiFi *esp32_wifi = new ESP32_WiFi();
        ESP32_MQTT *esp32_mqtt = new ESP32_MQTT();
        
        esp32_wifi->Initialize();
        esp32_wifi->Start();
        
        while(true){
            if(esp32_wifi->Connected()) {
                break;
            }
        }
        printf("Connected\n");
        esp32_mqtt->Initialize(esp32_vacuum);


        while(true) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}