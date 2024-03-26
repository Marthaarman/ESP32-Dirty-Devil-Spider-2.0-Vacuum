#TODO: make independent of ESP32
idf_component_register(SRCS "src/DDS2.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES DDS2_HAL DDS2_RobotControl DDS2_Sensors
)
