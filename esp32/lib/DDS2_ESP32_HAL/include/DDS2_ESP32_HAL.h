
#ifndef _DDS2_ESP32_HAL_H_
#define _DDS2_ESP32_HAL_H_

#include <stdint.h>
#include <string>
#include <list>
#include <utility>
#include <tuple>

#include "DDS2_HAL.h"

class DDS2_ESP32_HAL : public DDS2_HAL
{
    private:
        uint8_t _mPinMotorLeftRunForward;
        uint8_t _mPinMotorLeftRunBackward;
        uint8_t _mPinMotorRightRunForward;
        uint8_t _mPinMotorRightRunBackward;
        uint8_t _mPinMotorCleanFunctions;
        uint8_t _mPinSensorObstacle;
        uint8_t _mPinSensorBattery;
        uint8_t _mPinLedRed;
        uint8_t _mPinLedBlue;

        bool _mTaskRunning = true;

        // name, task, object, taskStopFlag
        std::list<std::tuple<std::string, void*, void*, bool>> _mTaskList;

        void _setPins();
        void _resetPins();

    public:
        DDS2_ESP32_HAL() {};
        ~DDS2_ESP32_HAL() {};

        void init() override;

        //  taskName, taskFunction, taskObject, taskStopFlag
        std::tuple<std::string, void*, void*, bool> getTaskInfo(const char* taskName);
        bool taskStopFlag(const char* taskName);
        void setTaskStopFlag(const char* taskName, bool set);

        void setPinMotorLeftRunForward(uint8_t pin) { _mPinMotorLeftRunForward = pin; }
        void setPinMotorLeftRunBackward(uint8_t pin) { _mPinMotorLeftRunBackward = pin;}
        void setPinMotorRightRunForward(uint8_t pin) { _mPinMotorRightRunForward = pin; }
        void setPinMotorRightRunBackward(uint8_t pin) { _mPinMotorRightRunBackward = pin; }
        void setPinMotorCleanFunctions(uint8_t pin) { _mPinMotorCleanFunctions = pin; }
        void setPinSensorObstacle(uint8_t pin) { _mPinSensorObstacle = pin; }
        void setPinSensorBattery(uint8_t pin) { _mPinSensorBattery = pin; }
        void setPinLedRed(uint8_t pin) { _mPinLedRed = pin; }
        void setPinLedBlue(uint8_t pin) { _mPinLedBlue = pin; }

        void motorLeftRunForward() override;
        void motorLeftRunBackward() override;
        void motorLeftStop() override;

        void motorRightRunForward() override;
        void motorRightRunBackward() override;
        void motorRightStop() override;

        void motorCleanFunctionsRun();
        void motorCleanFunctionsStop();
        void motorVacuumRun() override { motorCleanFunctionsRun(); };
        void motorVacuumStop() override { motorCleanFunctionsStop(); };

        void motorBrushesRun() override {};
        void motorBrushesStop() override {};

        bool sensorObstacle();
        bool sensorBump() override { return sensorObstacle(); };
        bool sensorCliff() override { return false; };

        bool sensorBattery() override;

        void ledRedOn() override;
        void ledRedOff() override;
        void ledBlueOn() override;
        void ledBlueOff() override;

        void delay(int ms) override;
        void createTask(void* taskF, void* taskCreator, void* &taskHandle, std::string taskName) override;
        void deleteTask(void* &taskHandle) override;

};

#endif //_DDS2_ESP32_HAL_H_