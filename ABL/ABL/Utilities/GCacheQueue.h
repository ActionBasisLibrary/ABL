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
    int curr, card, size;
    
public:
    GCacheQueue(int card, double ival = 0.0);
    GCacheQueue(const GCacheQueue &other);
    ~GCacheQueue();
    
    /* 
     * Value reading
     */
    inline double &operator[](int idx) { return vals[(curr+idx)%card]; }
    
    inline void getValues(double *buffer) { memcpy(buffer, vals, card*sizeof(double)); }
    
    void getOrderedValues(double *buffer);
    
    /*
     * Pushing and popping values
     */
    double shift(double nval);
    double pop();
    
    inline int getCard() { return card; }
    inline int getSize() { return size = (size <= card ? size : card); }
    inline void setSize(int s) { size = s < card ? s : card; }
    
    std::string toString();
};

#endif
