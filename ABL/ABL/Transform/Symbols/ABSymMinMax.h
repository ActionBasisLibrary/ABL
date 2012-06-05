//
//  ABSymMinMax.h
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymMinMax_h
#define ABL_ABSymMinMax_h

#include "ABSymbol.h"
#include <math.h>

class ABSymMin : public ABSymbol {
    double clamp;
    int prime;
    
public:
    ABSymMin(string name, int card, string input, double clampMin = -INFINITY);
    
    virtual void recalculate();
};

class ABSymMax : public ABSymbol {
    double clamp;
    int prime;
    
public:
    ABSymMax(string name, int card, string input, double clampMax = INFINITY);
    
    virtual void recalculate();
};

#endif
