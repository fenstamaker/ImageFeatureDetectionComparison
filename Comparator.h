//
//  Comparator.h
//  HarrisCornerDetection
//
//  Created by Gary Fenstamaker on 2/26/13.
//  Copyright (c) 2013 Gary Fenstamaker. All rights reserved.
//

#ifndef __HarrisCornerDetection__Comparator__
#define __HarrisCornerDetection__Comparator__

#include "gImage.h"
#include <iostream>
#include <vector>

class Comparator {
    
public:
    Comparator(gImage *image);
    
    bool compareWith(gImage *image);
    
private:
    gImage *baseImage;
    
};

#endif /* defined(__HarrisCornerDetection__Comparator__) */
