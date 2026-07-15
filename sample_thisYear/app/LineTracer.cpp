/******************************************************************************
 *  LineTracer.cpp (for SPIKE) 
 *  Created on: 2025/01/05
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/
#include "LineMonitor.h"
#include "LineTracer.h"
#include "PIDControl.h"
//追加
#include "Walker.h"
#include"stdio.h"

/*PIdControl.h ni idou
// 定数宣言
const float LineTracer::Kp = 1.5;
const int 	LineTracer::bias = 0;
*/
  
/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param walker 走行
 */
LineTracer::LineTracer(LineMonitor* lineMonitor,
                                Walker* walker,DistanceMeter* distanceMeter,
                                        PIDControl* pidControl)//追加
                       
    : mLineMonitor(lineMonitor),
      mWalker(walker),
      mDistanceMeter(distanceMeter),
      //tuika
      mPIDControl(pidControl),
      mIsInitialized(false){
}
/*
      //追加

      mIsOnBlueLine(false),
      mNeverCalled(false),
      mToLineTracer_8(false) {
}
*/



/**
 * ライントレースする
 */
void LineTracer::run(bool TurnSign,float TurnControl){//変更
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float diffReflection = mLineMonitor-> calDiffReflection();

    // 走行体の操作量を計算する
  	//float turn = calcPropValue(diffReflection);
    //LineTracer_8の時だけDの割合を0.5にする
    float turn = mPIDControl -> calcPropValue(diffReflection);

    //追加
    
    if(TurnSign == true){//符号を変更するか
        printf("左ライン走行です\n");
        turn = -turn;
    }
    //右ライン走行でturnが符号変更されずにマイナスになる
    printf("turn:%f\n",turn);
    turn = turn + TurnControl;//青ライン上で直接制御するための計算
    mWalker->setCommand(turn);//turnにはdiffの値に*1.5した値が入る
    
    
    mWalker->run();
    //mDistanceMeter->calWheelAngle();

    /*isOnlineからmWalkerのrunメソッドに変更：PID制御を組み込むため
    // 走行を行う
    if(mLineMonitor->isOnLine()){
        mWalker->OnOff(Walker::RIGHT);//ライン上であれば右に進む
    }else{
        mWalker->OnOff(Walker::LEFT);
    }
    */
}

