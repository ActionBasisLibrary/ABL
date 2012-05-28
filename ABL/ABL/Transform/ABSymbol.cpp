//
//  ABSymbol.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/3/12.
//  Copyright 2012 Matt Rubin. All rights reserved.
//

#include "ABSymbol.h"


/*
 * Constructors
 */
ABSymbol::ABSymbol(string name)
: card(0), vals(NULL), name(name),
numInputs(0), dataState(DIRTY), inputSyms(NULL)
{
    
}

ABSymbol::ABSymbol(string name, unsigned int card, vector<string> &inputs)
: name(name), card(card), inputSyms(NULL),
vals(new double[card]), dataState(DIRTY)
{
    pthread_mutex_init(&vLock, NULL);
    
    setInputs(inputs);
}

ABSymbol::ABSymbol(string name, unsigned int card, string input)
: name(name), card(card), inputSyms(NULL),
vals(new double[card]), dataState(DIRTY)
{    
    pthread_mutex_init(&vLock, NULL);
    
    vector<string> inVect(&input, &input+1);
    setInputs(inVect);
}

ABSymbol::ABSymbol(string name, unsigned int card)
: name(name), card(card), inputNames(), numInputs(0),
vals(new double[card]), dataState(DIRTY), inputSyms(NULL)
{
    pthread_mutex_init(&vLock, NULL);
    
    inputSyms = NULL;
}

ABSymbol::~ABSymbol()
{
    if (vals) delete[] vals;
    if (inputSyms) delete[] inputSyms;
    pthread_mutex_destroy(&vLock);
}

/*
 * Tree and data management
 */
ABSymbol::DataState ABSymbol::update(bool force)
{
    DataState treeState = dataState;
    for (size_t i = 0; i < numInputs; i++)
        if (inputSyms[i])
            treeState = inputSyms[i]->update(force) && treeState;
    
    dataState = CLEAN;
    
    if (treeState == DIRTY || force)
        recalculate();
    
    return treeState;
}

/*
 * Thread-safe access functions
 */
void ABSymbol::getValues(double *buff)
{
    lock();
    memcpy(buff, vals, card*sizeof(double));
    unlock();
}

double ABSymbol::getValue(unsigned int i)
{
    lock();
    double r = vals[i];
    unlock();
    return r;
}

void ABSymbol::setValues(double *buff)
{
    lock();
    memcpy(vals, buff, card*sizeof(double));
    unlock();
}

void ABSymbol::setValue(double val, unsigned int i)
{
    lock();
    vals[i] = val;
    unlock();
}

void ABSymbol::setInputs(vector<string> &inputs)
{
    inputNames = inputs;
    numInputs = inputs.size();
    
    if (inputSyms)
        delete[] inputSyms;
    
    inputSyms = new ABSymbol* [numInputs];
    for (size_t i = 0; i < numInputs; i++)
        inputSyms[i] = NULL;
}

void ABSymbol::setCard(unsigned int c)
{
    if (vals)
        delete[] vals;
    card = c;
    vals = new double[card];
}

/*
 * Simple access functions
 */
string ABSymbol::toString()
{
    stringstream ss;
    ss << name << ":";
    double buff[card];
    getValues(buff);
    for (unsigned int i = 0; i < getCard(); i++)
        ss << " " << buff[i];
    return ss.str();
}