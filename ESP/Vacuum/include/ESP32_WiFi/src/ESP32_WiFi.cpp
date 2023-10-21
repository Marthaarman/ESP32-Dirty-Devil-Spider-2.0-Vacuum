#include "ESP32_WiFi.h"

#include <string.h>
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"


const char* WIFI_TAG = "ESP32_WIFI";


void ESP32_WiFi::EventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP32_WiFi* pESP32_WiFi = static_cast<ESP32_WiFi*>(arg);
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        pESP32_WiFi->_mConnectedState = false;
        esp_wifi_connect();
        ESP_LOGI(WIFI_TAG, "Trying to (re)connect to the AP.");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        pESP32_WiFi->_mConnectedState = true;
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        ESP_LOGI(WIFI_TAG, "Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}


void ESP32_WiFi::Initialize()
{
    _Prepare();
    _Configure();
    _RegisterEventHandlers();
}

void ESP32_WiFi::Start()
{
    ESP_ERROR_CHECK(esp_wifi_start());
}

void ESP32_WiFi::_Prepare()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
}

void ESP32_WiFi::_Configure()
{
    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifiInitConfig));

    wifi_config_t wifiConfig = { };
    strcpy((char*)wifiConfig.sta.ssid, _mWiFiSSID.c_str());
    strcpy((char*)wifiConfig.sta.password, _mWiFiPass.c_str());
    wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifiConfig.sta.pmf_cfg.capable = true;
    wifiConfig.sta.pmf_cfg.required = false;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(static_cast<wifi_interface_t>(ESP_IF_WIFI_STA), &wifiConfig));
}

void ESP32_WiFi::_RegisterEventHandlers()
{
    esp_event_handler_instance_t instanceAnyId;
    esp_event_handler_instance_t instanceGotIp;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &EventHandler,
            this,
            &instanceAnyId
        )
    );

    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &EventHandler,
            this,
            &instanceGotIp
        )
    );
}

bool ESP32_WiFi::Connected() {
    return _mConnectedState;
}
