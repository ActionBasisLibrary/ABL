//
//  ABSymCurve.h
//  ABL
//
//  Created by Caleb Jordan on 5/23/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_ABSymCurve_h
#define ABL_ABSymCurve_h

#include "ABSymContinuous.h"
#include "GCacheQueue.h"

class ABSymCurve : public ABSymContinuous {
private:
    template<int order>
    struct Piece {
        double tmin, tmax;
        double coef[order+1];
        double r;
        Piece()
        : tmin(0), tmax(0)
        { for (int i = 0; i < order+1; i++) coef[i] = 0.0; }
        inline int getOrder() { return order; }
        double eval(double t) {
            double x = coef[order-1];
            for (int i = 1; i < order; i++) {
                x = x * t + coef[order-i-1];
            }
            return x;
        }
    };

    int order;
    double kDist;

    vector< GCacheQueue > caches;
    int tCache;
    
    const static int C_ORDER = 3;
    vector< vector< Piece<C_ORDER> > > curves;

public:
    ABSymCurve(string name, vector<string> &inputs, string time, int maxCache, double kDist);
    ~ABSymCurve();

    bool getValues(double *buff, double time);

    void recalculate();

private:
    bool pullCurrent();
    void updateCurves();
    void cullPast();
    
    double getValueAt(vector<Piece<C_ORDER> > &vect, double time);
    
    static Piece<3> getCurve(GCacheQueue &c, GCacheQueue &tc);
    static double distModify(int currSize, int maxSize);
};

#endif
