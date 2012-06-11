//
//  ABQuadIntegrator.h
//  ABL
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABQuadIntegrator_h
#define ABL_ABQuadIntegrator_h

#include "ABSymbol.h"

class ABQuadIntegrator : public ABSymbol {
    double center[2];
    double lasttime;
public:
    ABQuadIntegrator(string name, string input, string time, double ctr[2]);
    
    virtual void recalculate();
};

#endif
