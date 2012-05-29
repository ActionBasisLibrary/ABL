//
//  ABSymCurve.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/23/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "ABSymCurve.h"
#include "../../External/cmatrix"
#include <assert.h>

typedef techsoft::matrix<double> Matrix;
typedef techsoft::valarray<double> Vector;

ABSymCurve::ABSymCurve(string name, int card, string input, string time, int mCache, double kDist)
: ABSymContinuous(name, card), order(C_ORDER),
kDist(kDist), maxCache(mCache), cullCount(0)
{
    vector<string> inputs;
    inputs.push_back(input);
    inputs.push_back(time);
    setInputs(inputs);

    tCache = getCard();
    
    if (maxCache < C_ORDER*2)
        maxCache = C_ORDER*2;

    for (int i = 0; i < getCard()+1; i++) {
        GCacheQueue q(maxCache, 0.0);
        caches.push_back(q);
    }

    lastCurve = new int[getCard()];
    for (int i = 0; i < getCard(); i++) {
        curves.push_back(vector< Piece<C_ORDER> >());
        lastCurve[i] = -1;
    }
}

ABSymCurve::~ABSymCurve()
{
//    for (int i = 0; i < getCard(); i++)
//        printf("Num pieces %s = %lu\n", getSymbolName(i).c_str(), curves[i].size());
    delete[] lastCurve;
}

bool ABSymCurve::getValues(double *buff, double time)
{
    for (int i = 0; i < getCard(); i++) {
        
        buff[i] = getValueAt(i, time);
        
    }
    return true;
}

void ABSymCurve::ABSymCurve::ABSymCurve::recalculate()
{
    pullCurrent();
    updateCurves();
    
    if (++cullCount > maxCache) {
        cullPast();
        cullCount = 0;
    }

    dataState = DIRTY;
}

void ABSymCurve::clear()
{
    for (int i = 0; i < getCard(); i++) {
        lastCurve[i] = -1;
        caches[i].setSize(0);
        curves[i].clear();
    }
    caches[tCache].setSize(0);
}

bool ABSymCurve::ABSymCurve::ABSymCurve::pullCurrent()
{
    if (!inputSyms[1] || !*inputSyms) return false;
    
    caches[tCache].shift(inputSyms[1]->getValue(0));
    
    double buf[getCard()];
    (*inputSyms)->getValues(buf);
    setValues(buf);

    for (int i = 0; i < getCard(); i++)
        caches[i].shift(buf[i]);

    return true;
}

void ABSymCurve::ABSymCurve::updateCurves()
{
    GCacheQueue &tc = caches[tCache];
    for (int i = 0; i < getCard(); i++) {
        
        GCacheQueue &c = caches[i];
        if (c.getSize() == 0)
            continue;
        
        // Initialize if no curves
        if (curves[i].size() < 1) {
            Piece<C_ORDER> curve = ABSymCurve::getCurve(c, tc);
            curves[i].push_back(curve);
        }
        
        // Run the full calculation
        else if (c.getSize() < c.getCard()) {
            
            Piece<C_ORDER> curve = ABSymCurve::getCurve(c, tc);
            Piece<C_ORDER> last = curves[i].back();
        
            // If curve is still good, modify current top
            if (curve.r <= kDist * distModify(c.getSize(), c.getCard())) {
                curves[i].back() = curve;
            }
            
            // Curve is not good, so start a new one!
            else {
//                printf("Fit Final %d:\n", (int)curves[i].size()-1);
//                printf("\ttmin: %f, tmax: %f\n\t", curves[i].back().tmin, curves[i].back().tmax);
//                for (int c = 0; c <= C_ORDER; c++)
//                    printf(" %f", curves[i].back().coef[c]);
//                printf("\n");
                
                c.setSize(2);
                curve = ABSymCurve::getCurve(c, tc);
                curves[i].push_back(curve);
            }
        
        }

        // Otherwise, start a new top
        else {
//            printf("Num Final %d:\n", (int)curves[i].size()-1);
//            printf("\ttmin: %f, tmax: %f\n\t", curves[i].back().tmin, curves[i].back().tmax);
//            for (int c = 0; c <= C_ORDER; c++)
//                printf(" %f", curves[i].back().coef[c]);
//            printf("\n");
            
            c.setSize(2);
            Piece<C_ORDER> curve = ABSymCurve::getCurve(c, tc);
            curves[i].push_back(curve);
        }

    }
}

