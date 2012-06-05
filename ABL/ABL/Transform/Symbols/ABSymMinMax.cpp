//
//  ABSymMinMax.cpp
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymMinMax.h"

ABSymMin::ABSymMin(string name, int card, string input, double clampMin)
: ABSymbol(name, card, input), clamp(clampMin), prime(NUM_PRIMER_RUNS)
{
    for (int i = 0; i < card; i++)
        setValue(INFINITY, i);
}
    
void ABSymMin::recalculate()
{
    if (prime < 1) {
        double nvals[getCard()];
        inputSyms[0]->getValues(nvals);
        
        double vals[getCard()];
        getValues(vals);
        
        for (int i = 0; i < getCard(); i++)
            vals[i] = max(min(vals[i], vals[i]), clamp);
        
        setValues(vals);
    } else prime--;
}


ABSymMax::ABSymMax(string name, int card, string input, double clampMax)
: ABSymbol(name, card, input), clamp(clampMax), prime(NUM_PRIMER_RUNS)
{
    for (int i = 0; i < card; i++)
        setValue(-INFINITY, i);
}
    
void ABSymMax::recalculate()
{
    if (prime < 1) {
        double nvals[getCard()];
        inputSyms[0]->getValues(nvals);
        
        double vals[getCard()];
        getValues(vals);
        
        for (int i = 0; i < getCard(); i++)
            vals[i] = min(max(vals[i], vals[i]), clamp);
        
        setValues(vals);
    } else prime--;
}