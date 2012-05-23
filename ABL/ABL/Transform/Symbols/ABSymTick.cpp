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
: ABSymbol(name, 0, inputs), timer(atimer), refresh(refreshTime)
{   
    // Initialize our synch variables
    chunk = new tchunk(this);
}

ABSymTick::~ABSymTick()
{
    chunk->lock();
    
    if (chunk->count > 0) {
        chunk->count = -1;
        chunk->unlock();
    } else {
        delete chunk;
    }
}

void ABSymTick::start()
{
    chunk->count = 1;
    timer->tick(tickUpdate, chunk, refresh);
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
        return;
    } else if (chunk->count < 0) {
        delete chunk;
        return;
    }
}
