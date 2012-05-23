//
//  ABSymCombine.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/20/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymCombine.h"

ABSymCombine::ABSymCombine(string name, vector<string> &inputs)
: ABSymbol(name, inputs.size(), inputs)
{
    dataState = DIRTY;
}

void ABSymCombine::recalculate()
{
    double buff[getCard()];
    for (unsigned int i = 0; i < getCard(); i++) {
        buff[i] = (inputSyms[i] ? inputSyms[i]->getValue(0) : 0.);
    }
    setValues(buff);
}