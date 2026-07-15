#include "kernel_cfg.h"
#include "sil.h"
//#include <spike/pup.h> //ボタン用ライブラリ
//#include "spike/device.h"
#include "app.h"
#include <stdio.h>
#include <t_syslog.h>
#include "syssvc/syslog.h"

// EV3APIやボタン関連
//#include "ev3api.h" 

// 各自作成したクラスのヘッダ（パスは環境に合わせて調整してください）
#include "Walker.h"
#include "LineMonitor.h"
#include "Starter.h"
#include "LineTracer.h"
#include "ScenarioTracer.h"
#include "PIDControl.h"
#include "DistanceMeter.h"
#include "Light.h"
#include "Button.h"
#include "UltrasonicSensor.h"
#include "Display.h"

//APIをインクルードする
#include <spike/pup/forcesensor.h>
#include <libcpp/spike/Motor.h> 
#include <libcpp/spike/Clock.h>

// using宣言
using namespace spikeapi;


/* センサーポートの定義 */
static const pbio_port_id_t
  color_sensor_port    = PBIO_PORT_ID_E,
  left_motor_port      = PBIO_PORT_ID_B,
  right_motor_port     = PBIO_PORT_ID_A,
  force_sensor_port    = PBIO_PORT_ID_D,
  distance_sensor_port = PBIO_PORT_ID_C;//距離センサ


// 例：SPIKEのポート配置に合わせて定義（ポート等はご自身の環境に合わせてください）
//グローバルで実体化する
spikeapi::Motor gLeftWheel(EPort::PORT_B, spikeapi::Motor::EDirection::COUNTERCLOCKWISE, true); // <1>
spikeapi::Motor gRightWheel(EPort::PORT_A, spikeapi::Motor::EDirection::CLOCKWISE, true);  // <2>
spikeapi::ForceSensor gForceSensor(EPort::PORT_D);
spikeapi::ColorSensor gColorSensor(EPort::PORT_E);
spikeapi::UltrasonicSensor gUltrasonicSensor(EPort::PORT_C); //距離センサ
//ButtonとLightをグローバルで実体化する
//spikeapi::Button gButton;
spikeapi::Light gLight;
Clock gClock;
/*
static spikeapi::ForceSensor *gForceSensor(spikeapi::EPort::PORT_A);
static spikeapi::ColorSensor *gColorSensor(spikeapi::EPort::PORT_B);
static spikeapi::Motor       *gLeftWheel(spikeapi::EPort::PORT_C, spikeapi::Motor::EDirection::COUNTERCLOCKWISE, true);
static spikeapi::Motor       *gRightWheel(spikeapi::EPort::PORT_D, spikeapi::Motor::EDirection::CLOCKWISE, true);
*/

//ポインタを使用し、グローバルで参照できるようにする(システム生成関数で実体化)
/*
static spikeapi::Motor       *gLeftWheel;
static spikeapi::Motor       *gRightWheel;
static spikeapi::ColorSensor *gColorSensor;
static spikeapi::ForceSensor *gForceSensor;
*/

// Logicオブジェクトのポインタ（静的確保を推奨するが、動的生成も可能）
static Walker          *gWalker;
static LineMonitor     *gLineMonitor;
static Starter         *gStarter;
static LineTracer      *gLineTracer;
static ScenarioTracer  *gScenarioTracer;
static PIDControl      *gPIDControl;
static DistanceMeter   *gDistanceMeter;

/* システム生成ルーチン */
static void user_system_create() {
    // 旧世代のtslp_tsk(2ms)は、単なる待機であればdly_tskが適しています
    dly_tsk(2000U);  //[1]はタグ(ID)を意味する　→ 2msの待機


    // C++オブジェクトの動的生成
    /*
    gLeftWheel = new spikeapi::Motor(spikeapi::EPort::PORT_C, spikeapi::Motor::EDirection::COUNTERCLOCKWISE, true);
    gRightWheel = new spikeapi::Motor(spikeapi::EPort::PORT_D, spikeapi::Motor::EDirection::CLOCKWISE, true);
    gColorSensor = new spikeapi::ColorSensor(spikeapi::EPort::PORT_B);
    gForceSensor = new spikeapi::ForceSensor(spikeapi::EPort::PORT_A);
    */

    gWalker          = new Walker(gLeftWheel, gRightWheel);
    gStarter         = new Starter(gForceSensor);
    gLineMonitor     = new LineMonitor(gColorSensor);
    gPIDControl      = new PIDControl();
    gDistanceMeter   = new DistanceMeter(gLeftWheel, gRightWheel);
    gLineTracer      = new LineTracer(gLineMonitor, gWalker, gDistanceMeter, gPIDControl);
    gScenarioTracer  = new ScenarioTracer(gStarter, gWalker, gLineTracer,gDistanceMeter);


}

