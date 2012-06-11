//
//  ABSymForceRegress.h
//  ABL
//
//  Created by Caleb Jordan on 6/10/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymForceRegress_h
#define ABL_ABSymForceRegress_h

#include "ABSymbol.h"
#include "ABSymCurve.h"

template <int dimension>
class ABSymForceRegress : public ABSymbol {
    void *XMatrix;
    void *AVects;
    int nextIdx, maxData, numData;
    double skew[dimension];
    
public:
    ABSymForceRegress(string name, string curve, string time, int maxData, double *askew = NULL);
    ~ABSymForceRegress();
    
    virtual void recalculate();
    
private:
    void pullValues();
    void fitMatrix();
    
    int colLength();
};

#endif
