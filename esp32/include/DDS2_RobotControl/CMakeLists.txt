#TODO: make independent of ESP32
idf_component_register(SRCS "src/DDS2_RobotControl.cpp"
    INCLUDE_DIRS "include/"
    REQUIRES DDS2_HAL
)

