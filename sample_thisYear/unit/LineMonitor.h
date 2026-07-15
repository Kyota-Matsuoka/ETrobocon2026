/******************************************************************************
 *  LineMonitor.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class LineMonitor
 *  Author: Kazuhiro Kawachi
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_UNIT_LINEMONITOR_H_
#define ETTR_UNIT_LINEMONITOR_H_

#include "ColorSensor.h"
#include "spikeapi.h"


// 定義
class LineMonitor {
public:
    LineMonitor(const spikeapi::ColorSensor& colorSensor);


    spikeapi::ColorSensor::RGB rgb_val;
    spikeapi::ColorSensor::HSV hsv_val;


    int calDiffReflection() ;
    void setThreshold(int8_t threshold);
    void getHsv();
    bool isOnLine();
    //追加
    bool isOnBlueLine();
    bool isOnRedLine();
    bool isOnBlackLine();
    bool notIsBlueLine();
    
    
private:
    static const int8_t INITIAL_THRESHOLD_BLACK;
	static const int8_t INITIAL_THRESHOLD_WHITE;

    const spikeapi::ColorSensor& mColorSensor;
    int8_t mThreshold;
};

#endif  // ETTR_UNIT_LINEMONITOR_H_
