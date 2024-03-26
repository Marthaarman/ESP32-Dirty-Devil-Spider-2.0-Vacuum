#ifndef _DDS2_Robot_H_
#define _DDS2_Robot_H_

#include "DDS2_HAL.h"
#include <list>

enum class DDS2RobotControlDirection {
    STOP = 0,
    FORWARD = 1,
    BACKWARD = 2,
    LEFT = 3,
    RIGHT = 4
};

enum class DDS2RobotControlFunction {
    STARTBRUSHES = 0,
    STOPBRUSHES = 1,
    STARTVACUUM = 2,
    STOPVACUUM = 3,
    STOP = 4
};

class DDS2_RobotControl
{
    private:
        DDS2_HAL *_mpDDS2_HAL;

        void* _mpTaskHandleRobotControl = NULL;

        std::list<DDS2RobotControlDirection> _mDirections = {DDS2RobotControlDirection::STOP};
        std::list<DDS2RobotControlFunction> _mFunctions = {DDS2RobotControlFunction::STOP};

        void _moveForward();
        void _moveBackward();
        void _turnLeft();
        void _turnRight();

        void _stopMove();

        void _startBrushes();
        void _stopBrushes();

        void _startVacuum();
        void _stopVacuum();

    public:
        DDS2_RobotControl(DDS2_HAL *pDDS2_HAL) { _mpDDS2_HAL = pDDS2_HAL; }

        void setDirection(DDS2RobotControlDirection direction);
        void setFunction(DDS2RobotControlFunction function);

        void stop();

        void startRobotControl();
        void stopRobotControl();
        void robotControlTask();

        void moveForward() {_moveForward();}
        void moveBackward() {_moveBackward();}
        void moveLeft() {_turnLeft();}
        void moveRight() {_turnRight();}
        void moveStop() {_stopMove();}

        void startClean() {_startBrushes(); _startVacuum();}

};

#endif // _DDS2_Robot_H_