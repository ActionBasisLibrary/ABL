//
//  ABSymbolSubclasses.h
//  ABL
//
//  Created by Caleb Jordan on 5/7/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymbolSubclasses_h
#define ABL_ABSymbolSubclasses_h

#include "ABSymbol.h"

#pragma mark LIGHTWEIGHT SYMBOLS

/*
 * Symbol class for manually set symbol values
 */
class ABSymSingle : public ABSymbol {
public:
    ABSymSingle(string name, unsigned int card);
    
    void setValues(float *someVals);
};

/*
 * Symbol class for pulling input data
 */
class ABSymPull : public ABSymbol {
private:
    bool (*pullFunc)(float *ptr);
public:
    ABSymPull(string name, unsigned int card, bool(*fnPtr)(float *ptr));
    
    virtual DataState update();
};

#pragma mark DERIVED VALUE SYMBOLS

/*
 * Derived symbol class that derives the mean of another symbol's values
 */
class ABSymMean : public ABSymbol {
public:
    ABSymMean(string name, vector<string> &inputs);
    ABSymMean(string name, string input);
    
    virtual DataState update();
};

#pragma mark COMPOSITE

/*
 * Combines multiple single-output symbols into one
 */
class ABSymCombine : public ABSymbol {
public:
    ABSymCombine(string name, vector<string> &inputs);

    virtual DataState update();
};

#endif
