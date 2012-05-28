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
    
    /*
     Either CLEAN or DIRTY
     
     see update for effects
     */
    DataState dataState;

    /*
     Array of all input symbol pointers--only populated by link
     */
    ABSymbol **inputSyms;
    
private:
    const string name;
    unsigned int card, numInputs;
    
    /*
     List of input symbol names, used by transform to link
     */
    vector<string> inputNames;
    
    /*
     Lock that governs access to data
     Necessary because tick symbols have their own threads
     */
    pthread_mutex_t vLock;
    double *vals;
    
public:
    /**
     Creates a new symbol with a specified name, cardinality, and list of inputs
     
     @param name    string name of symbol
     @param card    cardenality (length of data buffer)
     @param inputs  vector of strings containing the names of input symbols
     */
    ABSymbol(string name, unsigned int card, vector<string> &inputs);
    
    /**
     Creates a new symbol with a specified name, cardinality, and one input symbol
     
     @param name    string name of symbol
     @param card    cardenality (length of data buffer)
     @param input   string containing the name of one input symbol
     */
    ABSymbol(string name, unsigned int card, string input);
    
    /**
     Creates a new symbol with a specified name and cardinality, with no input symbols
     
     @param name    string name of symbol
     @param card    cardenality (length of data buffer)
     */
    ABSymbol(string name, unsigned int card);
    
    /**
     Deletes the symbol
     */
    ~ABSymbol();
    
    /**
     Update method called frequently that updates the symbol. It is called recursively on
     all input symbols. Will call recalculate() on two conditions:
     
     1. if dataState is marked DIRTY
     (either recalculate marks dataState dirty after updating data every time, or else some
     other function marks dataState dirty when it updates data).
     
     2. if some dependency's update function returns DIRTY (the case if that symbol has changed
     its data.
     
     @param force   true forces the symbol to recalculate regardless of previous state.
     @return        DIRTY if any data in symbol or its dependencies was changed
     @see           recalculate()
     */
    DataState update(bool force = false);
    
    /**
     Recalculates the symbol's data. Should be subclassed by all other symbols.
     Sets dataState DIRTY if symbol data should always be recalculated, otherwise will only
     be called when some other (subclassed) method sets dataState dirty.
     
     @see update()
     */
    virtual void recalculate() = 0;
    
    /**
     Sets the value of the pointer at position i in the dependency list. The name
     of this input position can be obtained from getSymbolName(). Used by transform automatically.
     
     @param i   index of symbol pointer--indexes inputSyms
     @param ptr ABSymbol pointer to input symbol
     */
    inline void linkSymbol(unsigned int i, ABSymbol *ptr) { inputSyms[i] = ptr; }
    
    //Synchronized methods for access the values
    
    /**
     Synchronized method to access the data. Populates buffer with values of symbol.
     
     @param buff    buffer of size "getCard()" in which values are returned
     */
    void getValues(double *buff);
    
    /**
     Synchronized method for obtained one value in the symbol's data
     
     @param i   index of data member to be returned
     @return    data at index i
     */
    double getValue(unsigned int i);
    
    // Not synchronized access methods

    /**
     Method for obtaining the name of the symbol
     
     @return    immutable string name of symbol
     */
    inline const string &getName() { return name; }
    
    /**
     Method for obtaining the name of a dependency to the symbol. Used by transform to link symbols.
     
     @param i   index into inputSyms/inputNames
     */
    inline const string &getSymbolName(unsigned int i) { return inputNames[i]; }
    
    /**
     Returns the cardinality of the symbol's data. This is the size of the data buffer and any buffer that
     hope to obtain the symbol's data
     
     @return    int cardinality
     */
    inline virtual unsigned int getCard() { return card; }
    
    /**
     Returns the number of inputs (dependencies) to this symbol
     
     @return unsigned int number of dependencies.
     */
    inline unsigned int getNumInputs() { return numInputs; }
    
    /**
     Creates a string representation of this symbol useful for debugging.
     Prints out name and data values
     
     @return string representation of the symbol
     */
    string toString();
    
protected:
    /**
     Special constructor for a symbol without cardinality. Used by subclasses
     that must calculate their inputs and cardinality based on input data. Used
     in conjunction with setInputs and setCard.
     
     If a subclass does not call setCard() after using this constructor, it will
     have a zero-length data array
     
     @param name    name of symbol
     */
    ABSymbol(string name);
    
    // Synchronized methods for setting values
    
    /**
     Synchronized method for setting the data values, only used internally
     
     @param buff    buffer that holds new values
     */
    void setValues(double *buff);
    
    /**
     Synchronized method for setting one data value, only used internally
     
     @param val     new value of data member
     @param i       index to set
     */
    void setValue(double val, unsigned int i);
    
    // Synchro utilities
    /**
     Internal function that locks the data mutex
     */
    inline void lock() { pthread_mutex_lock(&vLock); }
    
    /**
     Internal function that unlocks the data mutex
     */
    inline void unlock() { pthread_mutex_unlock(&vLock); }
    
    // Special methods for subclasses to change data
    
    /**
     Sets all information for input symbols. Given a vector of input names,
     this will clear and reset vector of symbol pointers and the number of inputs.
     
     This is generally called by subclasses that wish to modify the input vector
     during creation, such as symbols that don't take a vector or symbols that rely
     on certain dependencies (time) always being last in the inputSyms array. Usually used
     in conjunction with setCard().
     
     @param inputs  new vector of input names.
     @see setCard()
     */
    void setInputs(vector<string> &inputs);
    
    /**
     Sets the cardinality of this symbol. Only called by symbols that need to make
     modifications to the cardinality during creation. Generally used in conjunction with setInputs.
     
     @param c   new cardinality
     @see setInputs()
     */
    void setCard(unsigned int c);
};

#endif