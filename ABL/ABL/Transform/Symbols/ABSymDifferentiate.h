//
//  ABSymDifferentiate.h
//  ABL
//
//  Created by Caleb Jordan on 5/21/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymDifferentiate_cpp
#define ABL_ABSymDifferentiate_cpp

#include "ABSymTick.h"
#include "GCacheQueue.h"
#include "GTimer.h"

#include <string>

class ABSymDifferentiate : public ABSymbol {
private:
    int timeIdx;
    double lastTime;
    double *lastVals;
    
public:
    ABSymDifferentiate(string name, vector<string> &inputs, string timeNode);
    ~ABSymDifferentiate();
    
    virtual void recalculate();
};

#endif
