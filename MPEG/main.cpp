//
//  main.cpp
//  MotionVectorizer
//
//  Created by Klaus Ulhaas on 21.05.14.
//  Copyright (c) 2014 Klaus Ulhaas. All rights reserved.
//

#include <iostream>
#include "Image.h"




bool reconstructImageByMotion(Image& result,
                              Image& imagePrev,
                              Image& imageCurr) {
    
    int w = imagePrev.getWidth();
    int h = imagePrev.getHeight();
    if (imageCurr.getWidth() != w || imageCurr.getHeight() != h) {
        return false;
    }
    
    result = imagePrev.clone();
    
    /* 
         Hier folgt Ihr Code um für einen 16x16 Block des Bildes imageCurr einen 
         moeglichst aehnlichen Block im Bild imagePrev zu finden.
         Dann schreiben Sie den ähnlichsten Block des vorausgehenden Bildes an die 
         Blockposition des aktuellen Bildes in das Bild result.
     
         Achtung: Die Blockposition des Zielbildes bzw. des Bildes imageCurr ist 
         fuer x und y durch 16 teilbar. Aber, die Blockposition des aehnlichsten 
         Blockes im Bild imagePrev kann jede Pixelposition einnehmen und ist somit 
         nicht zwangslaeufig durch 16 teilbar.
     */
    
    
    return true;
}



int main(int argc, const char * argv[])
{
    bool retVal1, retVal2;
    Image imagePrev, imageCurr, result;
    retVal1 = imagePrev.loadPGM("im1.pgm");
    retVal2 = imageCurr.loadPGM("im2.pgm");
    
    if(retVal1 & retVal2) {
        if(reconstructImageByMotion(result, imagePrev, imageCurr)) {
            result.writePGM("res.pgm");
        }
    }
    
    return 0;
}

