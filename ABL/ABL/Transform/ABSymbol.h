//
//  ABSymbol.h
//  ABL
//
//  Created by Caleb Jordan on 5/3/12.
//  Copyright 2012 Matt Rubin. All rights reserved.
//

#ifndef AB_SYMBOL_H
#define AB_SYMBOL_H

#include <vector>
#include <string>
#include <sstream>

using namespace std;

/*
 * Symbol struct
 *
 * Input value locations and updates output values with custom pointer
 */
class ABSymbol {
protected:
    double *vals;
    ABSymbol **inputSyms;
    
    /*
     * Boolean definitions for update function
     */
    typedef bool DataState;
    static const DataState CLEAN = true;
    static const DataState DIRTY = false;
    
    DataState dataState;
    
private:
    const string name;
    vector<string> inputNames;
    const unsigned int card, numInputs;
    
public:
    ABSymbol(string name, unsigned int card, vector<string> &inputs);
    ABSymbol(string name, unsigned int card, string input);
    ABSymbol(string name, unsigned int card);
    ~ABSymbol();
    
    /*
     * Virtual method to be subclassed into functionality
     */
    virtual DataState update();
    
    /*
     * Special method for setting symbol pointers
     */
    inline void linkSymbol(unsigned int i, ABSymbol *ptr) { inputSyms[i] = ptr; }
    
    /*
     * Methods for access the values and setting dependencies
     */
    inline const double *getValues() { return vals; }
    inline double getValue(unsigned int i) { return vals[i]; }
    
    inline const string &getName() { return name; }
    
    inline const string &getSymbolName(unsigned int i) { return inputNames[i]; }
    
    inline unsigned int getCard() { return card; }
    inline unsigned int getNumInputs() { return numInputs; }
    
    string toString();
    
protected:
    
    bool getTreeClean();
    bool shouldUpdate();
};

#endif