//
//  gImage.h
//  HarrisCornerDetection
//
//  Created by Gary Fenstamaker on 2/13/13.
//  Copyright (c) 2013 Gary Fenstamaker. All rights reserved.
//

#ifndef __HarrisCornerDetection__gImage__
#define __HarrisCornerDetection__gImage__

#include <iostream>
#include <string>
#include <vector>
#include <Magick++.h>
#include <armadillo>

class gImage {
    
public:
    gImage(std::string path);
    
    Magick::ColorRGB getColor(int x, int y); // Gets the RGB color of the pixel at that point
    int getIntensity(int x, int y); // Gets the luma values of the pixel at that point
    void mark(int x, int y);

    long int response(int x, int y); // Gives the response of the corner detection
    void findCorners(long threshold);
    void removeCorners();

    void sync(); // Saves any changes to the exisiting image file
    bool save(std::string path); // Saves any changes to a given path
    
    // Getters
    int getWidth();
    int getHeight();
    
    std::vector< std::vector< std::vector<float> > > decomposedWindows;
    int windowSize;
    
private:
    std::string file;
    std::vector< std::vector<int> > imageGradientX;
    std::vector< std::vector<int> > imageGradientY;
    std::vector< std::vector<arma::mat> > imageMatricies;
    
    int width;
    int height;
    
    Magick::Image image;
    Magick::PixelPacket *pixels;
    
    
    void sobel();
    void generateMatricies();
    arma::mat windowMatrixSum(int x, int y);
    void decompose(int x, int y, int window);
    std::vector< std::vector< std::vector<float> > > trimWindows();
    std::vector< std::vector<float> > flatten( std::vector< std::vector<float> > window);
    
};

#endif /* defined(__HarrisCornerDetection__gImage__) */
