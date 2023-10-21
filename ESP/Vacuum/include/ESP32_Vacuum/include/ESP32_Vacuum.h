#ifndef __ESP32_VACUUM_H__
#define __ESP32_VACUUM_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdio.h>
#include <inttypes.h>
#include <string>
#include <cstring>
// #include "esp_system.h"


// typedef uint8_t ESP32_Vacuum_MotorState;
// ESP32_Vacuum_MotorState IDLE = 0;
// ESP32_Vacuum_MotorState FORWARD = 1;
// ESP32_Vacuum_MotorState REVERSE = 2;

// struct ESP32_Vacuum_MotorState {
//     uint8_t IDLE;
//     uint8_t FORWARD;
//     uint8_t REVERSE;
// };

enum class ESP32_Vacuum_MotorState {
    IDLE = 00,
    FORWARD = 01,
    REVERSE = 02
};

enum class ESP32_Vacuum_Motor {
    LEFT = 10,
    RIGHT = 11
};

enum class ESP32_Vacuum_Mode {
    AUTONOMOUS = 20,
    MANUAL = 21
};

enum class ESP32_Vacuum_CleanState {
    IDLE = 30,
    OPERATE = 31
};

enum class ESP32_Vacuum_DriveDirection {
    IDLE = 40,
    FORWARD = 41,
    REVERSE = 42,
    LEFT = 43,
    RIGHT= 44
};

class ESP32_Vacuum {
    private:
        bool _mLedBlueState = true;
        bool _mLedRedState = false;

        ESP32_Vacuum_MotorState _mMotorStateRight = ESP32_Vacuum_MotorState::IDLE;
        ESP32_Vacuum_MotorState _mMotorStateLeft = ESP32_Vacuum_MotorState::IDLE;
        ESP32_Vacuum_Mode _mMode = ESP32_Vacuum_Mode::MANUAL;
        ESP32_Vacuum_CleanState _mCleanState = ESP32_Vacuum_CleanState::IDLE;

        TaskHandle_t _mTaskHandle = NULL;

        const uint8_t _mPinLedRed = (uint8_t) GPIO_NUM_26;
        const uint8_t _mPinLedBlue = (uint8_t) GPIO_NUM_25;

        const uint8_t _mPinMotorRightForward = (uint8_t) GPIO_NUM_32;
        const uint8_t _mPinMotorRightReverse = (uint8_t) GPIO_NUM_33;
        const uint8_t _mPinMotorLeftForward = (uint8_t) GPIO_NUM_22;
        const uint8_t _mPinMotorLeftReverse = (uint8_t) GPIO_NUM_21;

        const uint8_t _mPinCleanMotors = (uint8_t) GPIO_NUM_19;

        const uint8_t _mPinObstacle = (uint8_t) GPIO_NUM_18;

        const uint8_t _mPinBattery = (uint8_t) GPIO_NUM_27;

        // ESP32_WiFi *wifi = new ESP32_WiFi();

        static void _AutonomousRun( void * pParameters );

    public:
    ESP32_Vacuum();
    void ToggleLedBlue();
    void SetLedBlue(bool onoff);
    void ToggleLedRed();
    void SetLedRed(bool onoff);

    void ControlMotorLeft(ESP32_Vacuum_MotorState motorState);
    void ControlMotorRight(ESP32_Vacuum_MotorState motorState);
    void ControlMotor(ESP32_Vacuum_Motor motor, ESP32_Vacuum_MotorState motorState);

    void Callback(std::string Topic, std::string Message);

    void SetCleanState(ESP32_Vacuum_CleanState cleanState);
    void SetDriveDirection(ESP32_Vacuum_DriveDirection driveDirection);
    void Run();
    void Stop();

    bool ObstacleDetection();
    bool BatteryEmpty();

};

#endif //__ESP32_VACUUM_H__