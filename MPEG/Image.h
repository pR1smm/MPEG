//
//  Image.h
//  MotionVectorizer
//
//  Created by Klaus Ulhaas on 21.05.14.
//  Copyright (c) 2014 Klaus Ulhaas. All rights reserved.
//

#ifndef __MotionVectorizer__Image__
#define __MotionVectorizer__Image__

#include <string>


class Image {
    
protected:
    
    int width, height;   // Dimensions of the image
    int depth;           // Image depth in Bits
    unsigned char* data; // Image data
    int size;            // Image data size
    
public:
    
    Image();
    
    // shallow copy
    Image(Image& img);
    
    // deep copy
    Image clone();
    
    // allocates data buffer and sets memory to zero
    Image(int width, int height, int depth=8);
    
    // loads a grayscale image
    bool loadPGM(std::string filename);
    
    // writes a grayscale image
    bool writePGM(std::string filename);

    // returns the image buffer
    unsigned char* getBuffer() {return data;}
	void setBuffer(unsigned char myData[]) { data = myData;}

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getDepth() const { return depth; }
    int getSize() const { return size; }
};

#endif /* defined(__MotionVectorizer__Image__) */
