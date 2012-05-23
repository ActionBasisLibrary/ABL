//
//  ABL.h
//  ABL
//
//  Created by Matt Rubin on 4/24/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_h
#define ABL_h

#include "ABTransform.h"

#include "ABSymbol.h"
#include "ABSymSet.h"
#include "ABSymTime.h"
#include "ABSymPull.h"
#include "ABSymVarPull.h"
#include "ABSymMean.h"
#include "ABSymCombine.h"
#include "ABSymTick.h"
#include "ABSymDifferentiate.h"

#include "GTimer.h"
#include "GTimerThread.h"
#include "GCacheQueue.h"

class ABL {
public:
	static void test();
    
private:
    static void testTimerThread();
    static void testSymbols();
    static void testTransform();
    static void testCache();
    static void testCalculusSymbols();
};

#endif
