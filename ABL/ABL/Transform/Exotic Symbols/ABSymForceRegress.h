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

#include "../../External/cmatrix"

typedef techsoft::matrix<double> Matrix;
typedef techsoft::valarray<double> Vector;

//template<int dimension>

template <int dimension>
class ABSymForceRegress : public ABSymbol {
    Matrix *XMatrix;
    Vector *AVects;
    int nextIdx;
    
public:
    ABSymForceRegress(string name, string curve, string time, int maxData);
    ~ABSymForceRegress();
    
    virtual void recalculate();
    
private:
    void pullValues();
    void fitMatrix();
};

#endif
