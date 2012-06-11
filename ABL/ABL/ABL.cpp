//
//  ABL.cpp
//  ABL
//
//  Created by Matt Rubin on 4/24/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include <iostream>
#include "ABL.h"

#include "External/cmatrix"

GTimer timer;

void ABL::test(){    
//    testCache();
//    testTimerThread();
//    testSymbols();
//    testTransform();
//    testCalculusSymbols();
//    testMatrices();
//    testCurves();
    testCurvature();
}

void message(void *vmsg)
{
    char *msg = (char*)vmsg;
    printf("Message: %s\n", msg);
}

void ABL::testCache()
{
    int card = 5;
    GCacheQueue q(card, 0);
    
    cout << q.toString() << endl;
    
    for (int i = 1; i <= card; i++)
        q.shift(i);
    
    cout << q.toString() << endl;
    cout << "Sum: " << q.getSum() << endl;
    
    q.shift(card+1);
    q.shift(card+2);
    cout << q.toString() << endl;
    cout << "Sum: " << q.getSum() << endl;
    
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
        syms[0] = new ABSymTime("time1", &timer);
        syms[1] = new ABSymTime("time2", &timer);
        syms[2] = new ABSymTick("tick", vect, &ttimer, .1);
        
        syms[2]->linkSymbol(0, syms[0]);
        syms[2]->linkSymbol(1, syms[1]);
        
        ((ABSymTick*)syms[2])->start();
        for (int i = 0; i < 8; i++) {
            cout << syms[0]->toString() << endl;
            cout << syms[1]->toString() << endl;
            sleep(1);
        }
        
        cout << "Testing stopped tick (should stay constant)" << endl;
        ((ABSymTick*)syms[2])->stop();
        for (int i = 0; i < 4; i++) {
            cout << syms[0]->toString() << endl;
            cout << syms[1]->toString() << endl;
            sleep(1);
        }
        
        cout << "And restart (should jump to correct time)" << endl;
        ((ABSymTick*)syms[2])->start();
        for (int i = 0; i < 4; i++) {
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


static bool calcxyz(double *buff, double t)
{
    buff[0] = sin(t * 2*M_PI);
    buff[1] = cos(t * 4*M_PI)*sin(t*M_PI);
    buff[2] = 2*t;
    return true;
}
static bool pullxyz(double *buff)
{
    return calcxyz(buff, timer.getTime());
}

void ABL::testTransform()
{
    ABTransform transform(true);
    
    if (false) {
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
    
    
    if (false) {
        cout << "Clearing and starting input test." << endl;;
        string names[] = {"timepull", "time"};
        vector<string> compInputs(names, names+2);
        
        ABSymPull *timePull = new ABSymPull("timepull", 1, pullTime);
        ABSymTime *timeTime = new ABSymTime("time", &timer);
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
    
    {
        cout << "Starting curve test" << endl;
        
        GTimerThread ttimer;
        vector<string> vect;
        
        // Creating all of the symbols
        
        ABSymPull *xyzPull = new ABSymPull("xyz", 3, pullxyz);
        
        ABSymTime *time = new ABSymTime("time", &timer);
        
        vect.push_back("xyz");
        vect.push_back("curve");
        ABSymTick *tick = new ABSymTick("tick", vect, &ttimer, .005);
        
        ABSymCurve *curve = new ABSymCurve("curve", 3, "xyz", "time", 400, .001);
        
        // Loading the transform
        transform.addSymbol(xyzPull);
        transform.addSymbol(time);
        transform.addSymbol(tick);
        transform.addSymbol(curve);
        
        // Run the timer
        int sp = 8;
        cout << "Starting the clock and sleeping for " << sp << " secs" << endl;
        timer.resetTime();
        transform.startTick("tick");
        
        double tmin = timer.getTime() + .1;
        
        sleep(sp);
        
        double tmax = timer.getTime() - .1;
        transform.stopTick("tick");
        
        // Now recap and check results
        cout << "Stopped and printing: " << endl;
        
        curve->printStats();
        cout << endl;
        
        int N = 6;
        for (int i = 0; i <= N; i++) {
            double t = tmin + i*(tmax - tmin)/N;
            double xyz[3], xyz0[3];
            
            transform.getValues("curve", xyz, t);
            calcxyz(xyz0, t);
            
            cout << "actual   : " << xyz0[0] << '\t' << xyz0[1] << '\t' << xyz0[2] << endl;
            cout << "transform: " << xyz[0] << '\t' << xyz[1] << '\t' << xyz[2] << '\n' << endl;
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
    ABSymTime *time = new ABSymTime("time", &timer);
    
    vector<string> vect(names, names+1);
    ABSymDifferentiate *diff = new ABSymDifferentiate("dx", 1, "x", "time");
    
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
    double t = 0.0;
    ABSymSet tSym("time", 1, &t);
    
    double x = 0.0;
    ABSymSet xSym("x", 1, &x);
    
    ABSymCurve curve("curve", 1, "x", "time", 100, .01);
    curve.linkSymbol(0, &xSym);
    curve.linkSymbol(1, &tSym);
    
    int N = 1000;
    double adiff = 0.0;
    x = t = 0.0;
    for (int i = 0; i < N; i++) {
        t = i * M_PI / 10;
        x = sin(t) * cos(2*t) + t*t*3;
        
        curve.update();
    }

    for (int i = 0; i < N; i++) {
        t = i * M_PI / 10;
        x = sin(t) * cos(2*t) + t*t*3;
        
        double vals[2];
        curve.getValues(vals, t);
        //cout << "x = " << vals[0] << ", t = " << vals[1] << endl;
        adiff += (x - vals[0])*(x - vals[0]);
//        cout << "diff = " << x - vals[0] << endl;
    }
    adiff /= N;
    cout << "trig avdiff = " << adiff << endl;
    
    curve.clear();
    
    // Now run singularity tests
    for (int i = 0; i < N; i++) {
        t = i * 10.0 / N;
        x = 1.0;
        
        curve.update();
    }
    
    adiff = 0.0;
    for (int i = 0; i < N; i++) {
        t = i * 10.0 / N;
        x = 1.0;
        
        double val;
        curve.getValues(&val, t);
        
        adiff += (x - val)*(x - val);
    }
    adiff /= N;
    cout << "singular avdiff = " << adiff << endl;
    
    curve.clear();
    
}

void ABL::testCurvature()
{
    double xval[3], time;
    
    ABSymSet xsym("x", 3, xval);
    ABSymSet tsym("time", 1, &time);
    ABSymCurvature Ksym("K", "x", 3, "time");
    
    ABTransform T;
    T.addSymbol(&xsym);
    T.addSymbol(&tsym);
    T.addSymbol(&Ksym);
    
    int N = 100;
    for (int i = 0; i < N; i++) {
        time = (M_PI * i) / N;
        
        xval[0] = time;
        xval[1] = time*time;
        xval[2] = time*time;
        
        double K;
        T.getValues("K", &K);
        
        cout << "t: " << time << " ";
        cout << "x: " << xval[0] << " " << xval[1] << " " << xval[2] << " ";
        cout << "K: " << K << endl;
    }
    
}