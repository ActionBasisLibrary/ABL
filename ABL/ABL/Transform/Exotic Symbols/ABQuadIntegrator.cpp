//
//  ABQuadIntegrator.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABQuadIntegrator.h"

ABQuadIntegrator::ABQuadIntegrator(string name, string input, string time, double ctr[2])
: ABSymbol(name, 5)
{
    vector<string> names;
    names.push_back(input);
    names.push_back(time);
    
    center[0] = ctr[0];
    center[1] = ctr[1];
    
    lasttime = 0.0;
    for (int i = 0; i < getCard(); i++)
        setValue(0.0, i);
}

void ABQuadIntegrator::recalculate()
{
    double t = inputSyms[1]->getValue(0);
    double dt = t - lasttime;
    if (t == lasttime) return;
    lasttime = t;
    
    double x[2];
    inputSyms[0]->getValues(x);
    x[0] -= center[0];
    x[1] -= center[1];
    
    // 1 or 2
    if (x[1] > 0) {
        // 1
        if (x[0] > 0)
            setValue(getValue(0) + dt, 0);
        // 2
        else
            setValue(getValue(1) + dt, 1);
    }
    
    // 3 or 4
    else {
        // 3
        if (x[0] < 0)
            setValue(getValue(2) + dt, 2);
        // 4
        else
            setValue(getValue(3) + dt, 3);
    }
    
    setValue(getValue(4) + dt, 4);
}