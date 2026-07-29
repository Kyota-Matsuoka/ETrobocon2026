#include "stdio.h"
#include <cstring>
#include <cmath>
#include "BasicTools.h"
#include "LineMonitor.h"
#include "EnumState.h"
#include "syssvc/syslog.h"


constexpr float PI = 3.14159265358979323846f; //円周率の数値
/*
コンストラクタ
*/
//const使わない方が良い、コンストラクタも宣言も
BasicTools::BasicTools(spikeapi::Light& light,spikeapi::Button& button,
        spikeapi::Clock& clock,spikeapi::Display& display,spikeapi::Speaker& speaker, spikeapi::IMU& imu,spikeapi::UltrasonicSensor& ultrasonicSensor,DistanceMeter* distanceMeter,Starter* starter)
    : mLight(light),
      mButton(button),
      mClock(clock),
      mDisplay(display),
      mSpeaker(speaker),
      mIMU(imu),
      mUltrasonicSensor(ultrasonicSensor),
      mDistanceMeter(distanceMeter),
      mStarter(starter),
      volume(7), //音量の初期値
      hasPlayed(false), //音楽が再生されたかどうかのフラグ
      firstPlay(true), //初回再生かどうかのフラグ
      playSongInterval(40000000), // メロディ再生間隔 (ミリ秒)  10000000=10秒
      presentTime(0),
      lastPlayTime(0),
      imuTiltAngle(50.0f),
      imuTiltRad(imuTiltAngle * PI / 180.0f),
      sinTilt(std::sin(imuTiltRad)),
      cosTilt(std::cos(imuTiltRad)),
      currentAngularVelocity(0),
      nowRotationAngle_X(0),
      nowRotationAngle_Z(0),
      nowRotationAngle(0),
      nowRotationAngle_LapGateArea(0),
      nowRotationAngle_ETsumouArea(0),
      nowRotationAngle_BottleDeliveryArea(0),
      nowRotationAngle_ETRallyArea(0),
      nowRotationAngle_GarageArea(0),
      nowRotationAngle_Rad(0),
      callRotationAngle(0),
      deltaX(0),
      deltaY(0),
      transformedDeltaX(0),
      transformedDeltaY(0),
      currentX(0),
      currentY(0),
      currentDistance(0),
      currentRotationAngle(0),
      runDistance(0),
      mWalkerIsInitialized(false),
      mPeriodicIsInitialized(false)
      {
    // コンストラクタ内での初期化処理が必要な場合はここに記述
    // 例: ディスプレイの初期化、ライトの設定など
}

/*
ディスプレイの表示の関数
*/

void BasicTools::showMessage(char *message, uint32_t on,uint8_t off) {//英語しか無理。onは表示時間、offは消灯時間
    // メッセージを表示する処理をここに記述
    // 例: ディスプレイに文字列を表示する、ライトを点灯させるなど
    mDisplay.showText(message,on,off);
}
void BasicTools::showChar(char character){
    mDisplay.showChar(character);
}
void BasicTools::showNumber(int8_t number){
    mDisplay.showNumber(number);
}
void BasicTools::showScrollMessage(char* message,uint32_t delay){//delay:どれくらいのスピード感でスクロールするか
    mDisplay.scrollText(message,delay);
}
void BasicTools::showVariable(char* variableName,int8_t variable,uint32_t on, uint8_t off){//variableName:変数名(呼び出すときは、"変数名の名前"という感じで呼び出す)
    mDisplay.showText(variableName,on,off);
    mDisplay.showChar(':');
    mDisplay.showNumber(variable);//showNumberが2桁の数値表示できるか不明
}





/*
音声の関数
*/



void BasicTools::playSound(uint16_t frequency, uint32_t duration,uint32_t sleep_duration,uint8_t volume) {
    //durationとsleep_durationは同じ数値にすること
    // 音を鳴らす処理をここに記述
    // 例: スピーカーで指定した周波数の音を鳴らすなど
    mSpeaker.setVolume(volume);
    mSpeaker.playTone(frequency, duration);//frequencyは周波数、durationは音の長さ
    mClock.sleep(sleep_duration); // 音が鳴り終わったあと、どれくらい休止するか
}


