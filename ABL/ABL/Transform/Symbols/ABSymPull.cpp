//
//  ABSymPull.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymPull.h"

ABSymPull::ABSymPull(string name, unsigned int card, bool(*fnPtr)(double *ptr))
: ABSymbol(name, card), pullFunc(fnPtr)
{

}

void ABSymPull::recalculate()
{
    double buff[getCard()];
    if (pullFunc(buff)) {
        setValues(buff);
        dataState = DIRTY;
    }
}