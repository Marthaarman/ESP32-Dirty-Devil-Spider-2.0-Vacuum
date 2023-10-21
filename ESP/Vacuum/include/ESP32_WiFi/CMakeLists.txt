idf_component_register(SRCS "src/ESP32_WiFi.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES esp_wifi nvs_flash
)
