//
//  ABSymCurveCurvature.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymCurveCurvature.h"
#include "ABSymCurve.h"

#define EPSILON 0.000001

ABSymCurveCurvature::ABSymCurveCurvature(string name, string input, int dimension, string time)
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

ABSymCurveCurvature::~ABSymCurveCurvature()
{
    delete lastp;
    delete lastv;
}

void ABSymCurveCurvature::recalculate()
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
        ((ABSymCurve*)inputSyms[0])->getValues(x, t-0.1);
    }
    else {
        setValue(0.0, 0);
        return;
    }
    
    // Get current velocity, acceleration, and three dot products
    double v[incard], a[incard];
	((ABSymCurve*)inputSyms[0])->getVelocity(v, t-0.1);
	((ABSymCurve*)inputSyms[0])->getAcceleration(a, t-0.1);
	
    double vvdot = 0.0, avdot = 0.0;
    for (int i = 0; i < incard; i++) {
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
    if (vvdot < EPSILON || e2dot < EPSILON) {
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