/*
ボディライトの関数
*/

void BasicTools::turnOnLight(spikeapi::Light::EColor color) {
    // ライトを点灯させる処理をここに記述
    // 例: 指定した色でライトを点灯させるなど
    //spikeapi::Light::EColor color = spikeapi::Light::EColor::BLUE; // 例として青色に設定
    mLight.turnOnColor(color);
}

void BasicTools::turnOffLight(uint32_t duration) {
    // ライトを消灯させる処理をここに記述
    // 例: ライトを消灯させるなど
    mLight.turnOff();
    mClock.sleep(duration); // 消灯後、指定した時間待機させる
}

//一定時間、対象色を点灯・消灯させる関数  →周期で呼び出しているため、点滅できていない？
void BasicTools::turnOnLightAmountOfTime(char *colorName, uint32_t duration, uint32_t interval, uint32_t repeatCount) {
        //ポインタ変数なので、switch文は使えない、
        // デフォルトは消灯（NONE）にしておきます
        //colorはヘッダーファイルで宣言済み
        color = spikeapi::Light::EColor::NONE;

        // --- 全色の判定分岐 ---
        if (std::strcmp("BLACK", colorName) == 0) {
            color = spikeapi::Light::EColor::BLACK;
        }
        else if (std::strcmp("MAGENTA", colorName) == 0) {
            color = spikeapi::Light::EColor::MAGENTA;
        }
        else if (std::strcmp("VIOLET", colorName) == 0) {
            color = spikeapi::Light::EColor::VIOLET;
        }
        else if (std::strcmp("BLUE", colorName) == 0) {
            color = spikeapi::Light::EColor::BLUE;
        }
        else if (std::strcmp("CYAN", colorName) == 0) {
            color = spikeapi::Light::EColor::CYAN;
        }
        else if (std::strcmp("GREEN", colorName) == 0) {
            color = spikeapi::Light::EColor::GREEN;
        }
        else if (std::strcmp("YELLOW", colorName) == 0) {
            color = spikeapi::Light::EColor::YELLOW;
        }
        else if (std::strcmp("ORANGE", colorName) == 0) {
            color = spikeapi::Light::EColor::ORANGE;
        }
        else if (std::strcmp("RED", colorName) == 0) {
            color = spikeapi::Light::EColor::RED;
        }
        else if (std::strcmp("WHITE", colorName) == 0) {
            color = spikeapi::Light::EColor::WHITE;
        }
        else {
            // どの色にもマッチしない文字列が渡された場合は消灯にする
            color = spikeapi::Light::EColor::NONE;
        }


        // ライトを指定した色で点灯させ、指定した時間だけ点灯させる処理をここに記述
        // 例: 指定した色でライトを点灯させ、指定した時間だけ点灯させるなど
        for (uint32_t i = 0; i < repeatCount; ++i) {// repeatCount回繰り返す
            mLight.turnOnColor(color);
            mClock.sleep(duration);// durationミリ秒点灯
            mLight.turnOff();// ライトを消灯
            mClock.sleep(interval);// intervalミリ秒消灯
        }
    
}


/*
クロックの関数
*/
//getTim()は内部の経過時間(数値変えれない),nowも経過時間(resetで数値をリセットできる)
uint32_t BasicTools::getPassageOfTime(){
    return mClock.now();
}
uint32_t BasicTools::getInternalPassageOfTime(){
    return 0;
    //return mClock.getTim(); //→protectedなので、アクセスできない
}
void BasicTools::wait(uint64_t	duration){
    mClock.wait(duration);
}
void BasicTools::stop(uint64_t	duration){
    mClock.sleep(duration);
}


/*

IMUの関数

*/

