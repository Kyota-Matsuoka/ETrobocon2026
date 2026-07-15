/******************************************************************************
 *  LineTracer.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_APP_LINETRACER_H_
#define ETTR_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "Walker.h"
#include "PIDControl.h"
#include "DistanceMeter.h"

class LineTracer {
public:
/* PIDControl ni idou
	static const float Kp;
	static const int   bias;
*/
	LineTracer(LineMonitor* lineMonitor,
               Walker* walker,DistanceMeter* distanceMeter,
	       //tuika
	       PIDControl* pidControl);//コンストラクタでPIDControl型の変数を受け取る

    //void run();
    //変更と追加
    void run(bool TurnSign,float TurnControl);
    /*
    void doubleLoop_run(bool TurnSign,float TurnControl);
    void ToGoal_run(bool TurnSign,float TurnControl);
    bool ToLineTracer_8();
    
    void run_fast(bool TurnSign,float TurnControl);//追加（岡田）
    void run_so_fast(bool TurnSign,float TurnControl);//追加（岡田）
    void Brun(bool TurnSign,float TurnControl);
    */
private:
    LineMonitor*   mLineMonitor;
    Walker*        mWalker;
    //追加
    PIDControl*    mPIDControl;//mはクラスを実体化したオブジェクト
    DistanceMeter* mDistanceMeter;
    
    bool mIsInitialized;
    /*
    //追加
    bool mIsOnBlueLine;
    bool mNeverCalled;
    bool mToLineTracer_8;
    */
    //float calcPropValue(int diffReflection);
};

#endif  // ETTR_APP_LINETRACER_H_
