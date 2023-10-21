#ifndef _ESP32_MQTT_H_
#define _ESP32_MQTT_H_

#include "esp_event.h"
#include "ESP32_Vacuum.h"
#include "mqtt_client.h"

class ESP32_MQTT {

    private:
        esp_mqtt_client_handle_t _mMQTTClient;
        ESP32_Vacuum* _mESP32_Vacuum;

        static void _EventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

    public:

        void Initialize(ESP32_Vacuum* esp32_vacuum);
        void Start();
        void Check();
};

#endif