void BasicTools::getAngularVelocity(){//数値は整数なので注意 
    //左方向が正、右方向が負 : 小数点以下も取得できる → なぜか、syslogにすると表示できなかったので、100倍とかして確認する
    //制止状態の小数点以下の数値を調べる
    //単位はdegree/sなので、1秒あたりどの程度回転するか
    //例えば、gx = 50 の場合は1秒当たり50度回転するという意味
    //10ms(10000U) → ×100回で1秒
    mIMU.getAngularVelocity(gyzo);

    float gx = gyzo.x;
    float gy = gyzo.y;
    float gz = gyzo.z;
    
    syslog(LOG_NOTICE, "gx = %d, gy = %d, gz = %d",(int)gx,(int)gy,(int)gz);
}


float BasicTools::getCurrentRotationAngle(){//周期での回転角度(0.01秒に回転した量)を取得する
    //imuTiltRadはデバイスの傾き,gyzoは取得した角速度,
    //デバイスの傾きを考慮した補正をラジアンに変換
    imuTiltRad = imuTiltAngle * PI / 180.0f;
    sinTilt = std::sin(imuTiltRad);
    cosTilt = std::cos(imuTiltRad);

    //角速度を取得するs
    mIMU.getAngularVelocity(gyzo);
    //デバイスの傾きを考慮した角速度(合成)
    currentAngularVelocity = ((gyzo.x * cosTilt) + (gyzo.z * sinTilt));
    //0.01秒の単位にもどす
    currentAngularVelocity = currentAngularVelocity * 0.01f;

    return currentAngularVelocity;
}


//これはずっと呼び出し続ける
float BasicTools::calRotationAngle(){//角速度ベクトルから、傾きを考慮した軸
    currentAngularVelocity = getCurrentRotationAngle();
    nowRotationAngle += currentAngularVelocity;

    //float gy = gyzo.y;
    //float gz = gyzo.z;
    
    //syslog(LOG_NOTICE, "currentAngularVelocity = %d",(int)currentAngularVelocity);
    //syslog(LOG_NOTICE, "nowRotationAngle = %d",(int)nowRotationAngle);

    return nowRotationAngle;
}
float BasicTools::getRotationAngle(){
    return nowRotationAngle;
}

float BasicTools::getRotationAngle_LapGateArea(){
    currentAngularVelocity = getCurrentRotationAngle();
    nowRotationAngle_LapGateArea += currentAngularVelocity;
    return nowRotationAngle_LapGateArea;
}

float BasicTools::getRotationAngle_ETsumouArea(){
    currentAngularVelocity = getCurrentRotationAngle();
    nowRotationAngle_ETsumouArea += currentAngularVelocity;
    return nowRotationAngle_ETsumouArea;
}
float BasicTools::getRotationAngle_BottleDeliveryArea(){
    currentAngularVelocity = getCurrentRotationAngle();
    nowRotationAngle_BottleDeliveryArea += currentAngularVelocity;
    return nowRotationAngle_BottleDeliveryArea;
}
float BasicTools::getRotationAngle_ETRallyArea(){
    currentAngularVelocity = getCurrentRotationAngle();
    nowRotationAngle_ETRallyArea += currentAngularVelocity;
    return nowRotationAngle_ETRallyArea;
}
float BasicTools::getRotationAngle_GarageArea(){
    currentAngularVelocity = getCurrentRotationAngle();
    nowRotationAngle_GarageArea += currentAngularVelocity;
    return nowRotationAngle_GarageArea;
}



void BasicTools::getOffset(float &gx,float &gy,float &gz){
    //int型でしか表示できないため、小数点以下を表示するために10000倍する
    mIMU.getAngularVelocity(gyzo);
    gx = gyzo.x * 10000;
    gy = gyzo.y * 10000; 
    gz = gyzo.z * 10000;
    syslog(LOG_NOTICE, "gx = %d, gy = %d, gz = %d",(int)gx,(int)gy,(int)gz);

}
void  BasicTools::resetCallRotationAngle(){
    callRotationAngle = 0;
}

