//
//  GTimer.h
//  GMath
//
//  Created by Caleb Jordan on 11/29/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#ifndef GTimer_h
#define GTimer_h

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

class GTimer {
public:
	GTimer();
	~GTimer() {}
	
	double secondsPassed();
	double getTime();
	void resetTime();
	double absTime();
    
    timeval getSysTime();
    
private:
	double scale, lastTime, firstTime;
};

#endif