//
//  gImage.cpp
//  HarrisCornerDetection
//
//  Created by Gary Fenstamaker on 2/13/13.
//  Copyright (c) 2013 Gary Fenstamaker. All rights reserved.
//

#include "gImage.h"
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cfloat>
#include <armadillo>

gImage::gImage(std::string path) {
    
    windowSize = 8;
    
    file = path;
    Magick::Image image2(path);
    image = image2;
    width = (int)image.columns();
    height = (int)image.rows();
    std::cout << "Height " << height << std::endl;
    pixels = image.getPixels(0, 0, width, height);
    
    imageGradientX.resize(width);
    imageGradientY.resize(width);
    imageMatricies.resize(width);
    for (int i = 0; i < width; i++) {
        imageGradientX[i].resize(height);
        imageGradientY[i].resize(height);
        imageMatricies[i].resize(height);
    }
    
    sobel();
    generateMatricies();
}

Magick::ColorRGB gImage::getColor(int x, int y) {
    Magick::Color color;
    
    color = pixels[width * y + x];
    
    Magick::ColorRGB rgb(color);
    return rgb;
}

int gImage::getIntensity(int x, int y) {
    double luma = 0;
    
    //Magick::ColorRGB rgb = getColor(x, y);
    
    Magick::Color color;
    
    color = pixels[width * y + x];
    
    Magick::ColorRGB rgb(color);
    
    luma =  (rgb.red()   * 256) * 0.2126 +
            (rgb.green() * 256) * 0.7152 +
            (rgb.blue()  * 256) * 0.0722;
    
    return (int)luma;
}

void gImage::mark(int x, int y) {
    pixels[width * y + x] = Magick::Color("red");
    
    pixels[width * (y - 1) + x] = Magick::Color("red");
    
    pixels[width * y + (x - 1)] = Magick::Color("red");
    
    pixels[width * (y + 1) + x] = Magick::Color("red");
    
    pixels[width * y + (x + 1)] = Magick::Color("red");
}

void gImage::sobel() {
    int sobelHorizontal = 0;
	int sobelVertical = 0;
    
	for ( int y = 1; y < height - 2; y++ ) {
        
		for ( int x = 1; x < width - 2; x++ ) {
            
			// Optimized Sobel operation
			sobelHorizontal = getIntensity(x+1, y-1) + getIntensity(x+1, y+1)
                            - getIntensity(x-1, y-1) - getIntensity(x-1, y+1)
                            + 2 * getIntensity(x+1, y) - 2 * getIntensity(x-1, y);
            
			sobelVertical   = getIntensity(x+1, y+1) + getIntensity(x-1, y+1)
                            - getIntensity(x+1, y-1) - getIntensity(x-1, y-1)
                            + 2 * getIntensity(x, y+1) - 2 * getIntensity(x-1, y-1);
            
            //std::cout << sobelHorizontal << " ";
            imageGradientX[x][y] = sobelHorizontal;
            imageGradientY[x][y] = sobelVertical;
        }
    }
    
}

void gImage::generateMatricies() {
    for ( int y = 1; y < height - 1; y++ ) {
        for ( int x = 1; x < width - 1; x++ ) {
            int ix2 = imageGradientX[x][y]*imageGradientX[x][y];
            int iy2 = imageGradientY[x][y]*imageGradientY[x][y];
            int ixy = imageGradientX[x][y]*imageGradientY[x][y];
            
            arma::mat H;
            
            H << ix2 << ixy << arma::endr
              << ixy << iy2 << arma::endr;
            
            imageMatricies[x][y] = H;
        }
    }
}

arma::mat gImage::windowMatrixSum(int x, int y) {
    int radius = 5;
    
    arma::mat H;
    H << 0 << 0 << arma::endr << 0 << 0 << arma::endr;
    
    for ( int i = 0; i < radius; i++ ) {
        for ( int j = 0; j < radius; j++ ) {
            if ( ( x-(radius/2)+j < width - 1  && x-(radius/2)+j > 1 ) &&
                 ( y-(radius/2)+i < height - 1 && y-(radius/2)+i > 1 ) ) {
                
                H = H + imageMatricies[x-(radius/2)+j][y-(radius/2)+i];
                
            }
        }
    }
    
    return H;
    
}

long int gImage::response(int x, int y) {
    long r = 0;
    double k = 0.05;
    
    arma::mat H = windowMatrixSum(x, y);
    
    r = arma::det(H) - k * (arma::trace(H)*arma::trace(H)); 
    
    return r;
}

void gImage::removeCorners() {
    
    
    
    
    
}


void gImage::findCorners(long threshold) {
    
    for ( int y = windowSize; y < height-windowSize; y+= windowSize/2 ) {
        for ( int x = windowSize; x < width-windowSize; x++ ) {
            long r = response(x, y);
            if ( r > threshold ) {
                decompose(x, y, windowSize);
                mark(x, y);
             //   std::cout << x << " " << y << " mark" << std::endl;
                //y += windowSize/2;
                x += windowSize/2;
            }
        }
    }
   // sync();
  //  std::cout << height << " " << width << std::endl;
    //std::cin >> height;
}

