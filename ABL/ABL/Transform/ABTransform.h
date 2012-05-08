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
    
    const float *getValue(string name);
    
    // Methods to modify symbol tree
    
    void addSymbol(string name, ABSymbol *sym);
    
private:
    // Private methods
    void modifyTree();
    void link();
    
};

#endif