//
//  ABSymTick.h
//  ABL
//
//  Created by Caleb Jordan on 5/21/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymTick_cpp
#define ABL_ABSymTick_cpp

#include "ABSymbol.h"
#include "GTimerThread.h"
#include <pthread.h>

/*
 * Symbol designed to automatically update over time
 */
class ABSymTick : public ABSymbol {
private:
    struct tchunk {
        ABSymTick *sym;
        pthread_mutex_t mlock;
        int count;
        
        tchunk(ABSymTick *ptr): sym(ptr), count(0)
        { pthread_mutex_init(&mlock, NULL); }
        
        ~tchunk() { 
            pthread_mutex_unlock(&mlock);
            pthread_mutex_destroy(&mlock); 
        }
        inline void lock(){pthread_mutex_lock(&mlock);}
        inline void unlock(){pthread_mutex_unlock(&mlock);}
    } *chunk;
    
    double refresh;
    GTimerThread *timer;
    
public:
    ABSymTick(string name, vector<string> &inputs,
              GTimerThread *atimer, double refreshTime);
    ~ABSymTick();
    
    void start();
    void stop();
    
    virtual void tick();
    
    virtual void recalculate() { dataState = DIRTY; }
    
    friend void tickUpdate(void *vchunk);
    
private:
    void cullChildren();
};

#endif
