//
//  main.cpp
//  dense_points
//
//  Created by jaox on 5/4/19.
//  Copyright © 2019 jaox_developer_ios. All rights reserved.
//

#include <iostream>
#include "ParallelSamplingDensePoints.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    srand((unsigned int)time(NULL));
    
    //std::cout << "Hello, World!\n";
    
    ParallelSamplingDensePoints PSDP;
    
    PSDP.generateDensePoints();
    
    return 0;
}
