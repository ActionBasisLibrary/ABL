//
//  ABSymForceRegress.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymForceRegress.h"

#include "../../External/cmatrix"

#define REGRESS_NUM 2

typedef techsoft::matrix<double> Matrix;
typedef techsoft::valarray<double> Vector;

template<int dimension>
ABSymForceRegress<dimension>::ABSymForceRegress(string name, string curve, string time, int maxData, double *xskew)
: ABSymbol(name, REGRESS_NUM * dimension * dimension, curve), XMatrix(NULL), AVects(NULL), nextIdx(0.0), maxData(maxData)
{
    vector<string> inVect;
    inVect.push_back(curve);
    inVect.push_back(time);
    setInputs(inVect);
    
    if (xskew) {
        memcpy(skew, xskew, dimension*sizeof(double));
    } else {
        for (int i = 0; i < dimension; i++)
            skew[i] = 0;
    }
    
    XMatrix = new Matrix(maxData, REGRESS_NUM * dimension, 0.0);
    AVects = new Vector*[dimension];
    for (int i = 0; i < dimension; i++)
        ((Vector**)AVects)[i] = new Vector(maxData);
}

template<int dimension>
ABSymForceRegress<dimension>::~ABSymForceRegress()
{
    if (XMatrix) delete (Matrix*)XMatrix;
    for (int i = 0; i < dimension; i++)
        delete ((Vector**)AVects)[i];
    if (AVects) delete (Vector**)AVects;
}

template<int dimension>
void ABSymForceRegress<dimension>::recalculate()
{    
    pullValues();
    fitMatrix();
}

template<int dimension>
void ABSymForceRegress<dimension>::pullValues()
{
    ABSymCurve *curve = (ABSymCurve*)inputSyms[0];
    double val[dimension];
    double t = inputSyms[1]->getValue(0) - .1;
    
    // First get position
    curve->getValues(val, t);
//    printf("x:");
//    for (int i = 0; i < dimension; i++)
//        printf(" %f", val[i]);
//    printf("\n");
    
    for (int i = 0; i < dimension; i++)
        (*(Matrix*)XMatrix)[nextIdx][i] = val[i] - skew[i];
    
    // Next get velocity
    curve->getVelocity(val, t);
//    printf("v:");
//    for (int i = 0; i < dimension; i++)
//        printf(" %f", val[i]);
//    printf("\n");
    
    for (int i = 0; i < dimension; i++)
        (*(Matrix*)XMatrix)[nextIdx][i+dimension] = val[i];
    
    // Next acceleration
    curve->getAcceleration(val, t);
//    printf("a:");
//    for (int i = 0; i < dimension; i++)
//        printf(" %f", val[i]);
//    printf("\n");

    for (int i = 0; i < dimension; i++)
        (*((Vector**)AVects)[i])[nextIdx] = val[i];
    
    nextIdx = nextIdx+1 < maxData ? nextIdx+1 : 0;
    numData++;
}

template<int dimension>
void ABSymForceRegress<dimension>::fitMatrix()
{
    double buff[getCard()];
    for (int i = 0; i < getCard(); i++)
        buff[i] = 0.0;
    
    if (numData < getCard()) return;
    
    // Loop for each dimension
    for (int i = 0; i < dimension; i++) {
        
        Vector s;
        
        // Solve and replace if we win
        if (((Matrix*)XMatrix)->solve_qr(*((Vector**)AVects)[i], s))
            for (int j = 0; j < colLength(); j++) {
                buff[i*colLength() + j] = s[j];
                if (isnan(s[j])) { setValues(buff); return; }
            }
    }
    
    setValues(buff);
    
//    printf("%s\n", toString().c_str());
}

template<int dimension> inline
int ABSymForceRegress<dimension>::colLength()
{
    return REGRESS_NUM*dimension;
}

# pragma mark INSTANTIATE
template class ABSymForceRegress<2>;
template class ABSymForceRegress<3>;