bool  BasicTools::reachedTargetAngle(int targetAngle){ //使用する時は、if()で確認→未達の場合、処理を続ける.という形
    //imuTiltRadはデバイスの傾き,gyzoは取得した角速度,
    //デバイスの傾きを考慮した補正をラジアンに変換
    imuTiltRad = imuTiltAngle * PI / 180.0f;
    sinTilt = std::sin(imuTiltRad);
    cosTilt = std::cos(imuTiltRad);

    //角速度を取得するs
    mIMU.getAngularVelocity(gyzo);
    //デバイスの傾きを考慮した角速度(合成)
    currentAngularVelocity = ((gyzo.x * cosTilt) + (gyzo.z * sinTilt));
    //1sは10ms(10000U)の1/100倍
    callRotationAngle += currentAngularVelocity * 0.01f;


    //条件式(目標の角度に到達したか)
    if(callRotationAngle <= targetAngle) //未達
        return false;
    else{ //到達ずみ
        resetCallRotationAngle();//到達したらリセット？
        return true;
    }
}


/*
超音波センサの関数
*/
int32_t BasicTools::getDistance(){//単位:cm
    return mUltrasonicSensor.getDistance();
}

void BasicTools::lightOff(){
    mUltrasonicSensor.lightOff();
}
    
void BasicTools::lightOn(){
    mUltrasonicSensor.lightOn();
}

void BasicTools::setLight(int32_t bv1,int32_t bv2,int32_t bv3,int32_t bv4){
    mUltrasonicSensor.setLight(bv1,bv2,bv3,bv4);
}

void BasicTools::lightOnAmountOfTime(uint32_t duration,int32_t bv1,int32_t bv2,int32_t bv3,int32_t bv4){
    // セット
    mUltrasonicSensor.setLight(bv1, bv2, bv3, bv4);
    //点灯
    mUltrasonicSensor.lightOn();

    // duration ミリ秒待つ
    mClock.sleep(duration);

    // 消灯
    mUltrasonicSensor.lightOff();
}



/*
その他の関数
*/

/*


この関数は、lineMonitorから呼び出す!!!


ColorLineName BasicTools::getColorLineName(){
    if(mLineMonitor->isOnBlackLine())
        return BLACK_LINE;
    else if(mLineMonitor->isOnBlueLine())
        return BLUE_LINE;
    else if(mLineMonitor->isOnRedLine())
        return RED_LINE;
    else
        return UNKNOWN;
}
        
*/



void BasicTools::periodic(){
    if(mPeriodicIsInitialized == false){
        mPeriodicIsInitialized = true;
        //ここに、リセットしたい変数を書く
    }

    //ここに、条件式を書く
    if(mStarter->isPushed()) {
        runDistanceReset();
    }
}

void BasicTools::showMessageColorLineName(ColorLineName colorLineName,uint32_t on, uint8_t off){

    switch(colorLineName)
    {
        case BLACK_LINE:
            mDisplay.showText("BLACK",on,off);
            break;
        case BLUE_LINE:
            mDisplay.showText("BLUE",on,off);
            break;
        //case GREEN_LINE:
        case RED_LINE:
            mDisplay.showText("RED",on,off);
            break;
        default: 
            mDisplay.showText("UNKNOWN", on, off);
            break;
    }
}

void BasicTools::changeColorOverTime(){
    presentTime = mClock.now();
    color = spikeapi::Light::EColor::NONE;

    if(presentTime <= 30000000){ //30秒
        color = spikeapi::Light::EColor::BLUE;
        mLight.turnOnColor(color);
    }
    else if(presentTime <= 60000000){ //60秒
        color = spikeapi::Light::EColor::YELLOW;
        mLight.turnOnColor(color);
    }
    else if(presentTime <= 90000000){ //90秒    
        color = spikeapi::Light::EColor::ORANGE;
        mLight.turnOnColor(color);
    }
    else{ //90秒以上    
        color = spikeapi::Light::EColor::BLACK;
        mLight.turnOnColor(color);
    }
}


