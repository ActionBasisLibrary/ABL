//
//  ABSymSet.h
//  ABL
//
//  Created by Caleb Jordan on 5/21/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymSet_cpp
#define ABL_ABSymSet_cpp

#include "ABSymbol.h"

class ABSymSet : public ABSymbol {
private:
    double *buffer;
    
public:
    ABSymSet(string name, unsigned int card, double *buff)
    : ABSymbol(name, card), buffer(buff) { recalculate(); }
    
    inline void recalculate() { setValues(buffer); dataState = DIRTY; }
};

#endif
