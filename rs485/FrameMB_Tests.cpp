/*
	Author:		R.Gorohov (K1 SOFT)

	FrameMB_test.cpp
	Оgthltktybt класса для тестирования FrameMB.
*/

#include "StdAfx.h"
#include "FrameMB_Tests.h"

#include "FrameMB.h"
#include <iostream>
#include <string>

FrameMB_Tests::FrameMB_Tests(void)
{
}

FrameMB_Tests::~FrameMB_Tests(void)
{
}

void FrameMB_Tests::FrameFromBinary()
{
	BYTE binData[] = {	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
						0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

	BYTE asciiData[] = {0x3A, 0x31, 0x30, // start, address
						0x30, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33, // 0x00, 0x11, 0x22, 0x33
						0x34, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, // 0x44, 0x55, 0x66, 0x77
						0x38, 0x38, 0x39, 0x39, 0x41, 0x41, 0x42, 0x42, // 0x88, 0x99, 0xAA, 0xBB
						0x43, 0x43, 0x44, 0x44, 0x45, 0x45, 0x46, 0x46, // 0xCC, 0xDD, 0xEE, 0xFF
						0x00, 0x00, // LRC
						0x0D, 0x0A}; // \r, \n

	bool testValid = true;
	std::string testString;

	FrameMB frame(0x10, binData, sizeof(binData));

	const BYTE* asciiFrameData = frame.GetAsciiFrame();
	int len = frame.GetAsciiFrameLen();

	if( len != sizeof(asciiData) )
	{
		testValid = false;
		testString += "wrong ascii-frame len\n";
	}

	int i;
	for(i=0; i<len; ++i)
	{
		if( asciiData[i] != 0x00 )
		{
			if( asciiFrameData[i] != asciiData[i] )
			{
				testValid = false;
				testString += "wrong ascii-frame data\n";
				break;
			}
		}
	}

	if( testValid )
	{
		std::cout << std::endl << "TEST frame from binary COMPLETE" << std::endl;
	}
	else
	{
		std::cout << std::endl << "TEST frame from binary FAILED" << std::endl;
	}

	std::cout << testString;

	

	std::cout << std::endl;
	std::cout << std::dec;
	std::cout << "binary data (" << sizeof(binData) << "): ";
	std::cout << std::hex;
	for(i=0; i<sizeof(binData); ++i)
		std::cout << static_cast<int>(binData[i]) << " ";
	
	std::cout << std::endl << std::endl;
	std::cout << std::dec;
	std::cout << "ascii data (" << len << "): ";
	std::cout << std::hex;
	for(i=0; i<len; ++i)
		std::cout << static_cast<int>(asciiFrameData[i]) << " ";

	std::cout << std::endl << std::endl;
	std::cout << std::dec;
	std::cout << "control ascii data (" << sizeof(asciiData) << "): ";
	std::cout << std::hex;
	for(i=0; i<sizeof(asciiData); ++i)
		std::cout << static_cast<int>(asciiData[i]) << " ";
}

void FrameMB_Tests::FrameFromAscii()
{
}