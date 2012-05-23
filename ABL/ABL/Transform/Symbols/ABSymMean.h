//
//  ABSymMean.h
//  ABL
//
//  Created by Caleb Jordan on 5/20/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymMean_h
#define ABL_ABSymMean_h

#include "ABSymbol.h"

/*
 * Derived symbol class that derives the mean of another symbol's values
 */
class ABSymMean : public ABSymbol {
public:
    ABSymMean(string name, vector<string> &inputs);
    ABSymMean(string name, string input);
    
    virtual void recalculate();
};

#endif
