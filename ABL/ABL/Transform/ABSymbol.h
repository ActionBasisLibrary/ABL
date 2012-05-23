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
    /*
     * Boolean definitions for update function
     */
    typedef bool DataState;
    static const DataState CLEAN = true;
    static const DataState DIRTY = false;
    
    DataState dataState;

    ABSymbol **inputSyms;
    
private:
    const string name;
    unsigned int card, numInputs;
    
    vector<string> inputNames;
    
    pthread_mutex_t vLock;
    double *vals;
    
public:
    ABSymbol(string name, unsigned int card, vector<string> &inputs);
    ABSymbol(string name, unsigned int card, string input);
    ABSymbol(string name, unsigned int card);
    ~ABSymbol();
    
    /*
     * Virtual method to be subclassed into functionality
     */
    DataState update(bool force = false);
    virtual void recalculate() = 0;
    
    /*
     * Special method for setting symbol pointers
     */
    inline void linkSymbol(unsigned int i, ABSymbol *ptr) { inputSyms[i] = ptr; }
    
    /*
     * Synchronized methods for access the values
     */
    void getValues(double *buff);
    double getValue(unsigned int i);
    
    /*
     * Not synchronized access methods
     */
    inline const string &getName() { return name; }
    inline const string &getSymbolName(unsigned int i) { return inputNames[i]; }
    inline virtual unsigned int getCard() { return card; }
    inline unsigned int getNumInputs() { return numInputs; }
    string toString();
    
protected:
    ABSymbol(string name);
    
    /*
     * Synchronized methods for setting values
     */
    void setValues(double *buff);
    void setValue(double val, unsigned int i);
    
    /*
     * Synchro utilities
     */
    inline void lock() { pthread_mutex_lock(&vLock); }
    inline void unlock() { pthread_mutex_unlock(&vLock); }
    
    /*
     * Custom modifiers for subclasses to change data
     */
    void setInputs(vector<string> &inputs);
    void setCard(unsigned int c);
};

#endif