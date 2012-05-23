//
//  ABSymDifferentiate.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/21/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymDifferentiate.h"

ABSymDifferentiate::ABSymDifferentiate(string name, vector<string> &inputs, string timeNode)
: ABSymbol(name)
{
    // Verify that "time" is in inputs (otherwise it won't link correctly)
    if (find(inputs.begin(), inputs.end(), timeNode) == inputs.end()) {
        inputs.push_back(timeNode);
    } else {
        // Next, verify that "time" is the last input and switch if not
        timeIdx = find(inputs.begin(), inputs.end(), timeNode) - inputs.begin();
        if (timeIdx != inputs.size()-1)
            swap(inputs[timeIdx], inputs[inputs.size()-1]);
    }
    
    setCard(inputs.size()-1);
    setInputs(inputs);
    
    timeIdx = inputs.size()-1;
    
    lastVals = new double[getCard()];
    
    lastTime = 0;
}

ABSymDifferentiate::~ABSymDifferentiate()
{
    delete[] lastVals;
}

void ABSymDifferentiate::recalculate()
{
    double t = (inputSyms[timeIdx] ? inputSyms[timeIdx]->getValue(0) : -1.0);
    double x[getCard()];
    for (int i = 0; i < getCard(); i++)
        x[i] = (inputSyms[i] ? inputSyms[i]->getValue(0) : 0.0);
    
    double idt = 1.0/(t - lastTime);
    lastTime = t;
    
    // Calculate derivatives
    double dx0[getCard()];
    getValues(dx0);
    
    double dx[getCard()];
    for (int i = 0; i < getCard(); i++)
        dx[i] = ((x[i]-lastVals[i])*idt + dx0[i])*.5;
    memcpy(lastVals, x, sizeof(double)*getCard());
    
    setValues(dx);
    
    dataState = DIRTY;
}