//
//  ABSymSmooth.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/5/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymSmooth.h"

ABSymSmooth::ABSymSmooth(string name, int card, string input, int smoothRadius)
: ABSymbol(name, card, input)
{
    for (int i = 0; i < card; i++)
        qvect.push_back(GCacheQueue(smoothRadius+1));
}

void ABSymSmooth::recalculate()
{
    double vals[getCard()], nvals[getCard()];
    inputSyms[0]->getValues(vals);
    
    for (int i = 0; i < getCard(); i++) {
        GCacheQueue &q = qvect[i];
        
        q.shift(vals[i]);
        nvals[i] = q.getSum() / q.getSize();
    }
    
    setValues(nvals);
}