//
//  ABSymVarPull.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymVarPull.h"

ABSymVarPull::ABSymVarPull(string name, unsigned int maxCard, bool(*fnPtr)(double *ptr, unsigned int *card))
: ABSymbol(name, maxCard), pullFunc(fnPtr)
{
    
}

void ABSymVarPull::recalculate()
{
    double buff[ABSymbol::getCard()];
    
    if (pullFunc(buff, &currentCard)) {
        setValues(buff);
        dataState = DIRTY;
    }
	
	dataState = DIRTY;
}

unsigned int ABSymVarPull::getCard()
{
	return currentCard;
}