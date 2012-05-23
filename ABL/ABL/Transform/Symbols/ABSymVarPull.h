//
//  ABSymVarPull.h
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymVarPull_h
#define ABL_ABSymVarPull_h

#include "ABSymbol.h"

class ABSymVarPull : public ABSymbol {
private:
    bool (*pullFunc)(double *ptr, unsigned int *card);
	unsigned int currentCard;
    
public:
    ABSymVarPull(string name, unsigned int maxCard, bool(*fnPtr)(double *ptr, unsigned int *card));
    
    unsigned int getCard();
    
    virtual void recalculate();
};

#endif
