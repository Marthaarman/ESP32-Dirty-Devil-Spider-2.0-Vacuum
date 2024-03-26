
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

#include "driver/gpio.h"
#include "DDS2_ESP32_HAL.h"



static void taskProcessing(void* pParam) {
    if (pParam == nullptr) {
        return;
    }
    DDS2_ESP32_HAL* pDDS2_ESP32_HAL = reinterpret_cast<DDS2_ESP32_HAL*>(pParam);
    std::string tName = (std::string)pcTaskGetName(xTaskGetCurrentTaskHandle() );
    std::string cName;

    std::tuple<std::string, void*, void*, bool> taskInfo = pDDS2_ESP32_HAL->getTaskInfo(tName.c_str());

    if (std::get<1>(taskInfo) == nullptr) {
        return;
    }

    void (*func)(std::string, void*) = reinterpret_cast<void (*)(std::string, void*)>(std::get<1>(taskInfo));
    while(!pDDS2_ESP32_HAL->taskStopFlag(tName.c_str())) {
        func(tName, std::get<2>(taskInfo));
    }

    vTaskDelete(NULL);
}

void DDS2_ESP32_HAL::init() {
    _resetPins();
    delay(100);
    _setPins();
}

void DDS2_ESP32_HAL::_setPins() {
    gpio_set_direction((gpio_num_t)_mPinMotorLeftRunForward, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinMotorLeftRunBackward, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinMotorRightRunForward, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinMotorRightRunBackward, GPIO_MODE_OUTPUT);

    gpio_set_direction((gpio_num_t)_mPinMotorCleanFunctions, GPIO_MODE_OUTPUT);

    gpio_set_direction((gpio_num_t)_mPinSensorObstacle, GPIO_MODE_INPUT);
    gpio_set_direction((gpio_num_t)_mPinSensorBattery, GPIO_MODE_INPUT);

    gpio_set_direction((gpio_num_t)_mPinLedRed, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinLedBlue, GPIO_MODE_OUTPUT);

}

void DDS2_ESP32_HAL::_resetPins() {
    gpio_reset_pin((gpio_num_t)_mPinMotorLeftRunForward);
    gpio_reset_pin((gpio_num_t)_mPinMotorLeftRunBackward);
    gpio_reset_pin((gpio_num_t)_mPinMotorRightRunForward);
    gpio_reset_pin((gpio_num_t)_mPinMotorRightRunBackward);

    gpio_reset_pin((gpio_num_t)_mPinMotorCleanFunctions);

    // gpio_reset_pin((gpio_num_t)_mPinSensorObstacle);
    // gpio_reset_pin((gpio_num_t)_mPinSensorBattery);

    gpio_reset_pin((gpio_num_t)_mPinLedRed);
    gpio_reset_pin((gpio_num_t)_mPinLedBlue);

}

std::tuple<std::string, void*, void*, bool> DDS2_ESP32_HAL::getTaskInfo(const char* taskName) {
    std::list<std::tuple<std::string, void*, void*, bool>>::iterator it;
    for (it = _mTaskList.begin(); it != _mTaskList.end(); ++it){
        std::tuple<std::string, void*, void*, bool> task = *it;
        if (std::get<0>(task) == taskName) {
            return task;
        }
    }
    return std::make_tuple("", nullptr, nullptr, true);
}

bool DDS2_ESP32_HAL::taskStopFlag(const char* taskName) {
    std::list<std::tuple<std::string, void*, void*, bool>>::iterator it;
    for (it = _mTaskList.begin(); it != _mTaskList.end(); ++it){
        std::tuple<std::string, void*, void*, bool> task = *it;
        if (std::get<0>(task) == taskName) {
            return std::get<3>(task);
        }
    }
    return true;
}

void DDS2_ESP32_HAL::setTaskStopFlag(const char* taskName, bool set) {
std::list<std::tuple<std::string, void*, void*, bool>>::iterator it;
    for (it = _mTaskList.begin(); it != _mTaskList.end(); ++it){
        // std::tuple<std::string, void*, void*, bool> task = *it;
        if (std::get<0>(*it) == taskName) {
            std::get<3>(*it) = set;
            return;
        }
    }
    return;
}

