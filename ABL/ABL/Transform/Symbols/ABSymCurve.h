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
        double tmin, tmax, itrange;
        double coef[order+1];
        double r;
        Piece()
        : tmin(0), tmax(0)
        { for (int i = 0; i < order+1; i++) coef[i] = 0.0; }
        inline int getOrder() { return order; }
        double eval(double t, int deriv) {
            t = tNorm(t);
            switch (deriv) {
                case 0:
                {
                    double x = coef[order];
                    for (int i = 0; i < order; i++) {
                        x = x * t + coef[order-i-1];
                    }
                    return x;
                }
                case 1:
                {
                    if (order < 1) return 0;
                    double x = coef[order]*order;
                    for (int i = 0; i < order-1; i++) {
                        x = x * t + coef[order-i-1]*(order-i-1);
                    }
                    return x;
                }
                case 2:
                {
                    if (order < 2) return 0;
                    double x = coef[order]*order*(order-1);
                    for (int i = 0; i < order-2; i++) {
                        x = x * t + coef[order-i-1]*(order-i-1)*(order-i-2);
                    }
                    return x;
                }
                default:
                    fprintf(stderr, "Requesting unimplemented derivative. Sorry.\n");
                    return 0.0;
            }
            double x = coef[order];
            for (int i = 0; i < order; i++) {
                x = x * t + coef[order-i-1];
            }
            return x;
        }
        void setTRange(double amin, double amax) {
            tmin = amin; tmax = amax; itrange = 1.0/(tmax - tmin);
        }
        inline double tNorm(double t) {
            return (t - tmin)*itrange;
        }
        string toString() {
            stringstream ss;
            ss << "[" << tmin << ", " << tmax << "]:";
            for (int i = 0; i <= order; i++)
                ss << " " << coef[i];
            return ss.str();
        }
    };

    int order;
    double kDist;

    vector< GCacheQueue > caches;
    int tCache;
    
    const static int C_ORDER = 4;
    vector< vector< Piece<C_ORDER> > > curves;
    int *lastCurve, cullCount, maxCache;

public:
    ABSymCurve(string name, int card, string inputs, string time, int maxCache, double kDist);
    ~ABSymCurve();

    bool getValues(double *buff, double time);
    bool getDerivative(double *buff, double time, int tag);
    bool getVelocity(double *buff, double time);
    bool getAcceleration(double *buff, double time);

    void recalculate();
    
    void clear();
    
    void printStats();

private:
    bool pullCurrent();
    void updateCurves();
    void cullPast();
    
    double getValueAt(int curve, double time, int deriv = 0);
    
    static Piece<C_ORDER> getCurve(GCacheQueue &c, GCacheQueue &tc);
    static double distModify(int currSize, int maxSize);
};

#endif
