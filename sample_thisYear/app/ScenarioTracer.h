/******************************************************************************
 *  ScenarioTracer.h (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class ScenarioTracer
 *  Author: Kenya Yabe
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_APP_SCENARIOTRACER_H_
#define ETTR_APP_SCENARIOTRACER_H_

#include "Walker.h"
#include "SimpleTimer.h"
#include "Test.h"

#include "Starter.h"
#include "SimpleTimer.h"
#include "LineTracer.h"
//追加
//#include "LineTracer_8.h"

//追加（岡田）
#include "DistanceMeter.h"




class ScenarioTracer {
public:
    ScenarioTracer(Starter* starter, Walker* walker,LineTracer* lineTracer,DistanceMeter* distanceMeter);//追加
                    // Test* test);

    void run();
    //bool isCameraScene();//カメラシーンに移行するかどうか

private:
    enum State {
        UNDEFINED,
        INITIAL,
        WAIT_START,
        TEST,
        //TEST2,
        //TEST3,
        //追加予定
        //LINETRACER_8,

    };

    
    //追加（岡田）
    float preDistance;
    float currentDistance;
    float diffDistance;
    float SumDistance;
    bool IsInitialized;

    Starter* mStarter;
    Walker* mWalker;
    //Test* mTest;
    LineTracer* mLineTracer;
    State mState;
     //追加
    //LineTracer_8* mLineTracer_8;
    
    //追加（岡田）
    DistanceMeter* mDistanceMeter;

    void initAction();
    void execUndefined();
    void execInitial();
    void execStart();
    void execWalking();
    void execTest();

    //追加
    //void execTest2();
    //void execLineTracer_8();//わから
    
    float calSumDistance();
    
};

#endif  // ETTR_APP_SCENARIOTRACER_H_
