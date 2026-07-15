#include "DistanceMeter.h"
#include <stdio.h>



DistanceMeter::DistanceMeter(spikeapi::Motor& leftWheel,
                                spikeapi::Motor& rightWheel)
            :mLeftWheel(leftWheel),
             mRightWheel(rightWheel),
             preLeftAngle(0),
             preRightAngle(0),
             WheelDistance_currentState(0),
             preDistance(0),
             currentDistance(0),
             DiffLeftAngle(0),
             DiffRightAngle(0),
             WheelDistance(0){
             }


float DistanceMeter::calDistance(){
    //おそらくLeftDistance,currentLeftAngleは絶対値的なもの 
    float currentLeftAngle  = mLeftWheel.getCount();//現在の左の車輪の角度を調べる
    float currentRightAngle = mRightWheel.getCount();

    //diffLeftAngelに最後合計を突っ込む
    if(DiffLeftAngle == 0 || preLeftAngle == 0){
        DiffLeftAngle = currentLeftAngle;
        preLeftAngle  = currentLeftAngle;
    }
    if(DiffRightAngle == 0 || preRightAngle == 0){
        DiffRightAngle = currentRightAngle;
        preRightAngle  = currentRightAngle;
    }
    //バック判定
    if(currentLeftAngle < preLeftAngle)//前回のほうが大きい->バックしている->前回の値に今回の差を絶対値でタス
        DiffLeftAngle = DiffLeftAngle + (preLeftAngle - currentLeftAngle);
    else  
        DiffLeftAngle  = DiffLeftAngle + (currentLeftAngle -  preLeftAngle);
    

    if(currentRightAngle < preRightAngle)
        DiffRightAngle = DiffRightAngle + (preRightAngle - currentRightAngle);
    
    else
        DiffRightAngle = DiffLeftAngle + (currentRightAngle - preRightAngle);
    

    
    //preLeftAngleはずっと0で固定 DiffLeftAngleはずっと大きくなり津図ける　バックだとちいさくなりつずける
    //前回から車輪が動いた角度を調べる

    preLeftAngle  = currentLeftAngle;//更新
    preRightAngle = currentRightAngle;
    

    //直径*3.14 =　弧の長さ   直径:2
    float LeftDistance  = (5.5 * 3.14) * (DiffLeftAngle / 360);//弧の長さに対して、車輪が動いた角度分の長さを調べる
    float RightDistance = (5.5 * 3.14) * (DiffRightAngle / 360);

    
    //ここまで


    WheelDistance = (LeftDistance + RightDistance)/2;
    
    printf("WheelDisntance:%.2f\n",WheelDistance);

    return WheelDistance;
}
float DistanceMeter::calDistance_currentState(){
    currentDistance = DistanceMeter::calDistance();
        if(preDistance == 0){//呼び出した時リセットが必要
            preDistance = currentDistance;//計算を開始したときの今までの距離
        }
        WheelDistance_currentState = WheelDistance_currentState + (currentDistance - preDistance);//計算開始からの距離合計
        preDistance  = currentDistance;
        //printf("currentDisntance:%.2f",currentDistance);
        //printf("preDistance:%.2f",preDistance);
        
    printf("WheelDisntance_currentState:%.2f\n",WheelDistance_currentState);

    return WheelDistance_currentState;
}
void  DistanceMeter::calDistance_currentStateReset(){
    WheelDistance_currentState = 0.0;
    preDistance = 0.0;
}