char* BasicTools::robotStateToString(RobotState robotState)
{
    switch(robotState)
    {
        case WAITING:    return "WAITING";
        case LINE_TRACE: return "LINE_TRACE";
        case CURVE:      return "CURVE";
        case GOAL:       return "GOAL";
        default:         return "UNKNOWN";
    }
}

void BasicTools::showMessageState(RobotState robotState,uint32_t on, uint8_t off){//ここの型は変えればいい

    mDisplay.showText(robotStateToString(robotState), on, off);
}

/*
X軸で見た時、90度回転した座標にする
x* = [cosθ -sinθ][x]
y* = [sinθ  cosθ][y]

x* = (x * cosθ) - (y * sinθ)
y* = (x * sinθ) + (y * cosθ)


例：
cos90 = 0
sin90 = 1

x* = -y
y* =  x
になる 
*/

//直近で動いたXY距離を座標に表す(1周期分)   : dummieの方の数値を変えると、元のグローバル変数が変化する
void BasicTools::getDeltaPosition(float &transformedDeltaX_dummie,float &transformedDeltaY_dummie,float &deltaX_dummie,float &deltaY_dummie){
    currentDistance = mDistanceMeter->getCurrentDistance();
    nowRotationAngle = getRotationAngle();
 

    //ラジアンに変換
    nowRotationAngle_Rad = nowRotationAngle * M_PI / 180.0f;


    
    deltaX_dummie = currentDistance * std::cos(nowRotationAngle_Rad); 
    deltaY_dummie = currentDistance * std::sin(nowRotationAngle_Rad);
    transformedDeltaX_dummie = -deltaY_dummie;
    transformedDeltaY_dummie =  deltaX_dummie;


    
    syslog(LOG_NOTICE, "called getDeltaPosition!!!");
    syslog(LOG_NOTICE, "currentDistance = %d",(int)(currentDistance * 10000));
    syslog(LOG_NOTICE, "delta_X_dummie = %d",(int)(deltaX_dummie * 10000));//直近の移動X座標(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "delta_Y_dummie = %d",(int)(deltaY_dummie * 10000));//直近の移動X座標(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "transformedDelta_X_dummie = %d",(int)(transformedDeltaX_dummie * 10000));//変換後の直近の移動X座標(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "transformedDelta_Y_dummie = %d",(int)(transformedDeltaY_dummie* 10000));//変換後の直近の移動X座標(数字が小さいため100倍している) 
    syslog(LOG_NOTICE, "delta_X = %d",(int)(deltaX * 10000));//直近の移動X座標(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "delta_Y = %d",(int)(deltaY * 10000));//直近の移動X座標(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "transformedDelta_X = %d",(int)(transformedDeltaX * 10000));//変換後の直近の移動X座標(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "transformedDelta_Y = %d",(int)(transformedDeltaY * 10000));//変換後の直近の移動X座標(数字が小さいため100倍している) 
    

}



//現在のXT座標を取得する関数(毎周期呼び出しておく)
void BasicTools::getCurrentPosition(){
    getDeltaPosition(transformedDeltaX,transformedDeltaY,deltaX,deltaY);
    currentX += transformedDeltaX;//なぜか、マイナスにしないといけない
    currentY += transformedDeltaY;
    

    //syslog(LOG_NOTICE, "SumDistance_X = %d",(int)currentX); //今までの絶対移動距離
    syslog(LOG_NOTICE, "SumRotationAngle = %d",(int)nowRotationAngle); //現在の絶対角度
    syslog(LOG_NOTICE, "CurrentDistance = %d",(int)(this->currentDistance * 10000)); //直近で動いた距離(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "CurrentRotationAngle = %d",(int)(this->currentRotationAngle * 10000)); //直近で動いた角度(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "CurrentPosition_X = %d",(int)(this->currentX));//現在の絶対X座標(数字が小さいため100倍している)
    syslog(LOG_NOTICE, "CurrentPosition_Y = %d",(int)(this->currentY));//現在の絶対X座標(数字が小さいため100倍している)
    
    
}


