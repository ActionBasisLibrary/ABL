//
//  ABTransform.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/3/12.
//  Copyright 2012 Matt Rubin. All rights reserved.
//

#include "ABTransform.h"

ABTransform::ABTransform(bool manageSymbols)
: numSyms(0), linked(false), manageSyms(manageSymbols)
{
    
}

ABTransform::~ABTransform()
{
    if (manageSyms) deleteSymbols();
}

void ABTransform::deleteSymbols()
{
    for (size_t i = 0; i < symbols.size(); i++)
        delete symbols[i];
}

// Methods to get values

const float *ABTransform::getValue(string name)
{
    if (!linked) link();
    
    if (symMap.count(name) > 0)
        return symbols.at(symMap[name])->getValues();
    else
        return NULL;
}

// Methods to modify symbol tree

void ABTransform::addSymbol(string name, ABSymbol *sym)
{
    if (!sym) return;
    
    if (symMap.count(name) > 0) {
        if (manageSyms) delete symbols.at(symMap[name]);
        symbols[symMap[name]] = sym;
    }
    
    else {
        symbols.push_back(sym);
        symMap[name] = numSyms++;
    }
    
    modifyTree();
}

#pragma PRIVATE TREE METHODS

void ABTransform::modifyTree()
{
    linked = false;
}

void ABTransform::link()
{
    for (size_t i = 0; i < numSyms; i++) {
        ABSymbol *sym = symbols[i];
        
        for (size_t d = 0; d < sym->numInputs; d++) {
            string inName = sym->getSym(d);
            
            if (symMap.count(inName) == 0) {
                fprintf(stderr, "Symbol '%s' trying to link to unknown symbol '%s'\n",
                        sym->getName().c_str(), inName.c_str());
                sym->linkSym(d, NULL);
            }
            
            else {
                sym->linkSym(d, symbols[symMap[inName]]);
            }
        }
    }
    
    linked = true;
}