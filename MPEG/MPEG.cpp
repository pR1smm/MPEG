//
//  main.cpp
//  MotionVectorizer
//
//  Created by Klaus Ulhaas on 21.05.14.
//  Copyright (c) 2014 Klaus Ulhaas. All rights reserved.
//

#include <iostream>
#include "Image.h"
#include "stdafx.h"
#include <vector>




bool reconstructImageByMotion(Image& result, Image& imagePrev, Image& imageCurr) 
{

	int w = imagePrev.getWidth();
	int h = imagePrev.getHeight();
	if (imageCurr.getWidth() != w || imageCurr.getHeight() != h) {
		return false;
	}

	result = imagePrev.clone();

	std::cout << "Hoehe: " << w << ", " << "Breite: " << h << std::endl;
	
	//turn raw data of current image into vector for easier handling
	unsigned char * dataArrayCurrent = imageCurr.getBuffer();
	std::vector<std::vector<int>> vectorDataCurrent(imageCurr.getHeight(), std::vector<int>(imageCurr.getWidth()));
	int arrayCounter = 0;

	for (int i = 0; i < imageCurr.getHeight(); i++)
	{
		for (int j = 0; j < imageCurr.getWidth(); j++)
		{
			vectorDataCurrent.at(i).at(j) = (int)dataArrayCurrent[arrayCounter];
			arrayCounter++;
		}
		
	}

	//turn raw data of previous image into vector for easier handling
	unsigned char * dataArrayPrevious = imagePrev.getBuffer();
	std::vector<std::vector<int>> vectorDataPrevious(imagePrev.getHeight(), std::vector<int>(imagePrev.getWidth()));
	arrayCounter = 0;

	for (int i = 0; i < imagePrev.getHeight(); i++)
	{
		for (int j = 0; j < imagePrev.getWidth(); j++)
		{
			vectorDataPrevious.at(i).at(j) = (int)dataArrayPrevious[arrayCounter];
			arrayCounter++;
		}
		
	}
	
	// create vector which is able to accommodate vectors of type MxN
	std::vector<std::vector<std::vector<int>>> vectorPixelBocks;
    
	// create temp-vector for 16x16 pixel-block
	std::vector<std::vector<int>> tempBlock(16, std::vector<int>(16));
	
	// fill vectorPixelBlocks with 16x16 pixel-blocks
	for (int line = 0; line < imageCurr.getHeight(); line += 16)
	{
		for (int col = 0; col < imageCurr.getWidth(); col += 16)
		{
			for (int i = 0; i < 16; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					tempBlock.at(i).at(j) = vectorDataCurrent.at(line + i).at(col + j);
				}
			}
			vectorPixelBocks.push_back(tempBlock);
		}
	}

	// check if current image has been corrupted by conversion into vector 
	
	
	unsigned char * currDataArray;
	currDataArray = (unsigned char*)malloc(imageCurr.getSize() * sizeof(unsigned char));
	int currDataArrayPointer = 0;
	for (int i = 0; i < imageCurr.getHeight(); i++)
	{
		for (int j = 0; j < imageCurr.getWidth(); j++)
		{
			currDataArray[currDataArrayPointer++] = vectorDataCurrent.at(i).at(j);
		}
	}
	result.setBuffer(dataArrayCurrent);
	

	// initiate SAE-search between imageCurr and imagePrev







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

	if (retVal1 & retVal2) {
		if (reconstructImageByMotion(result, imagePrev, imageCurr)) {
			result.writePGM("res.pgm");
		}
	}

	return 0;
}

