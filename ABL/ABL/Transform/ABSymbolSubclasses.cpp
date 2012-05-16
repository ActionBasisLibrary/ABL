//
//  ABSymbolSubclasses.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymbolSubclasses.h"

#pragma mark SIMPLE

ABSymSingle::ABSymSingle(string name, unsigned int card)
: ABSymbol(name, card)
{
    dataState = CLEAN;
}

void ABSymSingle::setValues(float *someVals)
{
    memcpy(vals, someVals, getCard() * sizeof(float));
    dataState = DIRTY;
}

ABSymPull::ABSymPull(string name, unsigned int card, bool(*fnPtr)(float *ptr))
: ABSymbol(name, card), pullFunc(fnPtr)
{
    dataState = DIRTY;
}

ABSymbol::DataState ABSymPull::update()
{
    float buff[getCard()];
    if (pullFunc(buff)) {
        memcpy(vals, buff, getCard() * sizeof(float));
        dataState = CLEAN;
        return DIRTY;
    } else {
        return CLEAN;
    }
}

ABSymVarPull::ABSymVarPull(string name, unsigned int maxCard, bool(*fnPtr)(float *ptr, unsigned int *card))
: ABSymbol(name, maxCard), pullFunc(fnPtr)
{
    dataState = DIRTY;
}

ABSymbol::DataState ABSymVarPull::update()
{
    float buff[ABSymbol::getCard()];
    if (pullFunc(buff, &currentCard)) {
        memcpy(vals, buff, currentCard * sizeof(float));
        dataState = CLEAN;
        return DIRTY;
    } else {
        return CLEAN;
    }
}

unsigned int ABSymVarPull::getCard()
{
	return currentCard;
}


#pragma mark DERIVED

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
                          
ABSymMean::DataState ABSymMean::update()
{
    if (ABSymbol::update())
        return CLEAN;
    
    for (unsigned int i = 0; i < getCard(); i++) {
        if (!inputSyms[i]) continue;
        const float *inVals = inputSyms[i]->getValues();
        
        vals[i] = 0.0;
        for (unsigned int j = 0; j < inputSyms[i]->getCard(); j++)
            vals[i] += inVals[j];
        vals[i] /= inputSyms[i]->getCard();
    }
    
    // Make sure parents update
    return DIRTY;
}

#pragma mark COMPOSITES

ABSymCombine::ABSymCombine(string name, vector<string> &inputs)
: ABSymbol(name, inputs.size(), inputs)
{
    dataState = DIRTY;
}

ABSymbol::DataState ABSymCombine::update()
{
    if (ABSymbol::update())
        return CLEAN;
    
    for (unsigned int i = 0; i < getCard(); i++) {
        vals[i] = (inputSyms[i] ? inputSyms[i]->getValue(0) : 0.);
    }
    
    return DIRTY;
}