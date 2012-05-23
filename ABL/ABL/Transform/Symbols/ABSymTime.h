//
//  ABSymTime.h
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymTime_h
#define ABL_ABSymTime_h

#include "ABSymbol.h"
#include "GTimer.h"
#include "GCacheQueue.h"

/*
 * Symbol designed to automatically update over time
 */
class ABSymTime : public ABSymbol {
private:
    GCacheQueue que;
    GTimer *timer;
    int smoothRadius;
    
public:
    ABSymTime(string name, GTimer *atimer, int smoothRadius);
    ~ABSymTime();
    
    virtual void recalculate();
};

#endif
