//
//  ABSymForceRegress.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymForceRegress.h"

template<int dimension>
ABSymForceRegress<dimension>::ABSymForceRegress(string name, string curve, string time, int maxData)
: ABSymbol(name, 2 * dimension * dimension, curve), XMatrix(NULL), AVects(NULL), nextIdx(0.0)
{
    vector<string> inVect;
    inVect.push_back(curve);
    inVect.push_back(time);
    
    XMatrix = new Matrix(maxData, 2 * dimension, 0.0);
    AVects = new Vector[3];
    for (int i = 0; i < dimension; i++)
        AVects[i] = Vector(0.0, maxData);
}

template<int dimension>
ABSymForceRegress<dimension>::~ABSymForceRegress()
{
    if (XMatrix) delete XMatrix;
    if (AVects) delete AVects;
}

template<int dimension>
void ABSymForceRegress<dimension>::recalculate()
{
    if (!inputSyms[0]) return;
    
    pullValues();
    fitMatrix();
}

template<int dimension>
void ABSymForceRegress<dimension>::pullValues()
{
    ABSymCurve *curve = (ABSymCurve*)inputSyms[0];
    double val[dimension];
    double t = inputSyms[1]->getValue(0);
    
    // First get position
    curve->getValues(val, t);
    
    for (int i = 0; i < dimension; i++)
        (*XMatrix)[nextIdx][i] = val[i];
    
    // Next get velocity
    curve->getVelocity(val, t);
    
    for (int i = 0; i < dimension; i++)
        (*XMatrix)[nextIdx][i+3] = val[i];
    
    // Next acceleration
    curve->getAcceleration(val, t);

    for (int i = 0; i < dimension; i++)
        AVects[i][nextIdx] = val[i];
}

template<int dimension>
void ABSymForceRegress<dimension>::fitMatrix()
{
    double buff[dimension*dimension*2];
    for (int i = 0; i < dimension*dimension*2; i++)
        buff[i] = 0;
    
    // Loop for each dimension
    for (int i = 0; i < dimension; i++) {
        
        Vector s;
        
        // Solve and replace if we win
        if (XMatrix->solve_qr(AVects[i], s))
            for (int j = 0; j < 2*dimension; j++)
                buff[i*dimension + j] = s[j];
    }
    
    setValues(buff);
}

