//
//  ABTransform.h
//  ABL
//
//  Created by Caleb Jordan on 5/3/12.
//  Copyright 2012 Matt Rubin. All rights reserved.
//

#ifndef AB_TRANSFORM_H
#define AB_TRANSFORM_H

#include <map>
#include <string>
#include <vector>

#include "ABSymbol.h"
#include "ABSymTick.h"
#include "ABSymContinuous.h"

using namespace std;

class ABTransform {
public:
    // Public enums, etc
    
private:
    // Private definitions
    typedef unsigned int SymId;
    
    // Private member variables
    bool linked, manageSyms;
    SymId numSyms;
    map<string, SymId> symMap;
    vector<ABSymbol*> symbols;
    
public:
    // Public methods
    ABTransform(bool manageSymbols = false);
    ~ABTransform();
    
    void deleteSymbols();
    
    // Methods to get values
    
    void getValues(string name, double *buff);
    void getValues(string name, double *buff, double time);
    
    // Methods to modify symbol tree
    
    void addSymbol(ABSymbol *sym);
    void addSymbols(ABSymbol **sym, int num);
    
    // Wrapper methods for symbol actions
    
    bool startTick(string name);
    
private:
    // Private methods
    void modifyTree();
    void link();
    
};

#endif