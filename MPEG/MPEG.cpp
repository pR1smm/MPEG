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

//implements SAE-function between two 16x16 pixel-blocks
int SAEfunction(std::vector<std::vector<int>> previousBlock, std::vector<std::vector<int>> currentBlock)
{
	int result = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			result += abs(currentBlock.at(i).at(j) - previousBlock.at(i).at(j));
		}
	}

	return result;
}



bool reconstructImageByMotion(Image& result, Image& imagePrev, Image& imageCurr) 
{

	int w = imagePrev.getWidth();
	int h = imagePrev.getHeight();
	if (imageCurr.getWidth() != w || imageCurr.getHeight() != h) {
		return false;
	}

	result = imagePrev.clone();

	std::cout << "Hoehe: " << w << ", " << "Breite: " << h << std::endl;
	int numberOfBlocks = (int)(w / 16) * (h / 16);

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
    
	// create temp-vectors for 16x16 pixel-blocks
	std::vector<std::vector<int>> tempBlock(16, std::vector<int>(16));
	std::vector<std::vector<int>> tempSAEblock(16, std::vector<int>(16));
	
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

	unsigned char * currDataArray;
	currDataArray = (unsigned char*)malloc((int)imageCurr.getSize() * sizeof(unsigned char));
	int currDataArrayPointer = 0;
	int PixelBlockPointer = 0;
	
	for (int line = 0; line < imageCurr.getHeight(); line++)
	{
		for (int col = 0; col < imageCurr.getWidth(); col++)
		{
			currDataArray[currDataArrayPointer++] = vectorPixelBocks.at(PixelBlockPointer).at(line % 16).at(col % 16);
			if (col != 0)
			{
				if (col % 16 == 0 && PixelBlockPointer < numberOfBlocks)
				{
					PixelBlockPointer++;
					
				}
				else if (col % 352 == 0 && PixelBlockPointer < numberOfBlocks)
				{
					
					PixelBlockPointer -= 22;
					std::cout << "decremented by 22, ";
				}

			}
		}

		if (line != 0)
		{
			if (line % 16 == 0)
			{
				PixelBlockPointer += 22;
				std::cout << "incremented by 22, ";
			}
		}

	}
	
	
	


	/*
	for (int line2 = 0; line2 < imageCurr.getHeight(); line2++)
	{
		
			for (int col2 = 0; col2 < 16; col2++)
			{
				currDataArray[currDataArrayPointer++] = vectorPixelBocks.at(block).at(line2).at(col2);
			}
		
	}
	*/

	// conversion back into array from vector
	/*
	for (int line = 0; line < imageCurr.getHeight(); line++)
	{
		for (int col = 0; col < imageCurr.getWidth(); col++)
		{
			
			currDataArray[currDataArrayPointer++] = vectorDataCurrent.at(line).at(col);
			
		}
	}
	*/


	/*
	// initiate SAE-search between imageCurr and imagePrev
	int currentVectorPointer = 0;

	for (int a = 0; a < numberOfBlocks; a++)
	{
		int smallestSAEvalue = 9999999;

		for (int line = 0; line < imagePrev.getHeight(); line++)
		{
			for (int col = 0; col < imagePrev.getWidth(); col += 16)
			{
				//create temporary 16x16 block from data of previous picture
				for (int i = 0; i < 16; i++)
				{
					for (int j = 0; j < 16; j++)
					{
						tempSAEblock.at(i).at(j) = vectorDataPrevious.at(line).at(col + j);
					}
				}
				//compare temporary 16x16 block with a 16x16 block from the current picture via SAE
				int SAE = SAEfunction(tempBlock, vectorPixelBocks.at(currentVectorPointer));

				// current smallest divergent found from previous picture is placed in outgoing Vector for result picture
				if (SAE < smallestSAEvalue)
				{
					vectorPixelBocks.at(currentVectorPointer) = tempBlock;
					smallestSAEvalue = SAE;
				}
			}
		}
		currentVectorPointer++; 
	}
	
	unsigned char * currDataArray;
	currDataArray = (unsigned char*)malloc(imageCurr.getSize() * sizeof(unsigned char));
	int currDataArrayPointer = 0;
	
	for (int a = 0; a < numberOfBlocks; a++)
	{
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				currDataArray[currDataArrayPointer++] = vectorPixelBocks.at(a).at(i).at(j);
			}
		}
	}
	*/
	result.setBuffer(currDataArray);
	
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

