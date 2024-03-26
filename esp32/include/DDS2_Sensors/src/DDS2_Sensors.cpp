#include "DDS2_Sensors.h"
#include <algorithm>

static void _DDS2SensorTask(std::string taskName, void* taskObject) {
    if (taskName == "DDS2_Sensors" && taskObject != NULL) {
        DDS2_Sensors* pDDS2_Sensors = reinterpret_cast<DDS2_Sensors*>(taskObject);
        pDDS2_Sensors->sensorTask();
    }
    return;
}

void DDS2_Sensors::startSensorTask() {
    void* pTask = reinterpret_cast<void*>(&_DDS2SensorTask);
    _mpDDS2_HAL->createTask(pTask, (void*)this, _mpTaskHandle, "DDS2_Sensors");
}

void DDS2_Sensors::stopSensorTask() {
    _mpDDS2_HAL->deleteTask(_mpTaskHandle);
}

DDS2SensorWarning DDS2_Sensors::getSensorWarning() {
    if(_mSensorWarnings.size() > 0) {
        DDS2SensorWarning warning = _mSensorWarnings.front();
        return warning;
    }
    return DDS2SensorWarning::NONE;
}

void DDS2_Sensors::popSensorWarning() {
    if(_mSensorWarnings.size() > 0) {
        _mSensorWarnings.pop_front();
    }
}

void DDS2_Sensors::sensorTask() {

        bool sensorBattery = _mpDDS2_HAL->sensorBattery();
        if(
            sensorBattery == true &&
            std::find(_mSensorWarnings.begin(), _mSensorWarnings.end(), DDS2SensorWarning::BATTERY) == _mSensorWarnings.end()
        ) 
        {
            _mBatteryWarningCount++;
            if(_mBatteryWarningCount > 9) {
                _mSensorWarnings.push_back(DDS2SensorWarning::BATTERY);
                _mBatteryWarningCount = 0;
            }
        }else if(!sensorBattery) {
            _mBatteryWarningCount = 0;
        }

        if(
            _mpDDS2_HAL->sensorBump() == true && 
            std::find(_mSensorWarnings.begin(), _mSensorWarnings.end(), DDS2SensorWarning::BUMP) == _mSensorWarnings.end()
        ){
            _mSensorWarnings.push_back(DDS2SensorWarning::BUMP);
        }

        if(
            _mpDDS2_HAL->sensorCliff() == true && 
            std::find(_mSensorWarnings.begin(), _mSensorWarnings.end(), DDS2SensorWarning::CLIFF) == _mSensorWarnings.end()
        ) {
            _mSensorWarnings.push_back(DDS2SensorWarning::CLIFF);
        }
        _mpDDS2_HAL->delay(100);

}