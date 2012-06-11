//
//  ABCountIntegrate.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABCountIntegrate.h"


ABCountIntegrate::ABCountIntegrate(string name, string input, string time)
: ABSymbol(name, 1)
{
    vector<string> names;
    names.push_back(input);
    names.push_back(time);
    
    setInputs(names);
    
    lasttime = 0.0;
    setValue(0.0, 0);
}

void ABCountIntegrate::recalculate()
{
    double t = inputSyms[1]->getValue(0);
    double dt = t - lasttime;
    if (t == lasttime) return;
    lasttime = t;
    
    int val = (int)inputSyms[0]->getCard();
    
    if (val == 0) {
        
        setValue(0.0, 0);
        
    } else {
        
        double val0 = getValue(0);
        
        setValue(val0 + val * dt, 0);
        
    }
}