void ABSymCurve::cullPast()
{
    for (int i = 0; i < getCard(); i++) {
        
        vector< Piece<C_ORDER> > &vect = curves[i];
        
        for (int j = vect.size()-2; j > 0; j--) {
            Piece<C_ORDER> &curr = vect[j];
            Piece<C_ORDER> &pcurr = vect[j-1];
            
            bool same = true;
            const static double eps = 1e-14;
            for (int c = 0; c <= C_ORDER; c++)
                same = same && (fabs(curr.coef[c] - pcurr.coef[c]) < eps);
            
            if (same) {
                pcurr.tmax = curr.tmax;
                vect.erase(vect.begin() + j);
            }
        }
        
    }
}

/*
 * Does a simple binary search over the list of all pieces in the piece-wise
 * curve that's storing the values and then calculates the actual value.
 * Starts by checking the last curve queried and its neighbor.
 */
double ABSymCurve::getValueAt(int idx, double time)
{
    vector< Piece<C_ORDER> > &vect = curves[idx];
    int last = lastCurve[idx];
    
    // left is inclusive, right is exclusive index
    int l = 0, r = vect.size();
	if (r == 0) return -1;
    
    // First, check the last curve queried--usually consecutive
    // queuries will be for the same curve or consecutive curves
    if (last >= 0 && last < vect.size()) {
        if (vect[last].tmin <= time) {
            
            // Time is still in this curve
            if (vect[last].tmax >= time)
                return vect[last].eval(time);
            
            else {
                // Check following curve
                if (vect.size() > last+1 &&
                    vect[last+1].tmin <= time &&
                    vect[last+1].tmax >= time)
                    return vect[last+1].eval(time);
            }
            
            // No dice, but at least we've eliminated all curves less that this one
            l = last;
        } else {
            r = last;
        }
    }
        
    // Now go into binary search
    do {
        int h = (l+r)/2;
        Piece<C_ORDER> &p = vect[h];
        
        // If this piece is entirely before the time
        if (p.tmax < time) {
            l = h + 1;
        }
        
        // If the piece is entirely after the time
        else if (p.tmin > time) {
            r = h;
        }
        
        // Otherwise, this piece must include the time
        else {
            lastCurve[idx] = h;
            return p.eval(time);
        }
    } while (l < r);
    
    lastCurve[idx] = -1;
    
    return -1;
}

#pragma mark STATIC MATH FUNCTIONS

ABSymCurve::Piece<ABSymCurve::C_ORDER> ABSymCurve::getCurve(GCacheQueue &c, GCacheQueue &tc)
{
    int m = c.getSize();
    
    assert(m > 0);
    assert(m <= tc.getSize());
    
    int n = C_ORDER+1 < m ? C_ORDER+1 : m;
    
    // Begin filling curve
    Piece<C_ORDER> curve;
    if (m == 1)
        curve.setTRange(tc[m-1], tc[0]+1e-6);
    else
        curve.setTRange(tc[m-1], tc[0]);
    
    assert(curve.tmin < curve.tmax); // This could be a problem for super under-constrained moments
    
    // Populate the matrix
    double a[m*n];
    for (int i = 0; i < m; i++) {
        double t = curve.tNorm(tc[i]);
        a[i*n] = 1;
        for (int j = 1; j < n; j++)
            a[i*n+j] = t*a[i*n+j-1];
    }
    
    Matrix A(m,n,a);
    
    // Create the val and solution vector
    Vector v(m);
    for (int i = 0; i < m; i++)
        v[i] = c[i];
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

void ABSymCurve::printStats()
{
    printf("Curve '%s' stats:\n", getName().c_str());
    for (int i = 0; i < getCard(); i++) {
        printf("\tIn %d: %d curves\n", i, (int)curves[i].size());
    }
}