//
//  ABTransform.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/3/12.
//  Copyright 2012 Matt Rubin. All rights reserved.
//

#include "ABTransform.h"
#include "ABSymCurve.h"

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
    symbols.clear();
    symMap.clear();
    numSyms = 0;
    modifyTree();
}

// Methods to get values

void ABTransform::getValues(string name, double *buff)
{
    if (!linked) link();
    
    if (symMap.count(name) > 0)
        symbols.at(symMap[name])->getValues(buff);
}

void ABTransform::updateAndGetValues(string name, double *buff)
{
    if (!linked) link();

    if (symMap.count(name) > 0) {
        symbols.at(symMap[name])->update();
        symbols.at(symMap[name])->getValues(buff);
    }
}

void ABTransform::getValues(string name, double *buff, double time)
{
    if (!linked) link();
    
    if (symMap.count(name) > 0)
        ((ABSymContinuous*)symbols.at(symMap[name]))->getValues(buff, time);
}

// Methods to modify symbol tree

void ABTransform::addSymbol(ABSymbol *sym)
{
    if (!sym) return;
    
    string name = sym->getName();
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

void ABTransform::addSymbols(ABSymbol **syms, int num)
{
    for (int i = 0; i < num; i++)
        addSymbol(syms[i]);
}

bool ABTransform::startTick(string name)
{
    if (symMap.count(name) > 0) {
        if (!linked) link();
        ((ABSymTick*)symbols[symMap[name]])->start();
        return true;
    } else {
        return false;
    }
}

bool ABTransform::stopTick(string name)
{
    if (symMap.count(name) > 0) {
        ((ABSymTick*)symbols[symMap[name]])->stop();
        return true;
    } else {
        return false;
    }
}

void ABTransform::printSymbolDependencies(ostream &str)
{
    for (int i = 0; i < symbols.size(); i++) {
        ABSymbol *sym = symbols[i];
        str << sym->getName() << ": ";
        for (int d = 0; d < sym->getNumInputs(); d++) {
            str << (d == 0 ? "" : ", ") << sym->getSymbolName(d);
        }
        str << endl;
    }
}

bool ABTransform::printCurveData(string name)
{
	if (symMap.count(name) > 0) {
        ((ABSymCurve*)symbols[symMap[name]])->printStats();
        return true;
    } else {
        return false;
    }
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
        
        for (size_t d = 0; d < sym->getNumInputs(); d++) {
            string inName = sym->getSymbolName(d);
            
            if (symMap.count(inName) == 0) {
                fprintf(stderr, "Symbol '%s' trying to link to unknown symbol '%s'\n",
                        sym->getName().c_str(), inName.c_str());
                sym->linkSymbol(d, NULL);
            }
            
            else {
                sym->linkSymbol(d, symbols[symMap[inName]]);
            }
        }
    }
    
    linked = true;
}