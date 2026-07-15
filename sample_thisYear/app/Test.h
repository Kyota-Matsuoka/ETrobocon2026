#ifndef ETTR_APP_TEST_H_
#define ETTR_APP_TEST_H_

#include "LineMonitor.h"
#include "Walker.h"

class Test {
public:
	static const float Kp;
	static const int   bias;
	
	Test(LineMonitor* lineMonitor, Walker* walker);

    void run();

private:
    LineMonitor* mLineMonitor;
    Walker* mWalker;
    bool mIsInitialized;

    float calcPropValue(int diffReflection);
};

#endif  // ETTR_APP_TEST_H_
