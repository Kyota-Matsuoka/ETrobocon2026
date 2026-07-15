//D(微分)制御関数:前回のずれと今回のズレの変化量、閾値で大きいか判断。pとdの比重の設定
//drivative=currentDiffBrightness-prevousDiffBrightness
//calcPropoにderivative引数として
#include "PIDControl.h"
#include "LineTracer.h"
#include "stdio.h"

// 定数宣言
const float PIDControl::Kp = 0.3;
const int 	PIDControl::bias = 0;//走る位置の制御



//追加
const float   PIDControl::INITIAL_DERIVATIVE_ = 0.0;//変化率の初期化
const float   PIDControl::INITIAL_PREVOUSDIFF_=0.0;
const float   PIDControl::INITIAL_SUMDIFF_=0.0;

/**
 * コンストラクタ
 * 
 * 
 * @param lineMonitor     ライン判定
 * @param walker 走行
 */
//追加
PIDControl::PIDControl()
        :previousDiff(PIDControl::INITIAL_PREVOUSDIFF_),
         derivative(PIDControl::INITIAL_DERIVATIVE_),
         sumDiff(PIDControl::INITIAL_SUMDIFF_),
         calcPropValue_p(1.2),
         calcPropValue_i(0.0),
         calcPropValue_d(0.8)
         /*
         doubleLoop_calcPropValue_p(0.3),
	       doubleLoop_calcPropValue_i(0.0),
		     doubleLoop_calcPropValue_d(1.7),
         gate_calcPropValue_p(1.3),
         gate_calcPropValue_i(0.0),
		     gate_calcPropValue_d(1.0),
         
         BcalcPropValue_p(1.0),
         BcalcPropValue_i(0.0),
         BcalcPropValue_d(2.5),
         
         //追加
         gate_calcPropValue_p_far(0.7),
         gate_calcPropValue_i_far(0.0),
		     gate_calcPropValue_d_far(0.5),
		     put_calcPropValue_p(1.4),
		     put_calcPropValue_i(0.0),
		     put_calcPropValue_d(3.0),
         //追加
         
		     put2_calcPropValue_p(1.0),
		     put2_calcPropValue_i(0.0),
		     put2_calcPropValue_d(1.3),

         bottle_calcPropValue_p(0.5),
         bottle_calcPropValue_i(0.0),
         bottle_calcPropValue_d(0.8),
         
         //追加
         bottle_calcPropValue_p_far(0.7),
         bottle_calcPropValue_i_far(0.0),
         bottle_calcPropValue_d_far(0.8),
         
         toGoalcalcPropValue_p(0.3),
         toGoalcalcPropValue_i(0.0),
         toGoalcalcPropValue_d(0.7)
         */{
       }



/**
 * 走行体の操作量を計算する
 * @param diffBrightness ラインから外れた度合い（ライン閾値との差）
 */
 //pid seigyo no p
float PIDControl::calcPropValue(float diffBrightness) {//diffが入る

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * calcPropValue_p) + ((PIDControl::Kp * -derivative) * calcPropValue_d) + ((PIDControl::Kp * sumDiff) * calcPropValue_i) + PIDControl::bias;
	
    return turn;
}

/*
float PIDControl::doubleLoop_calcPropValue(float diffBrightness) {//diffが入る

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * doubleLoop_calcPropValue_p) + ((PIDControl::Kp * -derivative) * doubleLoop_calcPropValue_d) + ((PIDControl::Kp * sumDiff) * doubleLoop_calcPropValue_i) + PIDControl::bias;
	
    return turn;
}
//D(微分)制御関数:前回のずれと今回のズレの変化量、閾値で大きいか判断。pとdの比重の設定
//drivative=currentDiffBrightness-prevousDiffBrightness
//calcPropoにderivative引数として

float PIDControl::gate_calcPropValue(float diffBrightness) {//diffが入る

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * gate_calcPropValue_p) + ((PIDControl::Kp * -derivative) * gate_calcPropValue_d) + ((PIDControl::Kp * sumDiff) * gate_calcPropValue_i) + PIDControl::bias;
	
    return turn;
}


//追加
float PIDControl::put_calcPropValue(float diffBrightness) {//diffが入る

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * put_calcPropValue_p) + ((PIDControl::Kp * -derivative) * put_calcPropValue_d) + ((PIDControl::Kp * sumDiff) * put_calcPropValue_i) + PIDControl::bias;
	
    return turn;
}

float PIDControl::bottle_calcPropValue(float diffBrightness) {//diffが入る

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * bottle_calcPropValue_d) + ((PIDControl::Kp * sumDiff) * bottle_calcPropValue_i) + PIDControl::bias;
	
    return turn;
}


float PIDControl::gate_calcPropValue_far(float diffBrightness){


  //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * gate_calcPropValue_p_far) + ((PIDControl::Kp * -derivative) * gate_calcPropValue_d_far) + ((PIDControl::Kp * sumDiff) * gate_calcPropValue_i_far) + PIDControl::bias;
	
    return turn;
}


float PIDControl::put2_calcPropValue(float diffBrightness){
  //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * put2_calcPropValue_p) + ((PIDControl::Kp * -derivative) * put2_calcPropValue_d) + ((PIDControl::Kp * sumDiff) * put2_calcPropValue_i) + PIDControl::bias;
	
    return turn;
}



float PIDControl::bottle_calcPropValue_far(float diffBrightness) {//diffが入る

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * bottle_calcPropValue_p_far) + ((PIDControl::Kp * diffBrightness) * bottle_calcPropValue_d_far) + ((PIDControl::Kp * sumDiff) * bottle_calcPropValue_i_far) + PIDControl::bias;
	
    return turn;
}

float PIDControl::toGoalcalcPropValue(float diffBrightness){
        //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * toGoalcalcPropValue_p) + ((PIDControl::Kp * diffBrightness) * toGoalcalcPropValue_d) + ((PIDControl::Kp * sumDiff) * toGoalcalcPropValue_i) + PIDControl::bias;
	
    return turn;
}



float PIDControl::calcPropValue_fast(float diffBrightness) {//run_fast() to run_8 younoPIDseigyotikeisann

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * 2.2) + ((PIDControl::Kp * derivative) * 2.7) + ((PIDControl::Kp * sumDiff) * 0.002) + PIDControl::bias;
	
    return turn;
}



float PIDControl::calcPropValue_so_fast(float diffBrightness) {//run_so_fast用のPID制御値計算

    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * 1.24) + ((PIDControl::Kp * derivative) * 2.5) + ((PIDControl::Kp * sumDiff) * 0.002) + PIDControl::bias;
	//pwm:60                                         1.3か1.4                              3.0か3.0より上か                         0.003
    return turn;
}



float PIDControl::BcalcPropValue(float diffBrightness){
    //追加：微分計算:D制御
    float currentDiff = diffBrightness;
    derivative = currentDiff-previousDiff;
    previousDiff = currentDiff;
    printf("derivative:%.2f\n",derivative);

    //追加:積分計算:I制御:diffかderivativeどっちがいいか
    if(derivative > 5){//変化率が大きい場合だけ加算
      sumDiff = sumDiff - derivative * 0.1;
    }
    else if(derivative < -5){
      sumDiff = sumDiff + derivative * 0.1;
    }
    
    printf("sumDiff:%.2f\n",sumDiff);


    //PID制御計算
    float turn =((PIDControl::Kp * diffBrightness) * BcalcPropValue_p) + ((PIDControl::Kp * diffBrightness) * BcalcPropValue_d) + ((PIDControl::Kp * sumDiff) * BcalcPropValue_i) + PIDControl::bias;
	
    return turn;
}
*/
