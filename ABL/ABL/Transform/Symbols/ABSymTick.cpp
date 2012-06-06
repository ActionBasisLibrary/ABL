//
//  ABSymTick.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/21/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymTick.h"

void tickUpdate(void *vsym);

ABSymTick::ABSymTick(string name, vector<string> &inputs,
                     GTimerThread *atimer, double refreshTime)
: ABSymbol(name, 0, inputs), timer(atimer), refresh(refreshTime),
chunk(NULL)
{   
    
}

ABSymTick::~ABSymTick()
{
    stop();
}

void ABSymTick::start()
{
    if (!chunk) {
        cullChildren();
        
        chunk = new tchunk(this);
        
        chunk->lock();
        
        chunk->count = 1;
        timer->tick(tickUpdate, chunk, refresh);
        
        chunk->unlock();
    }
}

void ABSymTick::stop()
{
    if (chunk) {
        chunk->lock();
        
        if (chunk->count > 0) {
            chunk->count = -1;
            chunk->unlock();
        } else {
            delete chunk;
        }
        chunk = NULL;
    }
}

void ABSymTick::tick()
{
    update();
}

void tickUpdate(void *vchunk)
{
    ABSymTick::tchunk *chunk = (ABSymTick::tchunk*)vchunk;
    
    chunk->lock();
    if (chunk->count > 0) {
        chunk->sym->tick();
        
        chunk->sym->timer->tick(tickUpdate, chunk, chunk->sym->refresh);
        chunk->unlock();
    }
    
    else if (chunk->count < 0) {
        delete chunk;
    }
}

void ABSymTick::cullChildren()
{
    bool include[getNumInputs()];
    vector<const ABSymbol*> symList;
    
    // Initialize stack and include arrays
    for (int i = 0; i < getNumInputs(); i++) {
        include[i] = true;
        symList.push_back(getSymbolPointer(i));
    }
    
    // We need to run a complicated DFS to tap out descendents
    int skip = getNumInputs();
    while (symList.size() > 0) {
        const ABSymbol *sym = symList.back();
        symList.pop_back();
        if (!sym) continue;
        
        // This is one of the tick's kids, so don't check the vector
        if (symList.size() < skip) {
            skip--;
            
            // Find its index and check if it's been marked to cull already
            // continue if so
            if (!include[skip]) continue;
            
            for (int i = 0; i < sym->getNumInputs(); i++)
                symList.push_back(sym->getSymbolPointer(i));
        }
        
        // This is not an immediate kid, so check
        else {
            for (int i = 0; i < getNumInputs(); i++) {
                
                // Descendant is in first kids array, so mark for culling
                if (getSymbolName(i).compare(sym->getName()) == 0) {
                    // if already culled, we don't need to check lower
                    if (!include[i]) break;
                    
                    for (int j = 0; j < sym->getNumInputs(); j++)
                        symList.push_back(sym->getSymbolPointer(j));
                    
                    include[i] = false;
                }
                   
            }
        }
    }
    
    // Now that we have include array, create new 
    vector<int> idxs;
    for (int i = 0; i < getNumInputs(); i++)
        if (include[i]) idxs.push_back(i);
    setAndRelinkInputs(idxs);
}
