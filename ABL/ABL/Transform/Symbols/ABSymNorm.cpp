//
//  ABSymNorm.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymNorm.h"
#include <math.h>

ABSymNorm::ABSymNorm(string name, int card, string input)
: ABSymbol(name, 1, input)
{
    
}

void ABSymNorm::recalculate()
{
    int C = inputSyms[0]->getCard();
    double vals[C];
    inputSyms[0]->getValues(vals);
    
    double sum = 0.0;
    for (int i = 0; i < C; i++) {
        sum += vals[i]*vals[i];
    }
    setValue(sqrt(sum), 0);
}