//
//  ABCountIntegrate.h
//  ABL
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABCountIntegrate_cpp
#define ABL_ABCountIntegrate_cpp

#include "ABSymbol.h"

class ABCountIntegrate : public ABSymbol {
    double lasttime;
public:
    ABCountIntegrate(string name, string input, string time);
    
    virtual void recalculate();
};

#endif
