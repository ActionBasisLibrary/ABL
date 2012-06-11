//
//  ABSymIntegrate.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymIntegrate.h"

ABSymIntegrate::ABSymIntegrate(string name, int card, string input, string time)
: ABSymbol(name, card)
{
    vector<string> names;
    names.push_back(input);
    names.push_back(time);
    setInputs(names);
}

void ABSymIntegrate::recalculate()
{
    double t = inputSyms[1]->getValue(0);
    double dt = t - lasttime;
    if (t == lasttime) return;
    lasttime = t;
    
    double buff[getCard()];
    inputSyms[0]->getValues(buff);
    
    double vals[getCard()];
    getValues(vals);
    
    for (int i = 0; i < getCard(); i++)
        vals[i] += buff[i] * dt;
    
    setValues(vals);
}