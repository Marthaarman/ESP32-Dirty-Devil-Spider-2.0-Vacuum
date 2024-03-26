#ifndef _DDS2_HAL_H_
#define _DDS2_HAL_H_
#include <string>

class DDS2_HAL {
    private:

    public:
        DDS2_HAL() {};
        ~DDS2_HAL() {};

        virtual void init() {};

        virtual void motorLeftRunForward() {};
        virtual void motorLeftRunBackward() {};
        virtual void motorLeftStop() {};
        virtual void motorRightRunForward() {};
        virtual void motorRightRunBackward() {};
        virtual void motorRightStop() {};

        virtual void motorVacuumRun() {};
        virtual void motorVacuumStop() {};

        virtual void motorBrushesRun() {};
        virtual void motorBrushesStop() {};

        virtual bool sensorBump() { return true; };
        virtual bool sensorCliff() { return true; };
        virtual bool sensorBattery() { return false; };

        virtual void ledBlueOn() {};
        virtual void ledBlueOff() {};

        virtual void ledRedOn() {};
        virtual void ledRedOff() {};

        virtual void delay(int ms) {};

        virtual void createTask(void* taskFunction, void* taskCreator, void* &taskHandle, std::string taskName) { return; };
        virtual void deleteTask(void* &taskHandle) { return; };


};

#endif //_DDS2_HAL_H_