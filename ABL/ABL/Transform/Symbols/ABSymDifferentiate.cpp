//
//  ABSymDifferentiate.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/21/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymDifferentiate.h"

ABSymDifferentiate::ABSymDifferentiate(string name, int card, string input, string timeNode)
: ABSymbol(name, card)
{
    // Verify that "time" is in inputs (otherwise it won't link correctly)
    vector<string> names;
    names.push_back(input);
    names.push_back(timeNode);
    
    setInputs(names);
    
    timeIdx = 1;
    
    lastVals = new double[getCard()];
    for (int i = 0; i < getCard(); i++)
        lastVals[i] = 0.0;
    
    setValues(lastVals);
    
    lastTime = 0;
}

ABSymDifferentiate::~ABSymDifferentiate()
{
    delete[] lastVals;
}

void ABSymDifferentiate::recalculate()
{
    double t = inputSyms[timeIdx]->getValue(0);
    
    // If t is too close, peace out
    static double EPS = 1e-6;
    if (fabs(t - lastTime) < EPS) return;
    
    // Calculate inverse
    double idt = 1.0/(t - lastTime);
    lastTime = t;
    
    // Grab current values
    double x[getCard()];
    inputSyms[0]->getValues(x);
    
    // Grab differences
    double dx[getCard()];
    for (int i = 0; i < getCard(); i++)
        dx[i] = (x[i] - lastVals[i])*idt;
    memcpy(lastVals, x, sizeof(double)*getCard());
    
    setValues(dx);
}