float BasicTools::getCurrentPosition_X(){
    return currentX;
}
float BasicTools::getCurrentPosition_Y(){
    return currentY;
}



bool BasicTools::straightRun(float targetDistance){
    
    runDistance += mDistanceMeter -> getCurrentDistance();

    //runDistanceとsumDistanceで差が5もある。なぜ？
    syslog(LOG_NOTICE, "runDistance:%d",(int)runDistance);

    if(runDistance >= targetDistance){ //目標の距離まで走った場合    
        syslog(LOG_NOTICE, "FinishedRunning!");
        return true;
    }
    else
        return false;


}

void BasicTools::runDistanceReset(){
    runDistance = 0;
}





void BasicTools::YourAffection() {
    // 音量を少し大きめにしておく
    mSpeaker.setVolume(volume);
    
    // sleep - playToneの第2引数 が休符になる(sleepを第2引数と近づけたほうが、スタッカートのように聞こえる。)
    // ===== 1回目 ===== 
    //f#,b,b,f#,b,b
    mSpeaker.playTone(740, 500); mClock.sleep(600);
    mSpeaker.playTone(988, 500); mClock.sleep(550);//間は短め
    mSpeaker.playTone(988, 1000); mClock.sleep(1500);

    mSpeaker.playTone(740, 500); mClock.sleep(600);
    mSpeaker.playTone(988, 500); mClock.sleep(550);
    mSpeaker.playTone(988, 800); mClock.sleep(1200);
    
    // D# E F# D# B
    mSpeaker.playTone(1245, 250); mClock.sleep(80);
    mSpeaker.playTone(1319, 250); mClock.sleep(80);
    mSpeaker.playTone(1480, 250); mClock.sleep(80);
    mSpeaker.playTone(1245, 250); mClock.sleep(80);
    mSpeaker.playTone(988, 3500); mClock.sleep(3500);//このあとが3秒くらいあく
    
    
    // F# E D# B G# B D# C# B A# B B
    mSpeaker.playTone(1480, 500); mClock.sleep(600);
    mSpeaker.playTone(1319, 500); mClock.sleep(600);
    mSpeaker.playTone(1245, 250); mClock.sleep(80);
    mSpeaker.playTone(988, 500); mClock.sleep(100);

    mSpeaker.playTone(831, 250); mClock.sleep(80);
    mSpeaker.playTone(988, 250); mClock.sleep(80);
    mSpeaker.playTone(1245, 250); mClock.sleep(80);
    mSpeaker.playTone(1109, 250); mClock.sleep(80);
    mSpeaker.playTone(988, 250); mClock.sleep(300);//ここで、一瞬区切りができる

    mSpeaker.playTone(932, 250); mClock.sleep(600);//この後区切り
    mSpeaker.playTone(988, 550); mClock.sleep(750);
    mSpeaker.playTone(988, 3000); mClock.sleep(3300);
    

    // ===== 2回目 =====
    mSpeaker.playTone(740, 500); mClock.sleep(600);
    mSpeaker.playTone(988, 500); mClock.sleep(550);//間は短め
    mSpeaker.playTone(988, 1000); mClock.sleep(1500);

    mSpeaker.playTone(740, 500); mClock.sleep(600);
    mSpeaker.playTone(988, 500); mClock.sleep(550);
    mSpeaker.playTone(988, 1000); mClock.sleep(1500);

    mSpeaker.playTone(740, 500); mClock.sleep(600);
    mSpeaker.playTone(988, 500); mClock.sleep(550);
    mSpeaker.playTone(988, 3500); mClock.sleep(3500);

    // F# E D# B G# D# C# B A# B B
    mSpeaker.playTone(1480, 500); mClock.sleep(600);
    mSpeaker.playTone(1319, 500); mClock.sleep(600);
    mSpeaker.playTone(1245, 250); mClock.sleep(80);
    mSpeaker.playTone(988, 500); mClock.sleep(100);

    mSpeaker.playTone(831, 250); mClock.sleep(80);
    mSpeaker.playTone(988, 250); mClock.sleep(80);
    mSpeaker.playTone(1245, 250); mClock.sleep(80);
    mSpeaker.playTone(1109, 250); mClock.sleep(80);
    mSpeaker.playTone(988, 250); mClock.sleep(300);//ここで、一瞬区切りができる

    mSpeaker.playTone(932, 250); mClock.sleep(600);//この後区切り
    mSpeaker.playTone(988, 550); mClock.sleep(750);
    mSpeaker.playTone(988, 4000); mClock.sleep(4000);

    // ワンオクターブ下のEを伸ばす
    mSpeaker.playTone(659, 3000);
    
}

