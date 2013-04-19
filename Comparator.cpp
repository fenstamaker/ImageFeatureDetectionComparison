//
//  Comparator.cpp
//  HarrisCornerDetection
//
//  Created by Gary Fenstamaker on 2/26/13.
//  Copyright (c) 2013 Gary Fenstamaker. All rights reserved.
//

#include "Comparator.h"

Comparator::Comparator(gImage *image) {
    baseImage = image;
}

bool Comparator::compareWith(gImage *image) {
    bool compared = true;
    
    int numberOfElements = baseImage->windowSize * baseImage->windowSize;
    int windowMatch = 0;
    
    for ( int i = 0; i < baseImage->decomposedWindows.size(); i++ ) {
        
        std::cout << "Baseimage Window: " << i << "/" << baseImage->decomposedWindows.size() <<
            " " << windowMatch << std::endl;
        
        for ( int j = 0; j < image->decomposedWindows.size(); j++ ) {
            
            //std::cout << "Comparing with image Window: " << j << std::endl;
            
            int matches = 0;
            int baseNonZeros = 0;
            int imageNonZeros = 0;
            
            for ( int x = 0; x < baseImage->windowSize; x++ ) {
                for ( int y = 0; y < baseImage->windowSize; y++ ) {
                    // Compare against zeros
                    float baseValue =  baseImage->decomposedWindows[i][x][y];
                    float imageValue = image->decomposedWindows[j][x][y];
                    
                    if ( baseValue == 0){//|| imageValue == 0 ) {
                        continue;
                    }
                    
                    if ( baseValue != 0 )
                        baseNonZeros++;
                    
                    if ( baseValue == imageValue ) {
                        matches++;
                    }
                }
            }
            
            float percentage = (float)matches/baseNonZeros;
            
            if ( percentage > .5 ) {
                windowMatch++;
                break;
            }
            
        }
        
    }
    
    float chance = (float)windowMatch/baseImage->decomposedWindows.size();
    
    if ( chance > .50 ) {
        compared = true;
    } else {
        compared = false;
    }
    
    return compared;
}