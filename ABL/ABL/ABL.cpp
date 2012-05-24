//
//  ABL.cpp
//  ABL
//
//  Created by Matt Rubin on 4/24/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include <iostream>
#include "ABL.h"

#include "cmatrix"

GTimer timer;

void ABL::test(){    
//    testCache();
//    testTimerThread();
//    testSymbols();
//    testTransform();
//    testCalculusSymbols();
//    testMatrices();
    testCurves();
}

void message(void *vmsg)
{
    char *msg = (char*)vmsg;
    printf("Message: %s\n", msg);
}

void ABL::testCache()
{
    int card = 50;
    GCacheQueue q(card, 0);
    
    cout << q.toString() << endl;
    
    for (int i = 1; i <= card; i++)
        q.shift(i);
    
    cout << q.toString() << endl;
    
    q.shift(card+1);
    q.shift(card+2);
    cout << q.toString() << endl;
    
    double vals[card];
    q.getValues(vals);
    
    for (int i = 0; i < card; i++)
        cout << vals[i] << " ";
    cout << endl;
    
    q.getOrderedValues(vals);
    
    for (int i = 0; i < card; i++)
        cout << vals[i] << " ";
    cout << endl;
}

void ABL::testTimerThread()
{
    printf("Starting thread test\n");
    GTimerThread ttimer;
    
    ttimer.tickms(message, (char*)"First", 2000);
    ttimer.tickms(message, (char*)"Second", 6000);
    ttimer.tickms(message, (char*)"Third", 4000);

    sleep(8);
    printf("Enqueued @ %f.\n", ttimer.getTime());
}

void printSymTime(void *vsym)
{
    cout << "Printing" << endl;
    
    ABSymTime *sym = (ABSymTime *)vsym;
    
    sym->recalculate();
    
    cout << sym->toString() << endl;
}

void ABL::testSymbols()
{
    cout << "Testing symbols." << endl;
    
    {
    
        ABSymbol *syms[3];
        double nums[6] = {0, 1, 2, 3, 4, 5};
        syms[0] = new ABSymSet("numbers", 6, nums);
        
        vector<string> inputs;
        inputs.push_back("numbers");
        syms[1] = new ABSymMean("numbersMean", inputs);
        syms[1]->linkSymbol(0, syms[0]);
        
        printf("Just linked, no update: should be 0-5 and 0.\n");
        cout << syms[0]->toString() << endl;
        cout << syms[1]->toString() << endl;

        printf("Updating mean: should be 0-5 and 2.5\n");
        syms[1]->update();
        cout << syms[1]->toString() << endl;
        
        printf("Changing numbers, no update: should be 6,1-5 and 2.5\n");
        nums[0] = 6;
        cout << syms[0]->toString() << endl;
        cout << syms[1]->toString() << endl;
        
        printf("Updating mean: should be 6,1-5 and 3.5\n");
        syms[1]->update();
        cout << syms[1]->toString() << endl;
        
        delete syms[0];
        delete syms[1];
        
    }
    
    printf("Clearing and starting tick sym test (2 is smooth)\n");
    
    {
        
        GTimerThread ttimer;
        
        string names[] = {"time1", "time2"};
        vector<string> vect(names,names+2);
        
        ABSymbol *syms[3];
        syms[0] = new ABSymTime("time1", &timer, 0);
        syms[1] = new ABSymTime("time2", &timer, 40);
        syms[2] = new ABSymTick("tick", vect, &ttimer, .1);
        
        syms[2]->linkSymbol(0, syms[0]);
        syms[2]->linkSymbol(1, syms[1]);
        
        ((ABSymTick*)syms[2])->start();
        for (int i = 0; i < 8; i++) {
            cout << syms[0]->toString() << endl;
            cout << syms[1]->toString() << endl;
            sleep(1);
        }
        
        cout << "deleting symbols..." << endl;
        delete syms[0];
        delete syms[1];
        delete syms[2];
        cout << "Done!" << endl;
    }
}

bool pullTime(double *buff)
{
    *buff = timer.getTime();
    return true;
}