void BasicTools::Guild(){
    // 音量を少し大きめにしておく
    mSpeaker.setVolume(10);

    // --- メロディ：「にんげんという（A# B C# D# C# B A#）」 ---
    mSpeaker.playTone(466, 330); mClock.sleep(360); // に (A#4)
    mSpeaker.playTone(494, 330); mClock.sleep(360); // ん (B4)
    mSpeaker.playTone(554, 330); mClock.sleep(360); // げ (C#5)
    mSpeaker.playTone(622, 330); mClock.sleep(360); // ん (D#5)
    mSpeaker.playTone(554, 330); mClock.sleep(360); // と (C#5)
    mSpeaker.playTone(494, 330); mClock.sleep(360); // い (B4)
    mSpeaker.playTone(466, 690); mClock.sleep(720); // うー (A#4)

    mClock.sleep(360); // （一瞬お休み）

    // --- メロディ：「しごとをあたえられて（A# B C# D# C# B A# G#）」 ---
    mSpeaker.playTone(466, 330); mClock.sleep(360); // し (A#4)
    mSpeaker.playTone(494, 330); mClock.sleep(360); // ご (B4)
    mSpeaker.playTone(554, 330); mClock.sleep(360); // と (C#5)
    mSpeaker.playTone(622, 330); mClock.sleep(360); // を (D#5)
    mSpeaker.playTone(554, 330); mClock.sleep(360); // あ (C#5)
    mSpeaker.playTone(494, 330); mClock.sleep(360); // た (B4)
    mSpeaker.playTone(466, 330); mClock.sleep(360); // え (A#4)
    mSpeaker.playTone(415, 690); mClock.sleep(720); // らー (G#4)
    mSpeaker.playTone(415, 330); mClock.sleep(360); // れ (G#4)
    mSpeaker.playTone(466, 690); mClock.sleep(720); // てー (A#4)


}

void BasicTools::OneTimePlaySound() {
    presentTime = mClock.now(); // 現在の時間を取得
    

    //再生から一定時間経った場合、もう一度再生する
    if(firstPlay) { //初回再生の場合
        lastPlayTime = presentTime; // 現在時刻をlastPlayTimeに設定
        firstPlay = false; // 初回再生フラグをfalseにする
        hasPlayed = true; // 音楽が再生されたことを記録
        YourAffection(); // メロディを演奏
        
    }
    if ((presentTime - lastPlayTime) >= playSongInterval) {
        lastPlayTime = presentTime;
        YourAffection();
        
    } //変数が数字の場合と文字の場合で出力できるようにする
}
/*

2

C	131
C#	139
D	147
D#	156
E	165
F	175
F#	184
G	196
G#	208
A	220
A#	233
B	247

3

C	262
C#	277
D	294
D#	311
E	329
F	349
F#  368
G   391
G#  415
A	440
A#  466
B	494

4
C (高い)	523
C#  554
D   587
D#  622
E   659
F   698
F#  740
G   784
G#  831
A   880
A#  932
B   988

オクターブ
A1 110
A2 220
A3 440
*/