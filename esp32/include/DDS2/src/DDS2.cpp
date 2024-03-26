#include "DDS2.h"
#include <algorithm>

static void _DDS2Task(std::string taskName, void* taskObject) {

    if (taskName == "DDS2_Manual" && taskObject != NULL) 
    {
        DDS2* pDDS2 = reinterpret_cast<DDS2*>(taskObject);
        pDDS2->manualTask();
    }

    if (taskName == "DDS2_Auto" && taskObject != NULL) 
    {
        DDS2* pDDS2 = reinterpret_cast<DDS2*>(taskObject);
        pDDS2->autoTask();
    }

    if (taskName == "DDS2_Comm" && taskObject != NULL) 
    {
        DDS2* pDDS2 = reinterpret_cast<DDS2*>(taskObject);
        pDDS2->communicationTask();
    }



    return;   
}

DDS2::DDS2(DDS2_HAL* pDDS2_HAL) { 
    _mpDDS2_HAL = pDDS2_HAL; 
}

void DDS2::init()
{
    _mpDDS2_HAL->init();
    _mpDDS2_RobotControl = new DDS2_RobotControl(_mpDDS2_HAL);
    _mpDDS2_Sensors = new DDS2_Sensors(_mpDDS2_HAL);
}

void DDS2::start(){
    _mMode = DDS2Mode::AUTO;

    _mpDDS2_RobotControl->startRobotControl();
    _mpDDS2_Sensors->startSensorTask();

    _mpDDS2_HAL->delay(1000);
    if(_mMode == DDS2Mode::AUTO) {
        printf("Starting Auto Mode\n");
        _startAutoTask();
    }
}

void DDS2::stop() {
    _mMode = DDS2Mode::IDLE;

    _mpDDS2_RobotControl->stopRobotControl();
    _mpDDS2_Sensors->stopSensorTask();

    _mpDDS2_HAL->delay(1000);
    _stopAutoTask();
    _stopManualTask();
    _stopCommunicationTask();
}

void DDS2::_startCommunicationTask() {
    void* pTask = reinterpret_cast<void*>(&_DDS2Task);
    _mpDDS2_HAL->createTask(pTask, (void*)this, _mpTaskHandleCommunication, "DDS2_Comm");
}

void DDS2::_stopCommunicationTask() {
    _mpDDS2_HAL->deleteTask(_mpTaskHandleCommunication);
}

void DDS2::_startManualTask() {
    void* pTask = reinterpret_cast<void*>(&_DDS2Task);
    _mpDDS2_HAL->createTask(pTask, (void*)this, _mpTaskHandleManual, "DDS2_Manual");
}

void DDS2::_stopManualTask() {
    _mpDDS2_HAL->deleteTask(_mpTaskHandleManual);
    _mpDDS2_RobotControl->setFunction(DDS2RobotControlFunction::STOPBRUSHES);
    _mpDDS2_RobotControl->setFunction(DDS2RobotControlFunction::STOPVACUUM);
    _mpDDS2_RobotControl->setDirection(DDS2RobotControlDirection::STOP);
}

void DDS2::_startAutoTask() {
    _mpDDS2_RobotControl->setFunction(DDS2RobotControlFunction::STARTBRUSHES);
    _mpDDS2_RobotControl->setFunction(DDS2RobotControlFunction::STARTVACUUM);
    _mpDDS2_HAL->ledRedOff();
    _mpDDS2_HAL->ledBlueOn();

    void* pTask = reinterpret_cast<void*>(&_DDS2Task);
    _mpDDS2_HAL->createTask(pTask, (void*)this, _mpTaskHandleAuto, "DDS2_Auto");
}

void DDS2::_stopAutoTask() {
    _mMode = DDS2Mode::IDLE;
    _mpDDS2_HAL->deleteTask(_mpTaskHandleAuto);
    _mpDDS2_HAL->delay(50);
    _mpDDS2_RobotControl->stop();
    _mpDDS2_HAL->ledBlueOff();
}

void DDS2::_batteryStop() {
    _mpDDS2_HAL->ledRedOn();
    _stopAutoTask();  
}

void DDS2::autoTask() {
    if(_mMode == DDS2Mode::MANUAL) {
        _stopAutoTask();
        _startManualTask();
        return;
    }

    if(_mMode == DDS2Mode::IDLE) {
        _mpDDS2_HAL->delay(500);
        return;
    }

    DDS2SensorWarning sensorWarning = _mpDDS2_Sensors->getSensorWarning();
    if(sensorWarning != DDS2SensorWarning::NONE) {
        DDS2RobotControlDirection randDirection = DDS2RobotControlDirection::LEFT;
        uint16_t randDelay = rand() % (2000 - 250 + 1) + 250;
            if(rand() > (RAND_MAX / 2)) {
            randDirection = DDS2RobotControlDirection::RIGHT;
        }

        switch(sensorWarning) {
            case DDS2SensorWarning::BATTERY:
                _batteryStop();
                return;
                break;
            case DDS2SensorWarning::BUMP:
                _mpDDS2_RobotControl->setDirection(DDS2RobotControlDirection::BACKWARD);
                _mpDDS2_HAL->delay(2000);
                _mpDDS2_RobotControl->setDirection(randDirection);
                _mpDDS2_HAL->delay(randDelay);
                _mpDDS2_RobotControl->setDirection(DDS2RobotControlDirection::FORWARD);
                break;
            case DDS2SensorWarning::CLIFF:
                _mpDDS2_RobotControl->setDirection(DDS2RobotControlDirection::BACKWARD);
                _mpDDS2_HAL->delay(2000);
                _mpDDS2_RobotControl->setDirection(randDirection);
                _mpDDS2_HAL->delay(randDelay);
                _mpDDS2_RobotControl->setDirection(DDS2RobotControlDirection::FORWARD);
                break;
            default:
                _mpDDS2_RobotControl->setDirection(DDS2RobotControlDirection::FORWARD);
                break;
        }
        _mpDDS2_Sensors->popSensorWarning();
    } else {
        _mpDDS2_RobotControl->setDirection(DDS2RobotControlDirection::FORWARD);
    }
    
    _mpDDS2_HAL->delay(50);
}

void DDS2::manualTask() {


        if(_mMode == DDS2Mode::AUTO) {
            _stopManualTask();
            _startAutoTask();
            return;
        }
       
        _mpDDS2_HAL->delay(50);
    
}

void DDS2::communicationTask() {
        _mpDDS2_HAL->delay(50);
    
}






