//
//  ABL.cpp
//  ABL
//
//  Created by Matt Rubin on 4/24/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include <iostream>
#include "ABL.h"
#include "ABTransform.h"

void ABL::test(){
	printf("ABL is linked.\n");
}

void ABL::testSymbols()
{
    printf("Testing symbols.\n");
    
    ABSymbol *sym = new ABSymSingle("position", 2);
    
    float pos[] = {1.0, 2.0};
    
    ((ABSymSingle*)sym)->setValues(pos);
    
    printf("Values: %f %f\n", sym->getValues()[0], sym->getValues()[1]);
    
}

void ABL::testTransform()
{
    
    
    
}