#include "Test.h"
#include<stdio.h>
// 定数宣言
const float Test::Kp = 0.83;
const int 	Test::bias = 0;
  
/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param walker 走行
 */
Test::Test(LineMonitor* lineMonitor,
                       Walker* walker)
    : mLineMonitor(lineMonitor),
      mWalker(walker),
      mIsInitialized(false) {
}

/**
 * ライントレースする
 */
void Test::run() {
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    int diffReflection = mLineMonitor-> calDiffReflection();
    
    // 走行体の操作量を計算する
  	float turn = calcPropValue(diffReflection);

  	mWalker->setCommand(turn);


    // 走行を行う
    //mWalker->run();
    if(mLineMonitor->isOnLine()){
        mWalker->OnOff(Walker::RIGHT);
    }else{
        mWalker->OnOff(Walker::LEFT);
    }

}

/**
 * 走行体の操作量を計算する
 * @param diffBrightness ラインから外れた度合い（ライン閾値との差）
 */
float Test::calcPropValue(int diffBrightness) {
    float turn = Test::Kp * diffBrightness + Test::bias;
	
    return turn;
}
