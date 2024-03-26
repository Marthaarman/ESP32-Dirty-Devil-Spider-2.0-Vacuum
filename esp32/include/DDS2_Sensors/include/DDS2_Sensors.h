#ifndef _DDS2_Sensors_H_
#define _DDS2_Sensors_H_

#include "DDS2_HAL.h"
#include <list>


enum class DDS2SensorWarning {
    NONE = 0,
    BUMP = 1,
    CLIFF = 2,
    BATTERY = 3
};


class DDS2_Sensors
{
    private:
        DDS2_HAL *_mpDDS2_HAL;

        void* _mpTaskHandle = NULL;
        std::list<DDS2SensorWarning> _mSensorWarnings;

        uint8_t _mBatteryWarningCount = 0;

    public:
        DDS2_Sensors(DDS2_HAL *pDDS2_HAL) { _mpDDS2_HAL = pDDS2_HAL; }
        ~DDS2_Sensors() { _mpDDS2_HAL = NULL; }

        void startSensorTask();
        void stopSensorTask();
        void sensorTask();
        
        DDS2SensorWarning getSensorWarning();
        void popSensorWarning();

};

#endif //_DDS2_Sensors_H_