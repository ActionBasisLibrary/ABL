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

class GTimer {
public:
	GTimer();
	~GTimer() {}
	
	double secondsPassed();
	double getTime();
	void resetTime();
	double absTime();
private:
	double scale, lastTime, firstTime;
};

#endif