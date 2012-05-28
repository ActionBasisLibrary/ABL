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