void ABL::testTransform()
{
    ABTransform transform(true);
    
    {
        double pos0[] = {0, 1, 2, 3, 4, 5};
        double pos1[] = {0, 1, 2, 3};
        double pos2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        
        ABSymbol *pos[3] = {
            new ABSymSet("pos0", 6, pos0),
            new ABSymSet("pos1", 4, pos1),
            new ABSymSet("pos2", 10, pos2)
        };
        
        ABSymbol *mean[3] = {
            new ABSymMean("mean0", "pos0"),
            new ABSymMean("mean1", "pos1"),
            new ABSymMean("mean2", "pos2")
        };
        
        string names[] = {"mean0", "mean1", "mean2"};
        vector<string> compInputs(names,names+3);
        ABSymbol *comp = new ABSymCombine("composite", compInputs);
        
        transform.addSymbols(pos, 2);
        transform.addSymbols(mean, 3);
        transform.addSymbol(comp);
        
        cout << "Added SymSet 1,2, three means, comp. No link. Should be 0, 0, 0." << endl;
        cout << "Comp: " << comp->toString() << endl;

        cout << "Now linking and updating, should be 2.5, 1.5, 0. (in both)" << endl;
        double vals[comp->getCard()];
        transform.getValues("composite", vals);
        printf("Vals1: %f %f %f\n", vals[0], vals[1], vals[2]);
        cout << "Comp: " << comp->toString() << endl;
        
        transform.addSymbol(pos[2]);
        
        cout << "Added third symbol, no linking. Should be same." << endl;
        cout << "Comp: " << comp->toString() << endl;
        
        cout << "Now linking. Should be 2.5, 1.5, 4.5." << endl;
        transform.getValues("composite", vals);
        printf("Vals3: %f %f %f\n", vals[0], vals[1], vals[2]);
        
        transform.deleteSymbols();
    }
    
    printf("Clearing and starting input test.\n");
    
    {
        string names[] = {"timepull", "time"};
        vector<string> compInputs(names, names+2);
        
        ABSymPull *timePull = new ABSymPull("timepull", 1, pullTime);
        ABSymTime *timeTime = new ABSymTime("time", &timer, 2);
        ABSymbol *comp = new ABSymCombine("timeComp", compInputs);
        transform.addSymbol(timePull);
        transform.addSymbol(timeTime);
        transform.addSymbol(comp);
        
        for (int i = 0; i < 8; i++) {
            double tvals[2];
            transform.getValues("timeComp", tvals);
            
            printf("TimePull: %f, TimeReg: %f\n", tvals[0], tvals[1]);
            sleep(1);
        }
        
        transform.deleteSymbols();
    }
}

static bool xFn(double *buf)
{
    buf[0] = timer.getTime() * 4.0;
    return true;
}

void ABL::testCalculusSymbols()
{
    GTimerThread ttimer;
    
    string names[] = { "x", "dx", "time" };
    
    ABSymPull *pull = new ABSymPull("x", 1, xFn);
    ABSymTime *time = new ABSymTime("time", &timer, 0);
    
    vector<string> vect(names, names+1);
    ABSymDifferentiate *diff = new ABSymDifferentiate("dx", vect, "time");
    
    vect = vector<string>(names+1,names+2);
    ABSymTick *tick = new ABSymTick("tick", vect, &ttimer, .01);
    
    ABTransform transform(true);
    transform.addSymbol(pull);
    transform.addSymbol(time);
    transform.addSymbol(diff);
    transform.addSymbol(tick);
    
    transform.startTick("tick");
    for (int i = 0; i < 10; i++) {
        double x, d, t[3];
        transform.getValues("x", &x);
        transform.getValues("dx", &d);
        transform.getValues("time", t);
        cout << "t  = " << t[0] << endl;
        cout << "x  = " << x << endl;
        cout << "dx = " << d << endl << endl;;
        sleep(1);
    }
}

void ABL::testMatrices()
{
    typedef techsoft::matrix<double> Matrix;
    typedef std::valarray<double> Vector;
    
    double t[] = { 1, 2, 3, 4, 5, 6 };
    double x[] = { 1., 2., 3., 4., 5., 7. };
    
    double m[6*3];
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 3; j++)
            m[i*3+j] = pow(t[i],j);
    
    Matrix A(6,3,m);
    Vector v(x, 6);
    
    cout << "Printing matrix" << endl;
    cout << A;
    cout << "Printing vector" << endl;
    for (int i = 0; i < 6; i++)
        cout << v[i] << " ";
    cout << endl;
    
    /*Matrix R;
    A.qrd(R);
    
    cout << "Q:" << endl << A << endl;
    cout << "R:" << endl << R << endl;
    */
    Vector s;
    if (A.solve_qr(v, s))
        printf("qr!\n");
    
    cout << "Solution:" << endl;
    for (int i = 0; i < 6; i++)
        cout << s[i] << " ";
    cout << endl;
    
}

void ABL::testCurves()
{
    string names[] = { "x", "time" };
    
    double t = 0.0;
    ABSymSet tSym("time", 1, &t);
    
    double x = 0.0;
    ABSymSet xSym("x", 1, &x);
    
    vector<string> v(names,names+1);
    ABSymCurve curve("curve", v, "time", 100, .01);
    curve.linkSymbol(0, &xSym);
    curve.linkSymbol(1, &tSym);
    
    /*for (int i = 1; i < 20; i++) {
        x = t = i * .1;
        curve.update();
        
        cout << curve.toString() << endl;
    }*/
    
    
    x = t = 0.0;
    int N = 1000;
    for (int i = 0; i < N; i++) {
        t = i * M_PI / 10;
        x = sin(t) * cos(2*t) + t*t*3;
        
        curve.update();
        //cout << curve.toString() << endl;
    }
    
    double adiff = 0.0;
    for (int i = 0; i < N; i++) {
        t = i * M_PI / 10;
        x = sin(t) * cos(2*t) + t*t*3;
        
        double vals[2];
        curve.getValues(vals, t);
        //cout << "x = " << vals[0] << ", t = " << vals[1] << endl;
        adiff += (x - vals[0])*(x - vals[0]);
        cout << "diff = " << x - vals[0] << endl;
    }
    adiff /= N;
    cout << "avdiff = " << adiff << endl;
    
}



