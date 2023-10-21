#ifndef _ESP32_WIFI_H_
#define _ESP32_WIFI_H_

#include "esp_event.h"
#include <string>

class ESP32_WiFi {

    private:
        std::string _mWiFiSSID = "Mart";
        std::string _mWiFiPass = "n4#UW*N0#YVkZ7";
        
        bool _mConnectedState = false;

        void _Prepare();
        void _Configure();
        void _RegisterEventHandlers();

    public:
        void Initialize();
        void Start();
        bool Connected();
        static void EventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
};

#endif //_ESP32_WIFI_H_