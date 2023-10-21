#include "ESP32_MQTT.h"
#include "esp_log.h"


const char* MQTT_TAG = "MQTT";


void ESP32_MQTT::_EventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
    ESP32_MQTT* pMQTTObject = static_cast<ESP32_MQTT*>(handler_args);

    std::string Message;
    std::string Topic;

    switch(event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(MQTT_TAG, "MQTT Connected");
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(MQTT_TAG, "MQTT connection lost");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(MQTT_TAG, "Subscription successful");
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(MQTT_TAG, "Unsubscription successful.");
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(MQTT_TAG, "Publishing successful.");
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(MQTT_TAG, "Received message from cloud.");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            Topic.assign(event->topic, event->topic_len);
            Message.assign(event->data, event->data_len);

            // printf("length: %i\n", Topic.length());
            pMQTTObject->_mESP32_Vacuum->Callback(Topic, Message);
            break;
        
        default:
            ESP_LOGI(MQTT_TAG, "Default.");
            break;
    }
}

void ESP32_MQTT::Initialize(ESP32_Vacuum* esp32_vacuum) {
    _mESP32_Vacuum = esp32_vacuum;
    esp_mqtt_client_config_t cfg = {};
    cfg.broker.address.uri = "mqtt://192.168.50.10";
    cfg.broker.address.port = 1883;
    
    _mMQTTClient = esp_mqtt_client_init(&cfg);

    esp_mqtt_client_register_event(
        _mMQTTClient,
        static_cast<esp_mqtt_event_id_t>(ESP_EVENT_ANY_ID),
        _EventHandler,
        &_mMQTTClient
    );

    esp_mqtt_client_start(_mMQTTClient);

    esp_mqtt_client_subscribe(_mMQTTClient, "/vacuum/mode", 0);
    esp_mqtt_client_subscribe(_mMQTTClient, "/vacuum/command", 0);
    esp_mqtt_client_subscribe(_mMQTTClient, "/vacuum/drive", 0);
}


void ESP32_MQTT::Check() {
    printf("watchdog\n");
}