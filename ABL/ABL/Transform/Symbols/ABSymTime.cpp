//
//  ABSymTime.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymTime.h"

const static int MAX_DERIV = 2;

ABSymTime::ABSymTime(string name, GTimer *atimer)
: ABSymbol(name, MAX_DERIV+1), timer(atimer),
 que(MAX_DERIV+1)
{
    recalculate();
}

ABSymTime::~ABSymTime()
{
    
}

void ABSymTime::recalculate()
{
    // 0 deriv
    double t = timer->getTime();

    setValue(t, 0);
    que.shift(t);
    
    // Calculate derivatives
    int c = que.getCard();
    double d[c];
    que.getOrderedValues(d);
    
    for (int i = 1; i < MAX_DERIV+1; i++) {     

        for (int j = 0; j < c-i; j++)
            d[j] = d[j]-d[j+1];
        
        setValue(d[0], i); 
    }
    
    dataState = DIRTY;
}