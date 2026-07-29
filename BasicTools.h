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
#include "DistanceMeter.h"
#include "Starter.h"

//カラーの名前の定義 : switch-caseで使用する場合に必要

class BasicTools {
public:
    BasicTools(spikeapi::Light& light,spikeapi::Button& button,
        spikeapi::Clock& clock,spikeapi::Display& display,spikeapi::Speaker& speaker, spikeapi::IMU& imu,spikeapi::UltrasonicSensor& ultrasonicSensor,DistanceMeter* distanceMeter,Starter* starter);


    //10ミリ秒ごとに周期ハンドラは呼ばれる

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
    void  getAngularVelocity();
    void  getOffset(float& gx,float &gy, float& gz);//オフセットの値を取得する関数  → gxとgz両方を返したいので参照渡しで返す  :使用する側のファイルであらかじめgx,gy,gzを宣言しておく
    float getCurrentRotationAngle();//前回の周期呼び出し(0.01秒)からどの方向(X軸)にどの程度回転しているかを取得する
    float calRotationAngle();//開始からどの方向(X軸)にどの程度回転しているかを更新する (オフセット補正が必要)
    float getRotationAngle();
    float getRotationAngle_LapGateArea();//エリア1開始からどの方向にどの程度回転しているかを取得する
    float getRotationAngle_ETsumouArea();//以下ほぼ同文
    float getRotationAngle_BottleDeliveryArea();
    float getRotationAngle_ETRallyArea();
    float getRotationAngle_GarageArea();

    void  resetCallRotationAngle();//これは、reachedTargetAngle()とセットで呼ぶ(ResetCallRotationAngle() → reachedTargetAngle()呼び出し)
    bool  reachedTargetAngle(int targetAngle); //呼び出しから目標の角度回転したかを、真偽値で取得する(使う場合は、角度リセットする必要がある)
    //超音波センサ
    int32_t getDistance();//単位:cm
    void lightOff();
    void lightOn();
    void setLight(int32_t bv1,int32_t bv2,int32_t bv3,int32_t bv4);//bvはライトの輝度
    void lightOnAmountOfTime(uint32_t duration,int32_t bv1,int32_t bv2,int32_t bv3,int32_t bv4);//一定時間、超音波センサのライトを光らせる



    //その他

    void periodic(); //初回は必ずtrueを返し、それ以降は特定の条件でのみtrueを返す関数


    ColorLineName getColorLineName();
    void error();//エラー表示を音・ディスプレイ・ボディライトで示す
    void showMessageColorLineName(ColorLineName colorLineName,uint32_t on, uint8_t off);//ラインの色を表示する
    void showMessageRunnningEdge();//走行中のラインの端を表示する(左ラインもしくは右ライン走行かのどちらかを表示する)
    void showMessageRunningDistance();//走行距離を表示する
    void showMessageRunningSpeed();//走行速度を表示する

    //Stateを表示する場合、EnumStateへの追加、robotoStateToString関数への追加が必要
    //また、使用するファイルでEnumState.hをインクルードする必要がある
    char* robotStateToString(RobotState robotState);//stateを文字列として変換する関数
    void  showMessageState(RobotState robotState,uint32_t on, uint8_t off);//ステート(状態)を表示する
    
    void changeColorOverTime(); // ライトの色を時間経過で変化させる関数

    void getDeltaPosition(float &transformedDeltaX_dummie,float &transformedDeltaY_dummie,float &deltaX_dummie,float &deltaY_dummie);//前回の周期呼び出しからの移動距離(XY値)を座標で取得する
    void getCurrentPosition();//常に呼び出しておく　float transformedDeltaX,float transformedDeltaY);//現在のX座標を取得する(参照渡しで返す)
    float getCurrentPosition_X();//現在のX座標を取得する
    float getCurrentPosition_Y();//現在のY座標を取得する
    float getCurrentPosition_LapGateArea(float transformedDeltaX,float transformedDeltaY);//LapGateでの現在値を取得する関数
    
    bool  straightRun(float targetDistance);//与えられた距離分、直進したかどうかを真偽値で返す関数  :また目標まで走った場合は、mWalkerのstop関数を呼ぶ必要がある
    void  runDistanceReset(); //runDistance変数のリセット(straightRun関数とセットで活用する)

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
    DistanceMeter* mDistanceMeter; 
    Starter* mStarter;

    spikeapi::Light::EColor color;
    spikeapi::IMU::AngularVelocity gyzo;
    bool hasPlayed;
    bool firstPlay;
    int8_t volume;
    uint32_t playSongInterval;//型を揃える
    uint32_t presentTime;
    uint32_t lastPlayTime;

    float imuTiltAngle; //ロボットのデバイスの傾き
    float imuTiltRad; //ロボットのデバイスの傾きをラジアンに変換
    float sinTilt; //サインシータの値
    float cosTilt; //コサインシータの値
    float currentAngularVelocity; //デバイスの角度を補正した角速度
    float nowRotationAngle_X;//開始からのX軸の角度
    float nowRotationAngle_Z;//開始からのZ軸の角度
    float nowRotationAngle;//開始からのXとZ軸を合成した角度
    float nowRotationAngle_LapGateArea;//エリア1からのXとZ軸を合成した角度
    float nowRotationAngle_ETsumouArea;//以下ほぼ同文
    float nowRotationAngle_BottleDeliveryArea;
    float nowRotationAngle_ETRallyArea;
    float nowRotationAngle_GarageArea;
    float nowRotationAngle_Rad;//ラジアンに変換した角度
    float callRotationAngle;//関数から呼び出している間の角度
    float deltaX;//前回の周期呼び出しからの移動距離のX座標
    float deltaY;
    float transformedDeltaX;//変換した後(90度回転)のdeltaX
    float transformedDeltaY;
    float currentX;//現在のX座標
    float currentY;
    float currentDistance;//直近で動いた距離(1周期分)
    float currentRotationAngle;//直近で動いた角度(1周期分)
    float runDistance; //走った距離
    bool  mWalkerIsInitialized; //walkerのinitを初期化したかどうか
    bool  mPeriodicIsInitialized;//Perodic関数の初回呼び出し時の管理変数


    
};



#endif  // ETTR_UNIT_BASICTOOLS_H_