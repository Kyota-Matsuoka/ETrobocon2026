/******************************************************************************
 *  ScenarioTracer.cpp (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class ScenarioTracer
 *  Author: Kenya Yabe
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "ScenarioTracer.h"

#include <stdio.h>

/**
 * コンストラクタ
 * @param walker          走行
 * @param scenario        シナリオ
 * @param timer           タイマ
 */
ScenarioTracer::ScenarioTracer(Starter* starter, Walker* walker,LineTracer* lineTracer,DistanceMeter *distanceMeter)//追加
                                // Test* test)
    : mStarter(starter),
      mWalker(walker),
      //mTest(test),
      mLineTracer(lineTracer),
      //追加
      //mLineTracer_8(lineTracer_8),
      mState(UNDEFINED),
      preDistance(0),
      currentDistance(0),
      diffDistance(0),
      SumDistance(0),
      IsInitialized(false),
      mDistanceMeter(distanceMeter)
      {
}

/**
 * シナリオトレースする
 */
void ScenarioTracer::run() {
    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case INITIAL:
        execInitial();
        break;
    case WAIT_START:
        execStart();
        break;
    case TEST: //状態がTESTの場合、execTest()関数を呼び出す
        execTest();
        break;
    //追加
    /*
    case LINETRACER_8://mState 5と表示される
        execLineTracer_8();
        break;
    *///mState4と表示される
    default:
        break;
    }
    printf("\nmState:%d\n",mState);
}

float ScenarioTracer::calSumDistance(){
        currentDistance = mDistanceMeter->calDistance();
        if(preDistance == 0){
            preDistance =currentDistance;//計算を開始したときの今までの距離
        }
        diffDistance = diffDistance + (currentDistance - preDistance);//計算開始からの距離合計
        printf("diffDistance:%f\n",diffDistance);
        preDistance  = currentDistance;

        return diffDistance;
}

/**
 * シナリオトレース初期化処理
 */
void ScenarioTracer::initAction() {
    mWalker->init();
}


/**
 * 未定義状態の処理
 */
void ScenarioTracer::execUndefined() {
    mState = INITIAL;
}

/**
 * 初期状態の処理
 */
void ScenarioTracer::execInitial() {
    initAction();   		// ① entry アクション(INITIAL)

    mState = WAIT_START;   	// ① 状態遷移

}

void ScenarioTracer::execStart(){
    if(!mStarter->isPushed()){
        printf("wait");
    }else{
        mState = TEST;  //TEST状態に遷移する
    }
}


//走行中状態の処理

//呼ばれていない
/*
void ScenarioTracer::execWalking() {
   
    //mWalker->run();             		// ② do アクティビティ

    if (mSimpleTimer->isTimedOut()) {   // ② イベントチェック
        mSimpleTimer->stop();   		// ② exit アクション

        modeChangeAction();     		// ② entry アクション(WALKING)
    }
}
*/
void ScenarioTracer::execTest(){
    mLineTracer->run(true,0);
    //変更// ② do アクティビティ
    
    //mTest->run();

//追加（岡田）
    
    /*
    SumDistance = ScenarioTracer::calSumDistance();
    if(0 <= SumDistance && SumDistance < 280){
        printf("直進加速中\n");
        printf("SumDistance:%f\n",SumDistance);
        mLineTracer->run_so_fast(false,0);     		
    } else if(280 <= SumDistance && SumDistance < 333){
        printf("カーブ減速中\n");
        printf("SumDistance:%f\n",SumDistance);
        mLineTracer->run_fast(false,-10);
    } else if(333 <= SumDistance && SumDistance < 480){
        printf("直線加速中\n");
        printf("SumDistance:%f\n",SumDistance);
        mLineTracer->run_so_fast(false,0);
    } else if(480 <= SumDistance && SumDistance < 538){
        printf("カーブ減速中\n");
        printf("SumDistance:%f\n",SumDistance);
        mLineTracer->run_fast(false,-7);
    } else {
        printf("通常走行に戻ります\n");
        mLineTracer->run_so_fast(false,0);
    }
    

    //mLineTracer->run_fast(false,0);     		// ② do アクティビティ
    
    //mTest->run();
    */
}
/*
//追加
void ScenarioTracer::execLineTracer_8() {
    printf("execLineTracer_8を呼び出してます\n");
    if(mLineTracer_8->ScenarioTransition()){
            printf("TEST2に移行します\n");
            mState = TEST2;//exeWalking2           
    }
    mLineTracer_8->run_8();    
}
//追加
void ScenarioTracer::execTest2() {
    printf("TEST2をよびだしてます。\n");
    //mLineTracer->run(true,0); //8の字超えた後            		// ② do アクティビティ

    if (mSimpleTimer->isTimedOut()) {   // ② イベントチェック
        mSimpleTimer->stop();   		// ② exit アクション

        modeChangeAction();     		// ② entry アクション(WALKING)
    }
   

    if (IsInitialized == false) {
        SumDistance = 0;
        preDistance = 0;
        currentDistance = 0;
        diffDistance = 0;
        IsInitialized = true;

    }
    printf("SumDistance:%f\n",SumDistance);
    SumDistance = ScenarioTracer::calSumDistance();
    if(0.0 <= SumDistance && SumDistance <= 65.0){

        mLineTracer->run_fast(true,0);
        printf("直進加速中\n");

    } else if(65.0 < SumDistance && SumDistance <= 75.0){
        mLineTracer->run(true,0); //8の字超えた後            		// ② do アクティビティ
    }else{
        mState = TEST3;
        //mWalker -> stop();
    }

}

bool ScenarioTracer::isCameraScene(){
    //return true;
    
    if(mState == TEST3){//TEST2の場合CameraSceneに遷移
        return true;
    }else{
        return false;
    }
        

    
}
*/


