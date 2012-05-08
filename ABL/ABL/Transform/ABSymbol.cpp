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
vals(new double[card]), dataState(CLEAN)
{
    inputSyms = new ABSymbol* [numInputs];
    for (size_t i = 0; i < numInputs; i++)
        inputSyms[i] = NULL;
}

ABSymbol::ABSymbol(string name, unsigned int card, string input)
: name(name), card(card), numInputs(1),
vals(new double[card]), dataState(CLEAN)
{    
    inputNames.push_back(input);
    
    inputSyms = new ABSymbol* [numInputs];
    for (size_t i = 0; i < numInputs; i++)
        inputSyms[i] = NULL;
}

ABSymbol::ABSymbol(string name, unsigned int card)
: name(name), card(card), inputNames(), numInputs(0),
vals(new double[card]), dataState(CLEAN)
{
    inputSyms = NULL;
}

ABSymbol::~ABSymbol()
{
    if (vals) delete[] vals;
    if (inputSyms) delete[] inputSyms;
}

ABSymbol::DataState ABSymbol::update()
{
    DataState treeState = dataState;
    
    for (size_t i = 0; i < numInputs; i++)
        if (inputSyms[i])
            treeState = inputSyms[i]->update() && treeState;
    
    dataState = CLEAN;
    return treeState;
}

bool ABSymbol::getTreeClean()
{
    if (dataState == DIRTY) return DIRTY;
    
    for (size_t i = 0; i < numInputs; i++) {
        if (!inputSyms[i] || !inputSyms[i]->getTreeClean())
            return DIRTY;
    }
    
    return CLEAN;
}

string ABSymbol::toString()
{
    string result = name + ":";
    char buffer[20];
    for (unsigned int i = 0; i < getCard(); i++) {
        sprintf(buffer, " %f", vals[i]);
        result += buffer;
    }
    return result;
}