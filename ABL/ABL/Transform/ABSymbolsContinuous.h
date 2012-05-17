//
//  ABSymbolsContinuous.h
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymbolsContinuous_h
#define ABL_ABSymbolsContinuous_h

#include <string>
#include "ABSymbol.cpp"
#include "GTimerThread.h"

using namespace std;

/*
 * Symbol designed to automatically update over time
 */
class ABSymTick : public ABSymbol {
private:
    double refresh;
    GTimerThread *timer;
    
public:
    ABSymTick(string name, unsigned int card, vector<string> &inputs,
              GTimerThread *atimer, double refreshTime);
    
    virtual void tick();
    
    friend void tickUpdate(void *sym);
};

/*
 * Symbol that simply reports time
 */

#endif
