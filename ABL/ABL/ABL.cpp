//
//  ABL.cpp
//  ABL
//
//  Created by Matt Rubin on 4/24/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include <iostream>
#include <time.h>
#include "ABL.h"

void ABL::test(){
	printf("ABL is linked.\n");
    //testSymbols();
    testTransform();
}

void ABL::testSymbols()
{
    printf("Testing symbols.\n");
    
    ABSymbol *syms[3];
    syms[0] = new ABSymSingle("numbers", 6);
    float nums[6] = {0, 1, 2, 3, 4, 5};
    ((ABSymSingle*)syms[0])->setValues(nums);
    
    vector<string> inputs;
    inputs.push_back("numbers");
    syms[1] = new ABSymMean("numbersMean", inputs);
    syms[1]->linkSymbol(0, syms[0]);
    
    printf("Just linked, no update.\n");
    cout << syms[0]->toString() << endl;
    cout << syms[1]->toString() << endl;

    printf("Updating mean.\n");
    syms[1]->update();
    cout << syms[1]->toString() << endl;
    
    printf("Updating numbers.\n");
    nums[0] = 6;
    ((ABSymSingle*)syms[0])->setValues(nums);
    cout << syms[0]->toString() << endl;
    cout << syms[1]->toString() << endl;
    
    printf("Updating mean.\n");
    syms[1]->update();
    cout << syms[1]->toString() << endl;
    
    delete syms[0];
    delete syms[1];
}

bool pullTime(float *buff)
{
    *buff = clock();
    return true;
}

void ABL::testTransform()
{
    ABTransform transform(true);
    
    ABSymbol *pos[3] = {
        new ABSymSingle("pos0", 6),
        new ABSymSingle("pos1", 4),
        new ABSymSingle("pos2", 10)
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
    
    float pos0[] = {0, 1, 2, 3, 4, 5};
    float pos1[] = {0, 1, 2, 3};
    float pos2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    const float *vals = transform.getValue("composite");
    printf("Vals: %f %f %f\n", vals[0], vals[1], vals[2]);
    cout << comp->toString() << endl;
    
    ((ABSymSingle*)pos[0])->setValues(pos0);
    ((ABSymSingle*)pos[1])->setValues(pos1);
    ((ABSymSingle*)pos[2])->setValues(pos2);
    
    const float *vals2 = transform.getValue("composite");
    printf("Vals: %f %f %f\n", vals2[0], vals2[1], vals2[2]);
    cout << comp->toString() << endl;
    
    transform.addSymbol(pos[2]);
    
    const float *vals3 = transform.getValue("composite");
    printf("Vals: %f %f %f\n", vals3[0], vals3[1], vals3[2]);
    cout << comp->toString() << endl;
    
    transform.deleteSymbols();
    
    printf("Starting input test.\n");
    
    ABSymPull *timeSym = new ABSymPull("time", 1, pullTime);
    ABSymMean *timeMean = new ABSymMean("timeMean", "time");
    transform.addSymbol(timeSym);
    transform.addSymbol(timeMean);
    
//    for (int i = 0; i < 1000; i++)
//        printf("TimeMean: %f\n", transform.getValue("timeMean")[0]);
}