/******************************************************************************
 *  LineMonitor.h (for SPIKE )
 *  Created on: 2025/01/05
 *  Definition of the Class LineMonitor
 *  Author: Kazuhiro Kawachi
 *  Modifier : Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef ETTR_UNIT_BASICTOOLS_H_
#define ETTR_UNIT_BASICTOOLS_H_

#include "stdio.h"
#include "spikeapi.h"
#include "syssvc/syslog.h"
#include "Light.h"
#include "Button.h"
#include "Clock.h"
//#include "UltrasonicSensor.h"
#include "Display.h"
#include "Speaker.h"
#include "IMU.h"
#include "UltrasonicSensor.h"
#include "EnumState.h"

//カラーの名前の定義 : switch-caseで使用する場合に必要

class BasicTools {
public:
    BasicTools(spikeapi::Light& light,spikeapi::Button& button,
        spikeapi::Clock& clock,spikeapi::Display& display,spikeapi::Speaker& speaker, spikeapi::IMU& imu,spikeapi::UltrasonicSensor& ultrasonicSensor);


    //関数の宣言
    //ディスプレイ表示
    void showMessage(char* message, uint32_t on, uint8_t off);//メッセージを表示
    void showChar(char character);
    void showNumber(int8_t number);
    void showScrollMessage(char* message,uint32_t delay);//文字列をスクロールして表示
    void showVariable(char* variableName,int8_t variable,uint32_t on, uint8_t off);//変数の名前と数値を表示
    //スピーカー
    void playSound(uint16_t frequency, uint32_t duration,uint32_t sleep_duration,uint8_t volume);//音を鳴らす
    //ボディライト
    void turnOnLight(spikeapi::Light::EColor color);//ボディのライトを点灯させる
    void turnOffLight(uint32_t duration);//ボディのライトを消灯させる
    void turnOnLightAmountOfTime(char *colorName, uint32_t duration, uint32_t interval, uint32_t repeatCount);//一定時間、対象色を点灯・消灯させる関数
    void errorLight(char *colorName, uint32_t duration, uint32_t interval, uint32_t repeatCount);//エラー時のライト点滅
    //クロック
    uint32_t getPassageOfTime(); // 開始からの経過時間を取得する *マイクロ秒単位なので注意/10,000,000 µs = 10秒
    uint32_t getInternalPassageOfTime();//システム内部の経過時間(リセットできない)
    void wait(uint64_t	duration);//指定時間遅延させる
    void stop(uint64_t	duration);//指定時間停止させる
    //IMU
    void getAngularVelocity();
    //超音波センサ
    int32_t getDistance();//単位:cm
    void lightOff();
    void lightOn();
    void setLight(int32_t bv1,int32_t bv2,int32_t bv3,int32_t bv4);//bvはライトの輝度
    void lightOnAmountOfTime(uint32_t duration,int32_t bv1,int32_t bv2,int32_t bv3,int32_t bv4);//一定時間、超音波センサのライトを光らせる



    //その他
    ColorLineName getColorLineName();
    void error();//エラー表示を音・ディスプレイ・ボディライトで示す
    void showMessageColorLineName(ColorLineName colorLineName,uint32_t on, uint8_t off);//ラインの色を表示する
    void showMessageRunnningEdge();//走行中のラインの端を表示する(左ラインもしくは右ライン走行かのどちらかを表示する)
    void showMessageRunningDistance();//走行距離を表示する
    void showMessageRunningSpeed();//走行速度を表示する

    //Stateを表示する場合、EnumStateへの追加、robotoStateToString関数への追加が必要
    //また、使用するファイルでEnumState.hをインクルードする必要がある
    char* robotStateToString(RobotState robotState);//stateを文字列として変換する関数
    void showMessageState(RobotState robotState,uint32_t on, uint8_t off);//ステート(状態)を表示する
    
    void changeColorOverTime(); // ライトの色を時間経過で変化させる関数


    void Guild(); // ギルドのメロディを演奏する
    void YourAffection(); 
    void OneTimePlaySound(); //音楽を一定周期で鳴らす関数

private:
    spikeapi::Light& mLight;
    spikeapi::Button& mButton;
    spikeapi::Clock& mClock;
    spikeapi::Display& mDisplay;
    spikeapi::Speaker& mSpeaker;
    spikeapi::IMU& mIMU;
    spikeapi::UltrasonicSensor& mUltrasonicSensor;

    spikeapi::Light::EColor color;
    spikeapi::IMU::AngularVelocity gyzo;
    bool hasPlayed;
    bool firstPlay;
    int8_t volume;
    uint32_t playSongInterval;//型を揃える
    uint32_t presentTime;
    uint32_t lastPlayTime;
    
};



#endif  // ETTR_UNIT_BASICTOOLS_H_