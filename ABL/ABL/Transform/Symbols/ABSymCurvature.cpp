//
//  ABSymCurvature.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymCurvature.h"

ABSymCurvature::ABSymCurvature(string name, string input, int dimension, string time)
: ABSymbol(name, 1), incard(dimension)
{
    vector<string> invect;
    invect.push_back(input);
    invect.push_back(time);
    
    setInputs(invect);
    
    lastp = new double[incard];
    lastv = new double[incard];
    lastt = 0.0;
    
    for (int i = 0; i < incard; i++)
        lastp[i] = lastv[i] = 0.0;
}

ABSymCurvature::~ABSymCurvature()
{
    delete lastp;
    delete lastv;
}

void ABSymCurvature::recalculate()
{
    double t = 0, dt = 0, idt = 1;
    if (inputSyms[timeIdx]) {
        t = inputSyms[timeIdx]->getValue(0);
        dt = t - lastt;
        idt = 1/dt;
    }
    
    // Get current position
    double x[incard];
    if (inputSyms[0]) {
        inputSyms[0]->getValues(x);
    }
    else {
        setValue(0.0, 0);
        return;
    }
    
    // Get current velocity, acceleration, and three dot products
    double v[incard], a[incard];
    double vvdot = 0.0, avdot = 0.0;
    for (int i = 0; i < incard; i++) {
        v[i] = (x[i] - lastp[i]) * idt;
        a[i] = (v[i] - lastv[i]) * idt;
        
        vvdot += lastv[i]*lastv[i];
        avdot += a[i]*lastv[i];
    }

    // Create the e2 vector--a - proj(a, v)
    double e2[incard], e2dot = 0.0;
    for (int i = 0; i < incard; i++) {
        e2[i] = a[i] - avdot * lastv[i] / vvdot;
        e2dot += e2[i]*e2[i];
    }

    // We don't want to divide by zero... we'll get zero curvature anyway
    if (vvdot == 0.0 || e2dot == 0.0) {
        setValue(0.0, 0);
        
    } else {

        // Dot e1dt with e2
        double kdot = 0.0;
        for (int i = 0; i < incard; i++)
            kdot += a[i] * e2[i];
        kdot /= (vvdot * sqrt(e2dot));

        // Calculate actual curvature!
        double K = kdot;

        setValue(K, 0);
    }
    
    memcpy(lastp, x, incard * sizeof(double));
    memcpy(lastv, v, incard * sizeof(double));
    lastt = t;
}