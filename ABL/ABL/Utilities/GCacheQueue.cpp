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
: vals(new double[card]), card(card), curr(0), size(0)
{
    for (int i = 0; i < card; i++)
        vals[i] = ival;
}

GCacheQueue::GCacheQueue(const GCacheQueue &other)
: vals(new double[other.card]), card(other.card),
curr(other.curr), size(other.size)
{
    memcpy(vals, other.vals, card * sizeof(double));
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
    size = size < card ? size+1 : card;
    return rval;
}

double GCacheQueue::pop()
{
    if (size < 1) return 0.0;
    
    double rval = (*this)[size-1];
    size--;
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