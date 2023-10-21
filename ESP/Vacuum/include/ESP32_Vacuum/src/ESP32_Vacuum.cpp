#include "ESP32_Vacuum.h"

ESP32_Vacuum::ESP32_Vacuum() {
    gpio_set_direction((gpio_num_t)_mPinLedBlue, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinLedRed, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinMotorLeftForward, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinMotorLeftReverse, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinMotorRightForward, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinMotorRightReverse, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinCleanMotors, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t)_mPinObstacle, GPIO_MODE_INPUT);
    gpio_set_direction((gpio_num_t)_mPinBattery, GPIO_MODE_INPUT);

    
    gpio_set_level((gpio_num_t)_mPinLedBlue, 0);
    gpio_set_level((gpio_num_t)_mPinLedRed, 0);
    gpio_set_level((gpio_num_t)_mPinMotorLeftForward, 0);
    gpio_set_level((gpio_num_t)_mPinMotorLeftReverse, 0);
    gpio_set_level((gpio_num_t)_mPinMotorRightForward, 0);
    gpio_set_level((gpio_num_t)_mPinMotorRightReverse, 0);
    gpio_set_level((gpio_num_t)_mPinCleanMotors, 0);
    
}


void ESP32_Vacuum::ToggleLedBlue() {
    SetLedBlue(!_mLedBlueState);
}

void ESP32_Vacuum::ToggleLedRed() {
    SetLedRed(!_mLedRedState);
}

void ESP32_Vacuum::SetLedBlue(bool onoff) {
    _mLedBlueState = onoff;
    gpio_set_level((gpio_num_t)_mPinLedBlue, _mLedBlueState ? 1 : 0); 
}

void ESP32_Vacuum::SetLedRed(bool onoff) {
    _mLedRedState = onoff;
    gpio_set_level((gpio_num_t)_mPinLedRed, _mLedRedState ? 1 : 0); 
}



void ESP32_Vacuum::Run() {
    if(_mMode == ESP32_Vacuum_Mode::AUTONOMOUS) {
        SetCleanState(ESP32_Vacuum_CleanState::OPERATE);
        if(_mTaskHandle == NULL) {
            xTaskCreate(_AutonomousRun, "ESP32_Vacuum_Task", 4096, static_cast<void*>(this), tskIDLE_PRIORITY, &_mTaskHandle);
        }
    }
    SetLedRed(0);
    SetLedBlue(0);
}

void ESP32_Vacuum::Stop() {
    SetDriveDirection(ESP32_Vacuum_DriveDirection::IDLE);
    SetCleanState(ESP32_Vacuum_CleanState::IDLE);


    if(_mTaskHandle != NULL) {
        vTaskSuspend(_mTaskHandle);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        vTaskDelete(_mTaskHandle);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        _mTaskHandle = NULL;
    }    
}

void ESP32_Vacuum::ControlMotorLeft(ESP32_Vacuum_MotorState motorState) {
    ControlMotor(ESP32_Vacuum_Motor::LEFT, motorState);
}

void ESP32_Vacuum::ControlMotorRight(ESP32_Vacuum_MotorState motorState) {
    ControlMotor(ESP32_Vacuum_Motor::RIGHT, motorState);
}

void ESP32_Vacuum::ControlMotor(ESP32_Vacuum_Motor motor, ESP32_Vacuum_MotorState motorState) {
    gpio_num_t pinForward;
    gpio_num_t pinReverse;

    if (motor == ESP32_Vacuum_Motor::LEFT) {
        pinForward = static_cast<gpio_num_t>(_mPinMotorLeftForward);
        pinReverse = static_cast<gpio_num_t>(_mPinMotorLeftReverse);
    }else {
        pinForward = static_cast<gpio_num_t>(_mPinMotorRightForward);
        pinReverse = static_cast<gpio_num_t>(_mPinMotorRightReverse);         
    }
       
    switch(motorState) {
        case ESP32_Vacuum_MotorState::FORWARD:
            // printf("drive forward\n");
            gpio_set_level(pinReverse, 0);
            gpio_set_level(pinForward, 1);
            break;
        case ESP32_Vacuum_MotorState::REVERSE:
            // printf("drive reverse\n");
            gpio_set_level(pinForward, 0);
            gpio_set_level(pinReverse, 1);
            break;
        case ESP32_Vacuum_MotorState::IDLE:
        default:
            // printf("drive stop\n");
            gpio_set_level(pinForward, 0);
            gpio_set_level(pinReverse, 0);
            break;
    } 
}

void ESP32_Vacuum::SetCleanState(ESP32_Vacuum_CleanState cleanState) {
    _mCleanState = cleanState;
    if(_mCleanState == ESP32_Vacuum_CleanState::IDLE) {
        gpio_set_level(static_cast<gpio_num_t>(_mPinCleanMotors), 0);
    }else {
        gpio_set_level(static_cast<gpio_num_t>(_mPinCleanMotors), 1);
    }
    
}

