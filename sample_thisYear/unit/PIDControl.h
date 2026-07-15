#ifndef ETTR_UNIT_PIDCONTROL_H_
#define ETTR_UNIT_PIDCONTROL_H_//二重定義を防止するため

//#include "LineTracer.h"



class PIDControl{
public:
		static const float Kp;
		static const int   bias;
		float calcPropValue(float diffBrightness);
		/*
		float doubleLoop_calcPropValue(float diffBrightness);
		float gate_calcPropValue(float diffBrightness);
		float put_calcPropValue(float diffBrightness);//diffが入る
		float bottle_calcPropValue(float diffBrightness);//diffが入る
		float gate_calcPropValue_far(float diffBrightness);
		float bottle_calcPropValue_far(float diffBrightness);
        float put2_calcPropValue(float diffBrightness);
		float BcalcPropValue(float diffBrightness);
		float toGoalcalcPropValue(float diffBrightness);
		
		float calcPropValue_fast(float diffBrightness);		
		float calcPropValue_so_fast(float diffBrightness);
		*/

		//追加
		static const float  INITIAL_PREVOUSDIFF_;
		static const float  INITIAL_DERIVATIVE_;
		static const float  INITIAL_SUMDIFF_;
		PIDControl();
		
	
		
private:
		//LineTracer* lineTracer;
		float previousDiff;
		float derivative;
		float sumDiff;
		
		//追加
		float calcPropValue_p;
		float calcPropValue_i;
		float calcPropValue_d;
		/*
		float doubleLoop_calcPropValue_p;
		float doubleLoop_calcPropValue_i;
		float doubleLoop_calcPropValue_d;
		float put_calcPropValue_p;
		float put_calcPropValue_i;
		float put_calcPropValue_d;
		float gate_calcPropValue_p;
		float gate_calcPropValue_i;
		float gate_calcPropValue_d;
		float bottle_calcPropValue_p;
		float bottle_calcPropValue_i;
		float bottle_calcPropValue_d;
		
		float put2_calcPropValue_p;
		float put2_calcPropValue_i;
		float put2_calcPropValue_d;
			 
         
        float bottle_calcPropValue_p_far;
        float bottle_calcPropValue_i_far;
        float bottle_calcPropValue_d_far;
		 
        float gate_calcPropValue_p_far;
        float gate_calcPropValue_i_far;
		float gate_calcPropValue_d_far;
		
         float toGoalcalcPropValue_p;
         float toGoalcalcPropValue_i;
         float toGoalcalcPropValue_d;


		 
         float BcalcPropValue_p;
         float BcalcPropValue_i;
         float BcalcPropValue_d;
		
		*/
};








#endif //ETTR_UNIT_PIDCONTROL_H_
