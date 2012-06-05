//
//  ABSymCurvature.h
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymCurvature_h
#define ABL_ABSymCurvature_h

#include "ABSymbol.h"
#include <math.h>

class ABSymCurvature : public ABSymbol {
private:
    double *lastv, *lastp, lastt;
    
    int incard;
    
    const static int timeIdx = 1;
    
public:
    ABSymCurvature(string name, string input, int dimension, string time);
    ~ABSymCurvature();
    
    virtual void recalculate();
};

#endif
