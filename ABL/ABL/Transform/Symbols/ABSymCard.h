//
//  ABSymCard.h
//  ABL
//
//  Created by Caleb Jordan on 6/11/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymCard_h
#define ABL_ABSymCard_h

#include "ABSymbol.h"

class ABSymCard : public ABSymbol {
public:
    ABSymCard(string name, string other)
    : ABSymbol(name, 1, other) {}
    virtual void recalculate() { setValue(inputSyms[0]->getCard(), 0); }
};

class ABSymCardNeg : public ABSymbol {
public:
    ABSymCardNeg(string name, string other)
    : ABSymbol(name, 1, other) {}
    virtual void recalculate() {
        setValue(inputSyms[0]->getCard() > 0 ? inputSyms[0]->getCard() : -1.0, 0);
    }
};

#endif
