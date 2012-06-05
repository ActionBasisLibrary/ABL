//
//  ABSymNorm.h
//  ABL
//
//  Created by Caleb Jordan on 6/4/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymNorm_h
#define ABL_ABSymNorm_h

#import "ABSymbol.h"

class ABSymNorm : public ABSymbol {
public:
    ABSymNorm(string name, int card, string input);
    
    virtual void recalculate();
};

#endif
