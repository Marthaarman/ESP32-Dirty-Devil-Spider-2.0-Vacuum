#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "DDS2_ESP32.h"
#include "DDS2_ESP32_HAL.h"
#include "DDS2.h"
#include "driver/gpio.h"

#define TAG "DDS2_ESP32"

#define pinMotorLeftRunForward GPIO_NUM_0
#define pinMotorLeftRunBackward GPIO_NUM_1
#define pinMotorRightRunForward GPIO_NUM_2
#define pinMotorRightRunBackward GPIO_NUM_3
#define pinMotorCleanFunctions GPIO_NUM_4
#define pinSensorObstacle GPIO_NUM_5
#define pinSensorBattery GPIO_NUM_6
#define pinSensorLid GPIO_NUM_7
#define pinLedRed GPIO_NUM_8
#define pinLedBlue GPIO_NUM_9
#define pinSound GPIO_NUM_10


extern "C" {

    void DDS2_ESP32(void) {
        DDS2_ESP32_HAL* pDDS2_ESP32_HAL = new DDS2_ESP32_HAL();
        DDS2* pDDS2 = new DDS2(pDDS2_ESP32_HAL);

        pDDS2_ESP32_HAL->setPinMotorLeftRunForward(pinMotorLeftRunForward);
        pDDS2_ESP32_HAL->setPinMotorLeftRunBackward(pinMotorLeftRunBackward);
        pDDS2_ESP32_HAL->setPinMotorRightRunForward(pinMotorRightRunForward);
        pDDS2_ESP32_HAL->setPinMotorRightRunBackward(pinMotorRightRunBackward);
        pDDS2_ESP32_HAL->setPinMotorCleanFunctions(pinMotorCleanFunctions);
        pDDS2_ESP32_HAL->setPinSensorObstacle(pinSensorObstacle);
        pDDS2_ESP32_HAL->setPinSensorBattery(pinSensorBattery);
        pDDS2_ESP32_HAL->setPinLedRed(pinLedRed);
        pDDS2_ESP32_HAL->setPinLedBlue(pinLedBlue);


        pDDS2->init();
        pDDS2->start();

        while(true) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}