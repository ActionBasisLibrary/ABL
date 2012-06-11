//
//  ABSymContinuous.h
//  ABL
//
//  Created by Caleb Jordan on 5/23/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymContinuous_cpp
#define ABL_ABSymContinuous_cpp

#include "ABSymbol.h"

class ABSymContinuous : public ABSymbol {
public:
    ABSymContinuous(string name, unsigned int card, vector<string> &inputs)
    : ABSymbol(name, card, inputs) {}
    ABSymContinuous(string name, unsigned int card, string input)
    : ABSymbol(name, card, input) {}
    ABSymContinuous(string name, unsigned int card)
    : ABSymbol(name, card) {}
    
    virtual bool getValues(double *buff, double time) { return false; }
    virtual bool getDerivative(double *buff, double time, int tag) { return false; }
};

#endif