/* システム破棄ルーチン */
static void user_system_destroy() {
    gLeftWheel.stop();
    gRightWheel.stop();

    delete gScenarioTracer;
    delete gLineTracer;
    delete gLineMonitor;
    delete gStarter;
    delete gWalker;
    delete gPIDControl;
    delete gDistanceMeter;
}



/*
 メインタスク(起動時にのみ関数コールされる) 
void main_task(intptr_t unused) {
  printf("+---------------------------------+\n");
  printf("|   Press force sensor to start   |\n");
  printf("+---------------------------------+\n");
  #フォースセンサーが押下されるまで待機 
  pup_device_t *force_sensor = pup_force_sensor_get_device(force_sensor_port);
  while (!pup_force_sensor_touched(force_sensor)) {
    dly_tsk(10*1000);
  }

  #LineTracerに構成を渡す : LineTracer.cppファイルのLineTracer_Configure関数 
  LineTracer_Configure(left_motor_port,right_motor_port,color_sensor_port);
  printf("Start Line Trace!!\n");
    
  #ライントレースタスクの起動 
  sta_cyc(LINE_TRACER_TASK_CYC);

  #タスク終了 
  ext_tsk();
}
*/



/* メインタスク */
void main_task(intptr_t unused) {
    /* ログが流れるか確認するコード
    while (true) {
        printf("HELLO\n");

        syslog(LOG_NOTICE, "System Created");
        tslp_tsk(1000);   // 1秒待つ
    }
    */

    // 1. オブジェクトの生成
    user_system_create();
    printf("System Created\n");
    /*
    // 2. 実行開始のトリガー待機（例：ボタンが押されるまで）
    // 現世代ではループ内でデバイス状態を確認し、dly_tskで待つのが一般的です
    printf("Press Button to Start\n");
    // SPIKE-RT用の関数（pup_button_is_pressed）と定数（PB_BUTTON_CENTER）に差し替え
    while (!pup_button_is_pressed(PB_BUTTON_CENTER)) {
        dly_tsk(10 * 1000U); // 10ms待機。これはカーネル標準なのでOK
    }
    */

    // 3. 周期通知の開始  : app.cfgのCRE_CYCで定義されたCYC_TRACER → TRACER_TASKをCRE_TSKで呼び出し → CRE_TSKからtracer_taskが呼び出される
    sta_cyc(CYC_TRACER);  //[2]はタグ(ID)を意味する　→ 周期通知の開始
    printf("Tracer Started\n");
    slp_tsk();  // メインタスクをスリープさせる（tracer_taskが起動するまで待機）
    /*
    // 4. 終了条件の待機
    // SPIKEの左ボタンなどを終了条件にする例
    while (!pup_button_is_pressed(PB_BUTTON_LEFT)) {
        dly_tsk(100 * 1000U); // 100ms待機（カーネルサービスコール） [1]
    }
    */

    // 5. 周期通知の停止
    stp_cyc(CYC_TRACER);  //[2]はタグ(ID)を意味する　→ 周期通知の停止

    // 6. オブジェクトの破棄
    user_system_destroy();

    // 7. 自タスクの終了（第3世代では明示的な呼び出しが推奨されます）
    ext_tsk(); 
  }


void tracer_task(intptr_t unused) {
Button button;

    printf("ライントレ―ス中\n");

	  if(button.isLeftPressed()) {
	    wup_tsk(MAIN_TASK);  // レフトボタン押下
      printf("Left Button Pressed\n");
    }
    else {
        //ここでモーターが動き出す
        gScenarioTracer->run();  // 走行

        int distance = gUltrasonicSensor.getDistance();
        
        syslog(LOG_NOTICE, "Distance: %d mm", distance);


    }

    ext_tsk();
} 