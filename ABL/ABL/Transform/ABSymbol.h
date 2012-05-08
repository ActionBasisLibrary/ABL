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

using namespace std;

/*
 * Symbol struct
 *
 * Input value locations and updates output values with custom pointer
 */
class ABSymbol {
public:
    const unsigned int card, numInputs;
    
protected:
    float *vals;
    ABSymbol **inputSyms;
    bool updated;
    
private:
    const string name;
    const vector<string> inputNames;
    
public:
    ABSymbol(string name, unsigned int card, vector<string> &inputs);
    ABSymbol(string name, unsigned int card);
    ~ABSymbol();
    
    /*
     * Virtual method to be subclassed into functionality
     */
    virtual bool update();
    virtual bool good();
    
    /*
     * Methods for access the values and setting dependencies
     */
    const float *getValues();
    inline float at(unsigned int i) { return vals[i]; }
    inline void linkSym(unsigned int i, ABSymbol *ptr) { inputSyms[i] = ptr; }
    inline const string &getName() { return name; }
    inline const string &getSym(unsigned int i) { return inputNames[i]; }
};

class ABSymSingle : public ABSymbol {
public:
    ABSymSingle(string name, unsigned int card);
    
    void setValues(float *someVals);
};

#endif