void gImage::decompose(int x, int y, int window) {
    
    if ( window % 2 != 0 ) {
        std::cout << "Window is not even!" << std::endl;
        return;
    }
    
    int halfWindow = window / 2;
    
    int startX = x - halfWindow + 1;
    int startY = y - halfWindow + 1;
    int endX = x + halfWindow;
    int endY = y + halfWindow;
    
    /*
    std::cout << "window: " << window << std::endl;
    std::cout << "halfwindow: " << halfWindow << std::endl;
    std::cout << "startX: " << startX << std::endl;
    std::cout << "X: " << x << std::endl;
    std::cout << "endX: " << endX << std::endl;
    std::cout << "startY: " << startY << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "endY: " << endY << std::endl;
     */
    
    std::vector< std::vector<float> > tempWindow;
    tempWindow.resize(window);
    
    for ( int i = 0; i < window; i++ ) {
        tempWindow[i].resize(window);
    }
    
    for ( int j = startY; j <= endY; j++ ) {
        for ( int i = startX; i <= endX; i++ ) {
            tempWindow[i-startX][j-startY] = getIntensity(i, j);
        }
    }
    
    /*
    std::cout << "---Original Region----" << std::endl;
    for ( int j = 0; j < tempWindow[0].size(); j++ ) {
        for ( int i = 0; i < tempWindow.size(); i++ ) {
            std::cout << std::setw(5) << std::setprecision(0) << std::fixed << (int)tempWindow[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */
     
    std::vector<float> averages;
    std::vector<float> details;
    
    while ( window > 1 ) {
        
        // ROW DECOMPOSE
        for ( int j = 0; j < window; j++ ) {
            
            averages.clear();
            details.clear();
            
            for ( int i = 0; i < window; i += 2 ) {
                int firstValue = tempWindow[i][j];
                int secondValue = tempWindow[i+1][j];
                float average = 0;
                
                average = (firstValue + secondValue) / 2;
                
                float detail = (float)firstValue - average;
                
                averages.push_back(average);
                details.push_back(detail);
            }
            
            for ( int i = 0; i < averages.size(); i++ ) {
                tempWindow[i][j] = averages[i];
                tempWindow[i+(window/2)][j]  = details[i];
            }
            
        }
        
        // COL DECOMPOSE
        for ( int j = 0; j < window; j++ ) {
            
            averages.clear();
            details.clear();
            
            for ( int i = 0; i < window; i += 2 ) {
                int firstValue = tempWindow[j][i];
                int secondValue = tempWindow[j][i+1];
                float average = 0;
                
                average = (firstValue + secondValue) / 2;
                
                float detail = ((float)firstValue) - average;
                
                averages.push_back(average);
                details.push_back(detail);
            }
            
            for ( int i = 0; i < averages.size(); i++ ) {
                tempWindow[j][i] = averages[i];
                tempWindow[j][i+(window/2)]  = details[i];
            }
            
        }
        
        window = window / 2;
        
    }
    
    /*
    std::cout << "---Haar Wavelet----" << std::endl;
    for ( int j = 0; j < tempWindow[0].size(); j++ ) {
        for ( int i = 0; i < tempWindow.size(); i++ ) {
            std::cout << std::setw(5) << std::setprecision(0) << std::fixed <<  tempWindow[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
     */
    
    std::vector < std::vector<float> > temp = flatten(tempWindow);
    
    /*
    std::cout << "---Flattened Window----" << std::endl;
    for ( int j = 0; j < temp[0].size(); j++ ) {
        for ( int i = 0; i < temp.size(); i++ ) {
            std::cout << std::setw(5) << std::setprecision(0) << std::fixed <<  temp[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
     */
    
    decomposedWindows.push_back( temp );
    
}

std::vector< std::vector<float> > gImage::flatten( std::vector< std::vector<float> > window) {
    
    std::vector< std::vector<float> > result;
    
    result.resize(window.size());
    for ( int i = 0; i < result.size(); i++ ) {
        result[i].resize(window[i].size());
    }
    
    float max = 0;
    float min = FLT_MAX;
    
    for ( int x = 0; x < window.size(); x++ ) {
        for ( int y = 0; y < window[x].size(); y++ ) {
            if ( x == 0 && y == 0 )
                continue;
            
            int value = abs(window[x][y]);
            
            if ( value < min ) {
                min = value;
            }
            if ( value > max ) {
                max = value;
            }
            
        }
    }
    
    float medium = (max - min)/2 + min;
    
    
    for ( int x = 0; x < window.size(); x++ ) {
        for ( int y = 0; y < window[x].size(); y++ ) {
            
            int value = abs(window[x][y]);
            
            if ( value > medium ) {
                if ( window[x][y] < 0 ) {
                    result[x][y] = -1;
                } else {
                    result[x][y] = 1;
                }
            } else {
                result[x][y] = 0;
            }
            
        }
    }
    
    return result;
    
}

std::vector< std::vector< std::vector<float> > > gImage::trimWindows(){
    std::vector< std::vector< std::vector<float> > > windows;
    
    while ( !decomposedWindows.empty() ) {
        int r = rand() % decomposedWindows.size();
        
        std::vector< std::vector<float> > window = decomposedWindows[r];
        decomposedWindows.erase(decomposedWindows.begin()+r-1);
        for ( int i = 0; i < windows.size(); i++ ) {
            if ( window[windowSize/2][windowSize/2] > windows[i][windowSize/2][windowSize/2] - (windowSize/2) ) {
                
            }
        }
    }
    
    return windows;
}

void gImage::sync() {
    image.syncPixels();
}

bool gImage::save(std::string path) {
    bool success = true;
    sync();
    image.write(path);
    
    return success;
}

int gImage::getWidth() {
    return width;
}

int gImage::getHeight() {
    return height;
}