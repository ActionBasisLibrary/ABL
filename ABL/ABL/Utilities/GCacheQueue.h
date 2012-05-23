//
//  GCacheQueue.h
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_GCacheQueue_h
#define ABL_GCacheQueue_h

#include <memory.h>
#include <iostream>

class GCacheQueue {
private:
    double *vals;
    int curr, card;
    
public:
    GCacheQueue(int card, double ival = 0.0);
    ~GCacheQueue();
    
    /* 
     * Value reading
     */
    inline double &operator[](int idx) { return vals[(curr+idx)%card]; }
    
    inline void getValues(double *buffer) { memcpy(buffer, vals, card*sizeof(double)); }
    
    void getOrderedValues(double *buffer);
    
    /*
     * Pushing values
     */
    double shift(double nval);
    
    inline int getCard() { return card; }
    
    std::string toString();
};

#endif