/*

void LineTracer::ToGoal_run(bool TurnSign,float TurnControl){//変更
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float diffReflection = mLineMonitor-> calDiffReflection();

    // 走行体の操作量を計算する
  	//float turn = calcPropValue(diffReflection);
    //LineTracer_8の時だけDの割合を0.5にする
    float turn = mPIDControl -> toGoalcalcPropValue(diffReflection);

    //追加
    
    if(TurnSign == true){//符号を変更するか
        printf("左ライン走行です\n");
        turn = -turn;
    }
    //右ライン走行でturnが符号変更されずにマイナスになる
    printf("turn:%f\n",turn);
    turn = turn + TurnControl;//青ライン上で直接制御するための計算
    mWalker->setCommand(turn);//turnにはdiffの値に*1.5した値が入る
    
    
    mWalker->doubleLoop_run();
    //mDistanceMeter->calWheelAngle();

    /*isOnlineからmWalkerのrunメソッドに変更：PID制御を組み込むため
    // 走行を行う
    if(mLineMonitor->isOnLine()){
        mWalker->OnOff(Walker::RIGHT);//ライン上であれば右に進む
    }else{
        mWalker->OnOff(Walker::LEFT);
    }
    
}



//SenarioTracerから呼び出し:初めて青ラインに乗ったら遷移させる関数
bool LineTracer::ToLineTracer_8(){//LineTracer_8に遷移するかどうか
    /*
    if(mLineMonitor->isOnLine()){
        printf("黒ライン上にいます\n");
    }
    

    mIsOnBlueLine = mLineMonitor->isOnBlueLine();//mIsOnBlueLineもここで宣言してもよい

    if(mIsOnBlueLine){//青ライン上か
        if(mNeverCalled == false){//一度も今まで呼ばれたことがないか、ある場合ダメ
            mNeverCalled = true;
            printf("LineTracer_8起動\n");
            mToLineTracer_8 = true;//LineTracer_8 起動(十三機兵防衛圏みたいでかっこいい)
            return mToLineTracer_8;//単純にreturn trueでもよい   
        }
    }
    return false;
}


void LineTracer::run_fast(bool TurnSign,float TurnControl){//高速走行用
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float diffReflection = mLineMonitor-> calDiffReflection();

    float turn = mPIDControl -> calcPropValue_fast(diffReflection);//高速走行用のPID制御
    
    if(TurnSign == true){//符号を変更するか
        printf("左ライン走行です\n");
        turn = -turn;
    }
    printf("turn:%f\n",turn);
    turn = turn + TurnControl;
    mWalker->setCommand(turn);

    
    mWalker->run_fast();
    
}

void LineTracer::run_so_fast(bool TurnSign,float TurnControl){//高速走行用
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float diffReflection = mLineMonitor-> calDiffReflection();

    float turn = mPIDControl -> calcPropValue_so_fast(diffReflection);//高速走行用のPID制御
    
    if(TurnSign == true){//符号を変更するか
        printf("左ライン走行です\n");
        turn = -turn;
    }
    printf("turn:%f\n",turn);
    turn = turn + TurnControl;
    mWalker->setCommand(turn);

    
    mWalker->run_so_fast();
    
}



void LineTracer::Brun(bool TurnSign,float TurnControl){//変更
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float diffReflection = mLineMonitor-> calDiffReflection();

    // 走行体の操作量を計算する
  	//float turn = calcPropValue(diffReflection);
    //LineTracer_8の時だけDの割合を0.5にする
    float turn = mPIDControl -> BcalcPropValue(diffReflection);

    //追加
    
    if(TurnSign == true){//符号を変更するか
        printf("左ライン走行です\n");
        turn = -turn;
    }
    //右ライン走行でturnが符号変更されずにマイナスになる
    printf("turn:%f\n",turn);
    turn = turn + TurnControl;//青ライン上で直接制御するための計算
    mWalker->setCommand(turn);//turnにはdiffの値に*1.5した値が入る

    
    mWalker->run();
    //mDistanceMeter->calWheelAngle();

    isOnlineからmWalkerのrunメソッドに変更：PID制御を組み込むため
    // 走行を行う
    if(mLineMonitor->isOnLine()){
        mWalker->OnOff(Walker::RIGHT);//ライン上であれば右に進む
    }else{
        mWalker->OnOff(Walker::LEFT);
    }
    

}
#ダブルループ用のライントレースする
 
void LineTracer::doubleLoop_run(bool TurnSign,float TurnControl){//変更
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float diffReflection = mLineMonitor-> calDiffReflection();

    // 走行体の操作量を計算する
  	//float turn = calcPropValue(diffReflection);
    //LineTracer_8の時だけDの割合を0.5にする
    float turn = mPIDControl -> doubleLoop_calcPropValue(diffReflection);

    //追加
    
    if(TurnSign == true){//符号を変更するか
        printf("左ライン走行です\n");
        turn = -turn;
    }
    //右ライン走行でturnが符号変更されずにマイナスになる
    printf("turn:%f\n",turn);
    turn = turn + TurnControl;//青ライン上で直接制御するための計算
    mWalker->setCommand(turn);//turnにはdiffの値に*1.5した値が入る
    
    
    mWalker->doubleLoop_run();
    //mDistanceMeter->calWheelAngle();

    isOnlineからmWalkerのrunメソッドに変更：PID制御を組み込むため
    // 走行を行う
    if(mLineMonitor->isOnLine()){
        mWalker->OnOff(Walker::RIGHT);//ライン上であれば右に進む
    }else{
        mWalker->OnOff(Walker::LEFT);
    }
    
}
*/

/**
 * 走行体の操作量を計算する
 * @param diffBrightness ラインから外れた度合い（ライン閾値との差）
 */
 
 /*
float LineTracer::calcPropValue(int diffBrightness) {
    float turn = LineTracer::Kp * diffBrightness + LineTracer::bias;
	
    return turn;
}
*/
