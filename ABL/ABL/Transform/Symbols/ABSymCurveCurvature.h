//
//  ABSymCurveCurvature.h
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymCurveCurvature_h
#define ABL_ABSymCurveCurvature_h

#include "ABSymbol.h"
#include <math.h>

class ABSymCurveCurvature : public ABSymbol {
private:
    double *lastv, *lastp, lastt;
    
    int incard;
    
    const static int timeIdx = 1;
    
public:
    ABSymCurveCurvature(string name, string input, int dimension, string time);
    ~ABSymCurveCurvature();
    
    virtual void recalculate();
};

#endif
