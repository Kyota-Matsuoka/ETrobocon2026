/******************************************************************************
 *  LineMonitor.cpp (for SPIKE)
 *  Created on: 2025/01/05
 *  Implementation of the Class LineMonitor
 *  Author: Kazuhiro.Kawachi
 *  Modifier: Yuki Tsuchitoi
 *  Copyright (c) 2025 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineMonitor.h"
#include "stdio.h"
//#include "iostream"


/*
本番確認すること　->確認したらそれらの閾値設定
CameraShowではなく、CameraSceneで呼び出して値を取得する

CameraShow
練習環境
青ライン上ではr<g<bの順番で大きい
赤ライン上ではg<b<rの順番で大きい
青の上でr:70,green:200,blue:400
赤の上でred:800 green:300 blue:350
黒の上でr:50 ,g:50,b:50
白のうえでr:1000,g:1000,b:1000
黒の上でs:20
青の上でs:45
赤の上でs:90
*/


// 定数宣言
const int8_t LineMonitor::INITIAL_THRESHOLD_BLACK = 20;  // 黒色の光センサ値
const int8_t LineMonitor::INITIAL_THRESHOLD_WHITE = 30;  // 白色の光センサ値

/**
 * コンストラクタ
 * @param colorSensor カラーセンサ
 */
LineMonitor::LineMonitor(const spikeapi::ColorSensor& colorSensor)
    : mColorSensor(colorSensor),
      mThreshold(70)
      //追加
      //blue(INITIAL_BLUE_)     閾値の予定
      {
}

/**
 * ライン境界から外れた度合いを判定する
 * @retval ライン境界とセンサ値との差分
 */
int LineMonitor::calDiffReflection() {//calDiffSReflectionの後のconstを消した、constは本来mColorSensorをインスタンス化する時、constの構造体のため、そろえるために必要
    // 光センサからの取得値を見て
    // ライン境界の値との差分を算出して返す
    float diff = mColorSensor.getReflection() - (float)mThreshold;

    

    //追加
    //mColorSensor.getHSV(hsv_val);
    //printf("h:%u     s:%u     v:%u\n",hsv_val.h, hsv_val.s, hsv_val.v);
    printf("diff:%.2f\n",diff);//diffが0よりおおきければ、ライン上にいる

    return diff;
}
//追加
bool LineMonitor::isOnBlueLine() {//青ライン上かどうか
    mColorSensor.getRGB(rgb_val);
    mColorSensor.getHSV(hsv_val);
    
    printf("h:%u     s:%u     v:%u\n",hsv_val.h, hsv_val.s, hsv_val.v);
    printf("red:%u     green:%u     blue:%u\n",rgb_val.r, rgb_val.g, rgb_val.b);
    //if((rgb_val.r + 200 < rgb_val.b) && (rgb_val.g + 00 < rgb_val.b) && (600 < rgb_val.b && rgb_val.b < 1000) && (45 < hsv_val.s)){
    if((rgb_val.r + 200 < rgb_val.b) && (rgb_val.g + 100 < rgb_val.b) && (400 < rgb_val.b && rgb_val.b < 1000) && (45 < hsv_val.s)){
          //rがbよりも200以上小さい   gがbよりも100以上小さい    bが600よりも大きい&900よりも小さい->青ライン上かどうか
        //彩度が45以上(黒ライン上にいないことを確かめるため)
        printf("黒ライン上にいません、かつ青ライン上にいます,\n");
        return true;
    }
    else{
        return false;
    }
}

//追加
bool LineMonitor::isOnRedLine(){
    mColorSensor.getRGB(rgb_val);
    mColorSensor.getHSV(hsv_val);
    printf("h:%u     s:%u     v:%u\n",hsv_val.h, hsv_val.s, hsv_val.v);
    printf("red:%u     green:%u     blue:%u\n",rgb_val.r, rgb_val.g, rgb_val.b);
    //if((rgb_val.b + 200 < rgb_val.r) && (rgb_val.g + 400 < rgb_val.r) && (600 < rgb_val.r && rgb_val.r < 1000) && (70 < hsv_val.s)){
    if((rgb_val.b + 200 < rgb_val.r) && (rgb_val.g + 300 < rgb_val.r) && (400 < rgb_val.r && rgb_val.r < 1000) && (70 < hsv_val.s)){
        //rがbよりも400以上小さい   gがrよりも100以上小さい   rが600よりも大きい&900よりも小さい -> 青ライン上かどうか
        //彩度が70以上(黒ライン上にいないことを確かめるため)
        printf("黒ライン上にいません、かつ赤ライン上にいます,\n");
        return true;
    }
    else{
        return false;
    }
}
bool LineMonitor::isOnBlackLine(){
    // 光センサからの取得値を見て
    // 黒以上であれば「true」を、
    // そうでなければ「false」を返す
    //mColorSensor.getRGB(rgb_val);
    mColorSensor.getHSV(hsv_val);
    mColorSensor.getRGB(rgb_val);
    printf("h:%u     s:%u     v:%u\n",hsv_val.h, hsv_val.s, hsv_val.v);
    printf("red:%u     green:%u     blue:%u\n",rgb_val.r, rgb_val.g, rgb_val.b);
    //if (mColorSensor.getBrightness() >= mThreshold) {
    //RGBtoHSV();
    if((hsv_val.v <= 40) && rgb_val.r < 300 && rgb_val.g < 300 && rgb_val.b < 300){//vは明度
        printf("黒ライン上にいます！\n");
        return true; 
    } else {
        return false;
    }
}

bool LineMonitor::isOnLine(){
    // 光センサからの取得値を見て
    // 黒以上であれば「true」を、
    // そうでなければ「false」を返す
    //mColorSensor.getRGB(rgb_val);
    mColorSensor.getHSV(hsv_val);
    printf("h:%u     s:%u     v:%u\n",hsv_val.h, hsv_val.s, hsv_val.v);
    //if (mColorSensor.getBrightness() >= mThreshold) {
    //RGBtoHSV();
    if(hsv_val.v >= mThreshold) {//vは明度
        return true; 
    } else {
        return false;
    }
}

/**
 * ライン閾値を設定する
 * @param threshold ライン閾値
 */
void LineMonitor::setThreshold(int8_t threshold) {
    mThreshold = threshold;
}

//追加
bool LineMonitor::notIsBlueLine(){//青ライン上にいないかどうか
    
    mColorSensor.getHSV(hsv_val);
    if(hsv_val.s < 30){//彩度が30以下
        if(isOnLine()){
            return true;//青ライン上にいない&黒ライン上にいる
        }
    }else{
        return false;
    }

}