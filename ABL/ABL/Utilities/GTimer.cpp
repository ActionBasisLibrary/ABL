//
//  GTimer.cpp
//  GMath
//
//  Created by Caleb Jordan on 11/29/11.
//  Copyright 2011 Stanford University. All rights reserved.
//

#include "GTimer.h"
#include <mach/mach_time.h>

GTimer::GTimer()
{	
	mach_timebase_info_data_t info = {0,0};
	mach_timebase_info(&info);
	scale = 1e-9 * (info.numer / info.denom); 
	
	lastTime = firstTime = absTime();
}

double GTimer::secondsPassed()
{
	double newTime = absTime();
	double result = newTime - lastTime;
	lastTime = newTime;
	return result;
}

double GTimer::getTime()
{
	double result = absTime() - firstTime;
	return result;
}

void GTimer::resetTime()
{
	firstTime = absTime();
}

double GTimer::absTime()
{
	return mach_absolute_time() * scale;
}