void ESP32_Vacuum::SetDriveDirection(ESP32_Vacuum_DriveDirection driveDirection) {
    if(!BatteryEmpty()) {
        if(driveDirection == ESP32_Vacuum_DriveDirection::FORWARD) {
            ControlMotorLeft(ESP32_Vacuum_MotorState::FORWARD);
            ControlMotorRight(ESP32_Vacuum_MotorState::FORWARD);
        }else if(driveDirection == ESP32_Vacuum_DriveDirection::REVERSE) {
            ControlMotorLeft(ESP32_Vacuum_MotorState::REVERSE);
            ControlMotorRight(ESP32_Vacuum_MotorState::REVERSE);
        } else if (driveDirection == ESP32_Vacuum_DriveDirection::LEFT) {
            ControlMotorLeft(ESP32_Vacuum_MotorState::IDLE);
            ControlMotorRight(ESP32_Vacuum_MotorState::FORWARD);
        } else if (driveDirection == ESP32_Vacuum_DriveDirection::RIGHT) {
            ControlMotorLeft(ESP32_Vacuum_MotorState::FORWARD);
            ControlMotorRight(ESP32_Vacuum_MotorState::IDLE);
        } else {
            ControlMotorLeft(ESP32_Vacuum_MotorState::IDLE);
            ControlMotorRight(ESP32_Vacuum_MotorState::IDLE);
        }
    }else {
        ControlMotorLeft(ESP32_Vacuum_MotorState::IDLE);
        ControlMotorRight(ESP32_Vacuum_MotorState::IDLE);
    }
}



void ESP32_Vacuum::Callback(std::string Topic, std::string Message) {
    if(Topic == "/vacuum/mode") {
        Stop();
        if(Message == "autonomous") {
            _mMode = ESP32_Vacuum_Mode::AUTONOMOUS;
        }else {
            _mMode = ESP32_Vacuum_Mode::MANUAL;
        }
    }else if (Topic == "/vacuum/command") { 
        if(Message == "run") {
            Run();
        } else if(Message == "start_clean") {
            SetCleanState(ESP32_Vacuum_CleanState::OPERATE);
        } else if(Message == "stop_clean") {
            SetCleanState(ESP32_Vacuum_CleanState::IDLE);
        } else {
            Stop();
        }
    } else if (Topic == "/vacuum/drive") {
        if(_mMode == ESP32_Vacuum_Mode::MANUAL) {
            if(Message == "forward") {
                SetDriveDirection(ESP32_Vacuum_DriveDirection::FORWARD);
            } else if(Message == "reverse") {
                SetDriveDirection(ESP32_Vacuum_DriveDirection::REVERSE);
            } else if(Message == "left") {
                SetDriveDirection(ESP32_Vacuum_DriveDirection::LEFT);
            } else if(Message == "right") {
                SetDriveDirection(ESP32_Vacuum_DriveDirection::RIGHT);
            } else {
                SetDriveDirection(ESP32_Vacuum_DriveDirection::IDLE);
            }
        }
    }else {
        Stop();
    }
}

bool ESP32_Vacuum::ObstacleDetection() {
    return gpio_get_level((gpio_num_t)_mPinObstacle) == 1 ? false : true;
}

bool ESP32_Vacuum::BatteryEmpty() {
    return gpio_get_level((gpio_num_t)_mPinBattery) == 1 ? false : true;
}

void ESP32_Vacuum::_AutonomousRun(void *pParameters) {
    ESP32_Vacuum* esp32_vacuum = static_cast<ESP32_Vacuum*>(pParameters); 

    uint16_t taskExecutionSetTime = 50, taskExecutionRunTime = 0;
    ESP32_Vacuum_DriveDirection task = ESP32_Vacuum_DriveDirection::FORWARD;

    while(true) {
        if(esp32_vacuum->ObstacleDetection()) {
            taskExecutionRunTime = 0;
            taskExecutionSetTime = rand()%(20-10 + 1) + 10;
            task = ESP32_Vacuum_DriveDirection::REVERSE;
            printf("Obstacle!\n");
        }else if(esp32_vacuum->BatteryEmpty()) {
            task = ESP32_Vacuum_DriveDirection::IDLE;
            printf("Empty battery!\n");
            esp32_vacuum->SetLedRed(true);
            esp32_vacuum->SetLedBlue(true);
            esp32_vacuum->Stop();
            
        
        }else if(taskExecutionRunTime == taskExecutionSetTime) {
            if(task == ESP32_Vacuum_DriveDirection::FORWARD) {
                task = ESP32_Vacuum_DriveDirection::RIGHT;
                taskExecutionRunTime = 0;
                taskExecutionSetTime = rand()%(30-10 + 1) + 10;
            } else if(task == ESP32_Vacuum_DriveDirection::REVERSE) {
                if(rand() > (RAND_MAX / 2)) {
                    task = ESP32_Vacuum_DriveDirection::LEFT;
                }else{
                    task = ESP32_Vacuum_DriveDirection::RIGHT;
                }
                taskExecutionRunTime = 0;
                taskExecutionSetTime = rand()%(30-10 + 1) + 10;
            } else if(task == ESP32_Vacuum_DriveDirection::LEFT) {
                task = ESP32_Vacuum_DriveDirection::FORWARD;
                taskExecutionRunTime = 0;
                taskExecutionSetTime = rand()%(80-40 + 1) + 40;
            } else if(task == ESP32_Vacuum_DriveDirection::RIGHT) {
                task = ESP32_Vacuum_DriveDirection::FORWARD;
                taskExecutionRunTime = 0;
                taskExecutionSetTime = rand()%(80-40 + 1) + 40;
            } else {
                esp32_vacuum->Stop();
            }
        } else {
            esp32_vacuum->ToggleLedBlue();
            taskExecutionRunTime++;
        }
        esp32_vacuum->SetDriveDirection(task);
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}