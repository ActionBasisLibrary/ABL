//
//  GCacheQueue.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "GCacheQueue.h"
#include <sstream>

GCacheQueue::GCacheQueue(int card, double ival)
: vals(new double[card]), card(card), curr(0)
{
    for (int i = 0; i < card; i++)
        vals[i] = ival;
}

GCacheQueue::~GCacheQueue()
{
    delete[] vals;
}

void GCacheQueue::getOrderedValues(double *buffer)
{
    memcpy(buffer, vals+curr, (card-curr)*sizeof(double));
    memcpy(buffer+(card-curr), vals, curr*sizeof(double));
}

double GCacheQueue::shift(double nval)
{
    double rval = vals[curr=(curr ? curr-1 : card-1)];
    vals[curr] = nval;
    return rval;
}

std::string GCacheQueue::toString()
{
    std::stringstream ss;
    
    for (int i = 0; i < card; i++) {
        if (i == curr) ss << "| ";
        ss << vals[i] << " ";
    }
    
    return ss.str();
}