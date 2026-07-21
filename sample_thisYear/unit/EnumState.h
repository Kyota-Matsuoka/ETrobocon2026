
#ifndef ETTR_UNIT_ENUMSTATE_H_
#define ETTR_UNIT_ENUMSTATE_H_

#include "stdio.h"


enum RobotState
{
    WAITING,
    LINE_TRACE,
    CURVE,
    GOAL,
};

enum ColorLineName
{
    RED_LINE,
    BLUE_LINE,
    GREEN_LINE,
    BLACK_LINE,
    UNKNOWN,
};



#endif