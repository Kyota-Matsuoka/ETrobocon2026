/******************************************************************************
 *  SimpleTimer.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class SimpleTimer
 *  Author: Kenya Yabe
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_UNIT_SIMPLETIMER_H_
#define ETTR_UNIT_SIMPLETIMER_H_

#include "Clock.h"

class SimpleTimer {
public:
    explicit SimpleTimer(const spikeapi::Clock& clock);

    void setTime(uint32_t delay);
    void start();
    void stop();
    bool isTimedOut() const;
    bool isStarted() const;

private:
    const spikeapi::Clock& mClock;
    uint32_t mDelayMs;
    uint32_t mStartTime;
    uint32_t mTargetTime;
};

#endif  // ETTR_UNIT_SIMPLETIMER_H_
