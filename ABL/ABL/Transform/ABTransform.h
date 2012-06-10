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

    /**
     Create a transform object
     
     @param manageSymbols   true if a transform should delete allocated symbols, false by default
     @see deleteSymbols()
     */
    ABTransform(bool manageSymbols = false);
    
    /**
     Delete a symbol object. Calls deleteSymbols if manageSymbols is set true
     @see ABTransform()
     */
    ~ABTransform();
    
    /**
     Deletes all symbols and clears containers.
     */
    void deleteSymbols();
    
    // Methods to get values
    
    
    /**
     Populates the provided buffer with the named symbol's data.
     The user is responsible for knowing the correct size of the buffer.
     
     @param name    name of the symbol held by transform
     @param buff    buffer to fill
     */
    void getValues(string name, double *buff);
    
    /**
     Updates the provided symbol and populates the provided
     buffer with the named symbol's data.
     The user is responsible for knowing the correct size of the buffer.
     
     @param name    name of the symbol held by transform
     @param buff    buffer to fill
     */
    void updateAndGetValues(string name, double *buff);
    
    /**
     For symbols that are time dependant (like curves), returns the
     values of the symbol at the time provided. The result returned
     if "time" is not valid depends upon the symbol implementation.

     @param name    name of the symbol held by the transform
     @param buff    buffer to fill
     @param time    time to use for calculation
     */
    void getValues(string name, double *buff, double time);
    
    // Methods to modify symbol tree
    
    /**
     Add one symbol to the transform. Does no linking.

     @param sym:   the symbol to add
     */
    void addSymbol(ABSymbol *sym);
    
    /**
     Add multiple symbols to the transform. Simply calls addSymbol on each.
     
     @param syms    array containing symbol pointers to add
     @param num     number of symbols contained by array
     */
    void addSymbols(ABSymbol **sym, int num);
    
    // Wrapper methods for symbol actions
    
    /**
     Links all symbols and initiates the tick symbol's update function.
     
     @param name    name of the tick symbol
     @return        true if the symbol exists
     */
    bool startTick(string name);
    
    /**
     Interrupts a tick symbol's update function. It may be restarted.
     
     @param name    name of the tick symbol to be interrupted
     @return        true if the symbol exists
     */
    bool stopTick(string name);
    
    /**
     Use to print a certain symbol's info
     
     @param name    name of the symbol to be queried.
     */
    inline string getSymbolString(string name) { return symbols[symMap[name]]->toString(); }
    
    /**
     Use to print all symbols and their dependencies for debugging
     */
    void printSymbolDependencies(ostream &str);
    
	/**
	 Calls the printStats function on an ABSymCurve
	 */
	bool printCurveData(string name);
	
private:
    // Private methods
    
    /*
     Called whenever the symbol tree is modified. Primarily notates that
     the tree should be linked
     */
    void modifyTree();
    
    /*
     Loops over all symbols and provides each a pointer to its dependencies.
     */
    void link();
    
};

#endif