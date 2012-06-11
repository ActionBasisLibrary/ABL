//
//  ABSymIntegrate.h
//  ABL
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymIntegrate_h
#define ABL_ABSymIntegrate_h

#include "ABSymbol.h"

class ABSymIntegrate : public ABSymbol {
    double lasttime;
public:
    ABSymIntegrate(string name, int card, string input, string time);
    
    virtual void recalculate();
};

#endif
