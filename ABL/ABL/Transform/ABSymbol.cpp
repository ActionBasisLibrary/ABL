//
//  ABSymbol.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/3/12.
//  Copyright 2012 Matt Rubin. All rights reserved.
//

#include "ABSymbol.h"

ABSymbol::ABSymbol(string name, unsigned int card, vector<string> &inputs)
: name(name), card(card), inputNames(inputs), numInputs(inputs.size()),
vals(new float[card]), updated(false)
{
    inputSyms = new ABSymbol* [numInputs];
    for (size_t i = 0; i < numInputs; i++)
        inputSyms[i] = NULL;
}

ABSymbol::ABSymbol(string name, unsigned int card)
: name(name), card(card), inputNames(), numInputs(0),
vals(new float[card]), updated(false)
{
    inputSyms = NULL;
}

ABSymbol::~ABSymbol()
{
    if (vals) delete[] vals;
    if (inputSyms) delete[] inputSyms;
}

bool ABSymbol::update()
{
    if (good()) return true;
    
    else {
        for (size_t i = 0; i < numInputs; i++) {
            if (inputSyms[i]) inputSyms[i]->update();
            else return false;
        }
    }
    
    return true;
}

bool ABSymbol::good()
{
    if (!updated) return false;
    
    for (size_t i = 0; i < numInputs; i++) {
        if (!inputSyms[i] || !inputSyms[i]->good())
            return false;
    }
    
    return true;
}

const float *ABSymbol::getValues()
{
    if (update()) return vals;
    
    return NULL;
}

#pragma SUBCLASSES

ABSymSingle::ABSymSingle(string name)
: ABSymbol(name, 1)
{
    updated = true;
}

void ABSymSingle::setValue(float val)
{
    vals[0] = val;
}