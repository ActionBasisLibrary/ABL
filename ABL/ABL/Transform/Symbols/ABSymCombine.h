//
//  ABSymCombine.h
//  ABL
//
//  Created by Caleb Jordan on 5/20/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymCombine_cpp
#define ABL_ABSymCombine_cpp

#include "ABSymbol.h"

/*
 * Combines multiple single-output symbols into one
 */
class ABSymCombine : public ABSymbol {
public:
    ABSymCombine(string name, vector<string> &inputs);
    
    virtual void recalculate();
};

#endif
