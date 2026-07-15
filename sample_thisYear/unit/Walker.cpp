/******************************************************************************
 *  Walker.cpp (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class Walker
 *  Author: Kazuhiro.Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Walker.h"
#include "stdio.h"

const int8_t Walker::RIGHT  = 0;     // 左方向
const int8_t Walker::LEFT   = 1;     // 右方向
const int8_t Walker::DECIDE = 2;

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
Walker::Walker(spikeapi::Motor& leftWheel,
                                 spikeapi::Motor& rightWheel)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mForward(30),
      mForwardRight(30),
      mForwardLeft(30),
      mTurn(0) {
}

/**
 * 走行する
 */
void Walker::run() {
    // 左右モータに回転を指示する 
    //pwmは現在30,mTurnは平常時で-3ほど  
	mLeftWheel.setPower(pwm - mTurn); 
	mRightWheel.setPower(pwm + mTurn);
	printf("mLeftWheel:%d\n",pwm-mTurn);
	printf("mRightWheel:%d\n",pwm+mTurn);

}
/*
void Walker::doubleLoop_run() {
    // 左右モータに回転を指示する 
    //pwmは現在30,mTurnは平常時で-3ほど  
	mLeftWheel.setPower(pwm_8 - mTurn); 
	mRightWheel.setPower(pwm_8 + mTurn);
	printf("mLeftWheel:%d\n",pwm-mTurn);
	printf("mRightWheel:%d\n",pwm+mTurn);
}

void Walker::gate_run() {
    // 左右モータに回転を指示する 
    //pwmは現在30,mTurnは平常時で-3ほど  
    if(pwm_gate - mTurn < 10){
        mLeftWheel.setPower(10);
        mRightWheel.setPower(pwm_gate + mTurn);
	    printf("mLeftWheel:%d\n",10);
	    printf("mRightWheel:%d\n",pwm_gate+mTurn);
    }
    else if(pwm_gate + mTurn < 10){
        mLeftWheel.setPower(pwm_gate - mTurn);
        mRightWheel.setPower(10);  
	    printf("mLeftWheel:%d\n",pwm_gate-mTurn);
	    printf("mRightWheel:%d\n",10);
    }
    else{
	    mLeftWheel.setPower(pwm_gate - mTurn); 
	    mRightWheel.setPower(pwm_gate + mTurn);
        printf("mLeftWheel:%d\n",pwm_gate-mTurn);
	    printf("mRightWheel:%d\n",pwm_gate+mTurn);
    }
	
}
void Walker::gate2_run() {
    // 左右モータに回転を指示する 
    //pwmは現在30,mTurnは平常時で-3ほど  
    if(pwm_gate - mTurn < 10){
        mLeftWheel.setPower(10);
        mRightWheel.setPower(pwm_gate + mTurn);
	    printf("mLeftWheel:%d\n",10);
	    printf("mRightWheel:%d\n",pwm_gate+mTurn);
    }
    else if(pwm_gate + mTurn < 10){
        mLeftWheel.setPower(pwm_gate - mTurn);
        mRightWheel.setPower(10);  
	    printf("mLeftWheel:%d\n",pwm_gate-mTurn);
	    printf("mRightWheel:%d\n",10);
    }
    else{
	    mLeftWheel.setPower(pwm_gate - mTurn ); 
	    mRightWheel.setPower(pwm_gate + mTurn);
        printf("mLeftWheel:%d\n",pwm_gate-mTurn);
	    printf("mRightWheel:%d\n",pwm_gate+mTurn);
    }
	
}

void Walker::put_run() {
    // 左右モータに回転を指示する 
    //pwmは現在30,mTurnは平常時で-3ほど  
	mLeftWheel.setPower(pwm_put - mTurn); 
	mRightWheel.setPower(pwm_put + mTurn);
	printf("mLeftWheel:%d\n",pwm-mTurn);
	printf("mRightWheel:%d\n",pwm+mTurn);

}

void Walker::turn(int LeftTurn,int RightTurn){
    mLeftWheel.setPower(LeftTurn);
    mRightWheel.setPower(RightTurn);
    
    printf("mLeftWheel:%d\n",LeftTurn);
	printf("mRightWheel:%d\n",RightTurn);
}
*/

//追加（岡田）
/**
 * 停止する
 */
void Walker::stop() {
    // 左右モーターに回転を静止させる
    mLeftWheel.setPower(0);
    mRightWheel.setPower(0);
    printf("mLeftWheel:%d\n",pwm-mTurn);
	printf("mRightWheel:%d\n",pwm+mTurn);
}

/*

//追加（岡田）
void Walker::run_fast() {  
    mLeftWheel.setPower(pwm + 20 - mTurn); 
    mRightWheel.setPower(pwm + 20 + mTurn);
    printf("mLeftWheel:%d\n",pwm+15-mTurn);
    printf("mRightWheel:%d\n",pwm+15+mTurn);

}

//tuika(okada)
void Walker::run_so_fast() {
    mLeftWheel.setPower(pwm + 45 - mTurn); 
    mRightWheel.setPower(pwm + 45 + mTurn);
    printf("mLeftWheel:%d\n",pwm+40-mTurn);
    printf("mRightWheel:%d\n",pwm+40+mTurn);
    
}

*/
/**
 * 走行に必要なものをリセットする
 */
void Walker::init() {
    // モータをリセットする
    mLeftWheel.resetCount();
    mRightWheel.resetCount();
}

/**
 * PWM値を設定する
 * @param forward 前進値
 * @param turn    旋回方向
 */
void Walker::setCommand(float turn) {
    mTurn    = (int)turn;
}


//使わない:PID制御が組み込まれていないため
void Walker::OnOff(uint8_t turn){//turnには０，１，２が入る
    // 左右モータに回転を指示する
    int rightPWM = 0;
    int leftPWM = 0;
    
    if(turn == RIGHT) {//RIGHT=0、右に回る->左モータを回転させる
        rightPWM = 0;
        leftPWM = mForward;
    } else if(turn == LEFT) {
        rightPWM = mForward;//状況に応して、mForwardの値を変えるのもあり？
        leftPWM = 0;
    } else if(turn = DECIDE){
        rightPWM = mForwardRight;
        leftPWM = mForwardLeft;
    }else {
        rightPWM = 0;
        leftPWM = 0;
    }
    
    printf("rightPWM:%d\tleftPWM:%d\n",rightPWM, leftPWM);
    mRightWheel.setPower(rightPWM);
    mLeftWheel.setPower(leftPWM);
}
