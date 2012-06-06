//
//  ABSymMean.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/20/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymMean.h"

ABSymMean::ABSymMean(string name, vector<string> &inputs)
: ABSymbol(name, inputs.size(), inputs)
{
    dataState = DIRTY;
}

ABSymMean::ABSymMean(string name, string input)
: ABSymbol(name, 1, input)
{
    dataState = DIRTY;
}

void ABSymMean::recalculate()
{
    double sums[getCard()];
    for (int i = 0; i < getCard(); i++)
        sums[i] = 0.0;
    
    for (unsigned int i = 0; i < getCard(); i++) {
        if (!inputSyms[i] || inputSyms[i]->getCard() == 0) continue;
        double buff[inputSyms[i]->getCard()];// = new double[inputSyms[i]->getCard()];
        inputSyms[i]->getValues(buff);
        
        sums[i] = 0.0;
        for (unsigned int j = 0; j < inputSyms[i]->getCard(); j++)
            sums[i] += buff[j];
        sums[i] /= inputSyms[i]->getCard();
    }
    setValues(sums);
    
    dataState = DIRTY;
}