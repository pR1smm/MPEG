//
//  Image.cpp
//  MotionVectorizer
//
//  Created by Klaus Ulhaas on 21.05.14.
//  Copyright (c) 2014 Klaus Ulhaas. All rights reserved.
//

#include "Image.h"
#include <iostream>
#include <fstream>
#include <sstream>

Image::Image() {
    depth = 0;
    width = 0;
    height = 0;
    size = 0;
    data = 0;
}

// shallow copy
Image::Image(Image& img) {
    
    depth = img.depth;
    width = img.width;
    height = img.height;
    data = img.data;
    size = width*height*depth/8;
}

// deep copy
Image Image::clone() {
    
    Image img;
    img.depth = depth;
    img.width = width;
    img.height = height;
    img.size = width*height*depth/8;
    img.data = new unsigned char[size];
    memcpy(img.data, data, sizeof(unsigned char)*size);
    return img;
}

// allocates data buffer
Image::Image(int width, int height, int depth) {
    this->depth = depth;
    this->width = width;
    this->height = height;
    size = width*height*depth/8;
    data = new unsigned char[size];
    memset(data, 0, sizeof(unsigned char)*size);
}

// loads a grayscale image
// rough support of pgm-files
bool Image::loadPGM(std::string filename) {
    
    std::ifstream ifile;
    ifile.open(filename,std::ios::binary);
    if(!ifile.is_open()) {
        std::cerr << "Could not load image file with name "
                  << filename << std::endl;
        return false;
    }
    // Read magic key that should be P5
    char magic_key[3];
    ifile.get(magic_key, 3); // reads two characters
    
    if(magic_key[0] != 'P' || magic_key[1] != '5') {
        std::cerr << "The image file format "
        << filename << " is not supported! " << std::endl;
        return false;
    }
    // remove whitespace, linefeeds, ...
    ifile >> std::skipws;
    
    std::string widthStr;
    ifile >> widthStr;
    std::istringstream buffer1(widthStr);
    buffer1 >> width;

    // remove whitespace, linefeeds, ...
    ifile >> std::skipws;
    
    std::string heightStr;
    ifile >> heightStr;
    std::istringstream buffer2(heightStr);
    buffer2 >> height;
    
    // remove whitespace, linefeeds, ...
    ifile >> std::skipws;
    
    int maxVal;
    std::string maxValStr;
    ifile >> maxValStr;
    std::istringstream buffer3(maxValStr);
    buffer3 >> maxVal;
    
    if(maxVal > 255) {
        std::cerr << "The image file format "
        << filename << " is not supported! " << std::endl;
        return false;
    }
    
    // remove whitespace, linefeeds, ...
    ifile >> std::skipws;
   
    size = width*height*2;
    data = new unsigned char[size+1];
    for (int i=0; i<size; ++i ) {
        ifile >> std::noskipws >> data[i];
    }
    
    ifile.close();
    depth = 8;
    return true;
}

// writes a grayscale image
bool Image::writePGM(std::string filename) {
    
    std::ofstream ofile;
    ofile.open(filename,std::ios::binary);
    if(!ofile.is_open()) {
        std::cerr << "Could not create image file with name "
        << filename << std::endl;
        return false;
    }

    ofile << "P5" << ' ' << width << ' ' << height << ' ' << "255" << '\n';
    for (int i=0; i<size; ++i ) {
        ofile << data[i];
    }
    ofile.close();
    
    return true;
}
