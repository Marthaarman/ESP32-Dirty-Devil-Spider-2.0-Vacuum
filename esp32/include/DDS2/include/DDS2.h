#ifndef __DDS2_H__
#define __DDS2_H__

#include <stdio.h>
#include "DDS2_HAL.h"
#include "DDS2_RobotControl.h"
#include "DDS2_Sensors.h"


enum class DDS2Mode {
    IDLE = 0,
    MANUAL = 1,
    AUTO = 2
};

class DDS2
{
    private:
        DDS2_HAL* _mpDDS2_HAL;
        DDS2_RobotControl* _mpDDS2_RobotControl;
        DDS2_Sensors* _mpDDS2_Sensors;

        void* _mpTaskHandleManual = NULL;
        void* _mpTaskHandleAuto = nullptr;
        void* _mpTaskHandleCommunication = NULL;
        void* _mpTaskHandleControl = NULL;
        bool _mRunning = false;
       


        void _startCommunicationTask();
        void _stopCommunicationTask();

        void _startManualTask();
        void _stopManualTask();

        void _startAutoTask();
        void _stopAutoTask();

        void _batteryStop();
       
        DDS2Mode _mMode = DDS2Mode::IDLE;
        

    public:
        DDS2(DDS2_HAL* pDDS2_HAL);
        ~DDS2() { _mpDDS2_HAL = NULL; }

        void init();
        void start();
        void stop();

        void manualTask();
        void autoTask();
        void communicationTask();
};


#endif //__DDS2_H__