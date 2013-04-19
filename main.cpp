//
//  main.cpp
//  HarrisCornerDetection
//
//  Created by Gary Fenstamaker on 2/12/13.
//  Copyright (c) 2013 Gary Fenstamaker. All rights reserved.
//

#include <iostream>
#include <string>
#include <Magick++.h>
#include <armadillo>
#include "gImage.h"
#include "Comparator.h"

int main(int argc, const char * argv[]) {
    Magick::InitializeMagick(*argv);
    
    std::string firstImageName = argv[1];
    std::string secondImageName = argv[2];
    std::string outputFolder = argv[3];
    
    std::cout << "Creating new image" << std::endl;
    gImage *firstImage = new gImage(firstImageName);
    gImage *secondImage = new gImage(secondImageName);
    //gImage *secondImage = new gImage("/Users/fenstamaker/Desktop/Images/10.jpg");
    
    std::cout << "Finding corners in images" << std::endl;
    firstImage->findCorners(100000000000);
    firstImage->removeCorners();
    secondImage->findCorners(100000000000);
    
    std::cout << "Saving marked images" << std::endl;
    firstImage->save(outputFolder + "/" + firstImageName);
    secondImage->save(outputFolder + "/" + secondImageName);
    
    
    std::cout << "Creating new Comparator" << std::endl;
    Comparator *comparator = new Comparator(firstImage);
    
    std::cout << "Comparing first image with second image" << std::endl;
    bool eric = comparator->compareWith(secondImage);
    
    std::cout << "Finished!" << std::endl;
    if ( eric ) {
        std::cout << "Match!" << std::endl;
    } else {
        std::cout << "No Match!" << std::endl;
    }
    
    return 0;
}

