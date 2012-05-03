//
//  ABTransform.h
//  ABL
//
//  Created by Caleb Jordan on 5/3/12.
//  Copyright 2012 Matt Rubin. All rights reserved.
//

#include <map>
#include <string>
#include <vector>

using namespace std;

class ABTransform {
public:
    // Public enums, etc
    
private:
    // Private definitions
    typedef string Symbol;
    typedef unsigned int SymId;
    typedef float symVal;
    
    // Private member variables
    bool linked;
    map<Symbol, SymId> symbols;
    vector<symVal> symVals;
    
public:
    // Public methods
    
    ABTransform();
    ~ABTransform();
    
    void addInput();
    
private:
    // Private methods

};