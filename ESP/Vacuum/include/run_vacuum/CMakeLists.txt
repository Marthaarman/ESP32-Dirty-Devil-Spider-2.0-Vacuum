idf_component_register(SRCS "src/run_vacuum.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES ESP32_Vacuum ESP32_MQTT ESP32_WiFi
)
