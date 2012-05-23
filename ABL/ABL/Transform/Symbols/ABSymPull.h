//
//  ABSymPull.h
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymPull_h
#define ABL_ABSymPull_h

#include "ABSymbol.h"

/*
 * Symbol class for pulling input data
 */
class ABSymPull : public ABSymbol {
private:
    bool (*pullFunc)(double *ptr);
    
public:
    ABSymPull(string name, unsigned int card, bool(*fnPtr)(double *ptr));
    
    virtual void recalculate();
};

#endif
