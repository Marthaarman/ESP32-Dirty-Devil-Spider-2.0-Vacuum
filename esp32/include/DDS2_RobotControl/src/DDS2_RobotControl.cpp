#include "DDS2_RobotControl.h"

static void _DDS2RobotControlTask(std::string taskName, void* taskObject) {
    if (taskName == "DDS2_Control" && taskObject != NULL) 
    {
        DDS2_RobotControl* pDDS2_RobotControl = reinterpret_cast<DDS2_RobotControl*>(taskObject);
        pDDS2_RobotControl->robotControlTask();
    }
    return;   
}

void DDS2_RobotControl::_moveForward()
{
    _mpDDS2_HAL->motorLeftRunForward();
    _mpDDS2_HAL->motorRightRunForward();
}

void DDS2_RobotControl::_moveBackward()
{
    _mpDDS2_HAL->motorLeftRunBackward();
    _mpDDS2_HAL->motorRightRunBackward();
}

void DDS2_RobotControl::_turnLeft()
{
    _mpDDS2_HAL->motorLeftRunBackward();
    _mpDDS2_HAL->motorRightRunForward();
}

void DDS2_RobotControl::_turnRight()
{
    _mpDDS2_HAL->motorLeftRunForward();
    _mpDDS2_HAL->motorRightRunBackward();
}

void DDS2_RobotControl::_stopMove()
{
    _mpDDS2_HAL->motorLeftStop();
    _mpDDS2_HAL->motorRightStop();
}

void DDS2_RobotControl::_startBrushes()
{
    _mpDDS2_HAL->motorBrushesRun();
}

void DDS2_RobotControl::_stopBrushes()
{
    _mpDDS2_HAL->motorBrushesStop();
}

void DDS2_RobotControl::_startVacuum()
{
    _mpDDS2_HAL->motorVacuumRun();
}

void DDS2_RobotControl::_stopVacuum()
{
    _mpDDS2_HAL->motorVacuumStop();
}

void DDS2_RobotControl::stop()
{
    _mDirections.clear();
    _mFunctions.clear();
    _stopMove();
    _mpDDS2_HAL->motorBrushesStop();
    _mpDDS2_HAL->motorVacuumStop();
}

void DDS2_RobotControl::startRobotControl()
{
    void* pTask = reinterpret_cast<void*>(&_DDS2RobotControlTask);
    _mpDDS2_HAL->createTask(pTask, (void*)this, _mpTaskHandleRobotControl, "DDS2_Control");
}

void DDS2_RobotControl::stopRobotControl()
{
    _stopMove();
    _stopBrushes();
    _stopVacuum();
    _mpDDS2_HAL->deleteTask(_mpTaskHandleRobotControl);
}

void DDS2_RobotControl::setDirection(DDS2RobotControlDirection direction) { 
    if( ( _mDirections.back() != direction && _mDirections.back() != DDS2RobotControlDirection::STOP) || _mDirections.size() == 0) {
        _mDirections.push_back(direction); 
    }
};

void DDS2_RobotControl::setFunction(DDS2RobotControlFunction function) { 
    if( (_mFunctions.back() != function && function != DDS2RobotControlFunction::STOP) || _mFunctions.size() == 0) {
        _mFunctions.push_back(function);
    }
};

void DDS2_RobotControl::robotControlTask()
{
    
    if (_mDirections.size() > 0) {
        DDS2RobotControlDirection direction = _mDirections.front();
        _mDirections.pop_front();
        if (direction == DDS2RobotControlDirection::FORWARD) {
            _moveForward();
        } else if (direction == DDS2RobotControlDirection::BACKWARD) {
            _moveBackward();
        } else if (direction == DDS2RobotControlDirection::LEFT) {
            _turnLeft();
        } else if (direction == DDS2RobotControlDirection::RIGHT) {
            _turnRight();
        } else {
            _stopMove();
        }
        
    }
    
    if (_mFunctions.size() > 0) {
        DDS2RobotControlFunction function = _mFunctions.front();
        _mFunctions.pop_front();
        if (function == DDS2RobotControlFunction::STARTBRUSHES) {
            _startBrushes();
        } else if (function == DDS2RobotControlFunction::STOPBRUSHES) {
            _stopBrushes();
        } else if (function == DDS2RobotControlFunction::STARTVACUUM) {
            _startVacuum();
        } else if (function == DDS2RobotControlFunction::STOPVACUUM) {
            _stopVacuum();
        } else {
            _stopBrushes();
            _stopVacuum();
        }
    }

    _mpDDS2_HAL->delay(50);
    
}