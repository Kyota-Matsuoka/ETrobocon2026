#ifndef ETTR_UNIT_DISTANCEMETER_H_
#define ETTR_UNIT_DISTANCEMETER_H_

#include "Motor.h"

class DistanceMeter {
    public:
        DistanceMeter(spikeapi::Motor& leftWheel,
                                spikeapi::Motor& rightWheel);
        float calDistance();
        float calDistance_currentState();
        void  calDistance_currentStateReset();

    private:
        spikeapi::Motor& mLeftWheel;
        spikeapi::Motor& mRightWheel;
        float preLeftAngle;
        float preRightAngle;
        float WheelDistance_currentState;
        float preDistance;
        float currentDistance;
        float DiffLeftAngle;
        float DiffRightAngle;
        float WheelDistance;
};

#endif //ETTR_UNIT_DISTANCEMETOR_H_