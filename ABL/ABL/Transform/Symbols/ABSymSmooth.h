//
//  ABSymSmooth.h
//  ABL
//
//  Created by Caleb Jordan on 6/5/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymSmooth_h
#define ABL_ABSymSmooth_h

#include "ABSymbol.h"
#include "GCacheQueue.h"

class ABSymSmooth : public ABSymbol {
    vector<GCacheQueue> qvect;
    
public:
    ABSymSmooth(string name, int card, string input, int smoothRadius = 0);
    
    virtual void recalculate();
};

#endif
