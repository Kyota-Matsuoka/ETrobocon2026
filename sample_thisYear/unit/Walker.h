/******************************************************************************
 *  Walker.cpp (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class Walker
 *  Author: Kazuhiro.Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_UNIT_WALKER_H_
#define ETTR_UNIT_WALKER_H_

#include "Motor.h"

class Walker {
public:
    static const int RIGHT_TURN = -30;  
    static const int LEFT_TURN = 30; 

    static const int8_t RIGHT;
    static const int8_t LEFT;
    static const int8_t DECIDE;   
  
    Walker(spikeapi::Motor& leftWheel,
                    spikeapi::Motor& rightWheel);

    void init();
    void run();
    /*
    void doubleLoop_run();
    void gate_run(); 
    void gate2_run();
    void put_run();
    void run_fast(); //追加（岡田）
    void run_so_fast(); //tuika(okada)
    */
    void setCommand(float turn);
    void OnOff(uint8_t turn);
    void turn(int LeftTurn,int RightTurn);
    void stop();

private:
    spikeapi::Motor& mLeftWheel;
    spikeapi::Motor& mRightWheel;
    int mTurn;
    int mForward;
    int mForwardRight;
    int mForwardLeft;

  #ifndef MAKE_RASPIKE
    const int8_t pwm = 38;//55;//50;//70;
    /*
    const int8_t pwm_8 = 30;
    const int8_t pwm_gate = 50;//40;
    const int8_t pwm_put  = 45;//40;
    const int8_t pwm_gate2divide  =  20;//平常時に右に進むように調節する変数
    */
  #else

    const int8_t pwm = 38;//40;
    /*
    const int8_t pwm_8 = 30;//40;
    const int8_t pwm_gate = 50;//40;//40;
    const int8_t pwm_put  = 45;//40;
    const int8_t pwm_gate2divide  =  20;
    */
  #endif
};

#endif  // ETTR_UNIT_WALKER_H_