void DDS2_ESP32_HAL::motorLeftRunForward() {
    gpio_set_level((gpio_num_t)_mPinMotorLeftRunForward, 1);
    gpio_set_level((gpio_num_t)_mPinMotorLeftRunBackward, 0);
}

void DDS2_ESP32_HAL::motorLeftRunBackward() {
    gpio_set_level((gpio_num_t)_mPinMotorLeftRunForward, 0);
    gpio_set_level((gpio_num_t)_mPinMotorLeftRunBackward, 1);
}

void DDS2_ESP32_HAL::motorLeftStop() {
    gpio_set_level((gpio_num_t)_mPinMotorLeftRunForward, 0);
    gpio_set_level((gpio_num_t)_mPinMotorLeftRunBackward, 0);
}

void DDS2_ESP32_HAL::motorRightRunForward() {
    gpio_set_level((gpio_num_t)_mPinMotorRightRunForward, 1);
    gpio_set_level((gpio_num_t)_mPinMotorRightRunBackward, 0);
}

void DDS2_ESP32_HAL::motorRightRunBackward() {
    gpio_set_level((gpio_num_t)_mPinMotorRightRunForward, 0);
    gpio_set_level((gpio_num_t)_mPinMotorRightRunBackward, 1);
}

void DDS2_ESP32_HAL::motorRightStop() {
    gpio_set_level((gpio_num_t)_mPinMotorRightRunForward, 0);
    gpio_set_level((gpio_num_t)_mPinMotorRightRunBackward, 0);
}

void DDS2_ESP32_HAL::motorCleanFunctionsRun() {
    gpio_set_level((gpio_num_t)_mPinMotorCleanFunctions, 1);
}

void DDS2_ESP32_HAL::motorCleanFunctionsStop() {
    gpio_set_level((gpio_num_t)_mPinMotorCleanFunctions, 0);
}

bool DDS2_ESP32_HAL::sensorObstacle() {
    if(gpio_get_level((gpio_num_t)_mPinSensorObstacle) > 0) {
        return false;
    } 
    return true;
}

bool DDS2_ESP32_HAL::sensorBattery() {
    if(gpio_get_level((gpio_num_t)_mPinSensorBattery) > 0) {
        return false;
    }else {
        return true;
    }
}

void DDS2_ESP32_HAL::ledRedOn() {
    gpio_set_level((gpio_num_t)_mPinLedRed, 1);
}

void DDS2_ESP32_HAL::ledRedOff() {
    gpio_set_level((gpio_num_t)_mPinLedRed, 0);
}

void DDS2_ESP32_HAL::ledBlueOn() {
    gpio_set_level((gpio_num_t)_mPinLedBlue, 1);
}

void DDS2_ESP32_HAL::ledBlueOff() {
    gpio_set_level((gpio_num_t)_mPinLedBlue, 0);
}

void DDS2_ESP32_HAL::delay(int ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void DDS2_ESP32_HAL::createTask(void* taskFunction, void* taskObject, void* &taskHandle, std::string taskName) {
    taskHandle = (void*)new TaskHandle_t();

    std::tuple<std::string, void*, void*, bool>taskInfo = std::make_tuple(taskName, taskFunction, taskObject, false);
    _mTaskList.push_back(taskInfo);

    printf("Task created: %s\n", taskName.c_str());
    xTaskCreate((TaskFunction_t)taskProcessing, taskName.c_str(), configMINIMAL_STACK_SIZE, (void*)this, tskIDLE_PRIORITY, (TaskHandle_t*)taskHandle); 
}

void DDS2_ESP32_HAL::deleteTask(void* &pTaskHandle) {
    if(pTaskHandle == nullptr) { return; }

    TaskHandle_t taskHandle = *((TaskHandle_t*)pTaskHandle);
    const char* taskName = pcTaskGetName(taskHandle);
    setTaskStopFlag(taskName, true);
    _mTaskRunning = true;
}