//
//  ABSymbolsContinuous.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymbolsContinuous.h"

#pragma mark TICK SYMBOL--NECESSARY TO UPDATE CONTINUOUS REGULARLY

ABSymTick::ABSymTick(string name, unsigned int card, vector<string> &inputs,
                     GTimerThread *atimer, double refreshTime)
: ABSymbol(name, card, inputs), timer(atimer), refresh(refreshTime)
{
    dataState = DIRTY;
}

void ABSymTick::tick()
{
    
}

void tickUpdate(void *vsym)
{
    // Get the symbol pointer
    ABSymTick *sym = (ABSymTick*)vsym;
    
    // Call the symbols tick function
    sym->tick();
    
    // Register a new tick event
    sym->timer->tick(tickUpdate, sym, sym->refresh);
}