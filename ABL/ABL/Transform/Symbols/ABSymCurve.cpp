//
//  ABSymCurve.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/23/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymCurve.h"
#include "cmatrix"
#include <assert.h>

typedef techsoft::matrix<double> Matrix;
typedef techsoft::valarray<double> Vector;

ABSymCurve::ABSymCurve(string name, vector<string> &inputs, string time, int maxCache, double kDist)
: ABSymContinuous(name, inputs.size()), order(C_ORDER), kDist(kDist)
{
    inputs.push_back(time);
    setInputs(inputs);

    tCache = getCard();
    
    if (maxCache < C_ORDER*2)
        maxCache = C_ORDER*2;

    for (int i = 0; i < getCard()+1; i++) {
        GCacheQueue q(maxCache, 0.0);
        caches.push_back(q);
    }
    
    for (int i = 0; i < getCard(); i++) {
        curves.push_back(vector< Piece<C_ORDER> >());
    }
}

ABSymCurve::~ABSymCurve()
{
    for (int i = 0; i < getCard(); i++)
        printf("Num pieces %s = %lu\n", getSymbolName(i).c_str(), curves[i].size());
}

bool ABSymCurve::getValues(double *buff, double time)
{
    for (int i = 0; i < getCard(); i++) {
        
        buff[i] = getValueAt(curves[i], time);
        
    }
    return true;
}

void ABSymCurve::ABSymCurve::ABSymCurve::recalculate()
{
    pullCurrent();
    updateCurves();
    cullPast();

    dataState = DIRTY;
}

bool ABSymCurve::ABSymCurve::ABSymCurve::pullCurrent()
{
    if (!inputSyms[tCache]) return false;
    
    caches[tCache].shift(inputSyms[tCache]->getValue(0));

    for (int i = 0; i < getCard(); i++) {       
        if (inputSyms[i]) {
            double v = inputSyms[i]->getValue(0);
            setValue(v, i);
            caches[i].shift(v);
        } else {
            setValue(0.0, i);
            caches[i].shift(0.0);
        }
    }

    return true;
}

void ABSymCurve::ABSymCurve::updateCurves()
{
    GCacheQueue &tc = caches[tCache];
    for (int i = 0; i < getCard(); i++) {
        
        GCacheQueue &c = caches[i];
        
        // Initialize if no curves
        if (curves[i].size() < 1) {
            Piece<C_ORDER> curve = ABSymCurve::getCurve(c, tc);
            curves[i].push_back(curve);
        }
        
        // Run the full calculation
        else if (c.getSize() < c.getCard()) {
            
            Piece<C_ORDER> curve = ABSymCurve::getCurve(c, tc);
        
            // If curve is still good, modify current top
            if (curve.r <= kDist * distModify(c.getSize(), c.getCard())) {
                curves[i].back() = curve;
            }
            
            // Curve is not good, so start a new one!
            else {
                c.setSize(2);
                curve = ABSymCurve::getCurve(c, tc);
                curves[i].push_back(curve);
            }
        
        }

        // Otherwise, start a new top
        else {
            c.setSize(2);
            Piece<C_ORDER> curve = ABSymCurve::getCurve(c, tc);
            curves[i].push_back(curve);
        }

    }
}

void ABSymCurve::cullPast()
{

}

double ABSymCurve::getValueAt(vector<Piece<C_ORDER> > &vect, double time)
{
    for (int i = 0; i < (int)vect.size(); i++) {
        Piece<C_ORDER> &p = vect[i];
        
        if (p.tmin <= time && p.tmax >= time) {
            return p.eval(time);
        }
    }
    return -1;
}

#pragma mark STATIC MATH FUNCTIONS

ABSymCurve::Piece<ABSymCurve::C_ORDER> ABSymCurve::getCurve(GCacheQueue &c, GCacheQueue &tc)
{
    int m = c.getSize();
    assert(m <= tc.getSize());
    
    int n = C_ORDER < m ? C_ORDER : m;
    
    // Begin filling curve
    Piece<C_ORDER> curve;
    curve.tmin = tc[c.getSize()-1];
    curve.tmax = tc[0];
    
    assert(curve.tmin <= curve.tmax); // This could be a problem for super under-constrained moments
    
    // Populate the matrix
    double a[m*n];
    for (int i = 0; i < m; i++) {
        double t = tc[i];
        a[i*n] = 1;
        for (int j = 1; j < n; j++)
            a[i*n+j] = t*a[i*n+j-1];
    }
    
    Matrix A(m,n,a);
    
    // Create the val and solution vector
    Vector v(&(c[0]),m);
    Vector s;
    
    // Solve the system!
    A.solve_qr(v, s);
    
    // Calculate the residual
    Vector x = A*s;
    
    double r = 0.0;
    for (int i = 0; i < m; i++)
        r += (x[i] - v[i])*(x[i] - v[i]);
    
    // Fill the curve
    for (int i = 0; i < n; i++)
        curve.coef[i] = s[i];
    curve.r = r;
    
    return curve;
}

double ABSymCurve::distModify(int currSize, int maxSize)
{
    return 1.0;
}