#ifndef DISPLAY_H
#define DISPLAY_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display.h (ver 1.0)  -  Module to handle formatted output for XMLMetadata Manager.						  // 																					   
//		 																									  //
//	Language		:   Visual C++, 2010																	  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #2 - XMLMetadataManager, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================

  This module will be responsible for handling the output of the XML MetadataManager. Multiple modules 
  will call the methods to print the data in a formatted manner. The external modules will be passing 
  the Package data or the list of files based on the function requirement and the Display module will
  format the output and display on screen. No processing carried out for the data in this module.

  Public Interface:
  =================
  
  Display dp;						     // Create Object
  PrintScanPackage(Package);             // Print formatted data for Scanned Package object.
  PrintFiles(std::vector<std::string>);  // Print formatted list of files.
  PrintRepositoryLocation();	         // Print formatted repository location.
  GeneratingXMLMessage();				 // Print message for Generating XML.
  CompletedXMLMessage(int);              // Print message for All Completed XML.
  PrintAnalyzedPackage(Package);         // Print formatted data for Analyzed Package object.

  Build Process:
  ==============
  
  Required files

    - Display.h, Display.cpp, Package.h, Package.cpp
    - WinTools.h,wintools.cpp, fileInfo.h, fileInfo.cpp
	- StringConversions.h,StringConversions.cpp
  
  Build commands ( Can be used directly copying from here )

    - cl /EHsc /DTEST_DISPLAY Display.cpp WinTools.cpp ^
Package.cpp fileInfo.cpp
	 
  Maintenance History:
  ====================
  ver 1.0 : 25 Mar 11
*/
#include <iostream>
#include "Package.h"
#include "WinTools.H"

////////////////////////////////////////////////////////////////////
// Repository Location for Scanning Files.
// Program exe will look for a folder named MetadataRepository
// within the current folder and find a xml in that folder
#define REPOSITORY_LOCATION "MetadataRepository\\"

////////////////////////////////////////////////////////////////////
// Display class responsible for handling Xml Metadata Output.
class Display{

public  :

	void PrintScanPackage(Package);

	void PrintFiles(std::vector<std::string>);

	void PrintRepositoryLocation();

	void GeneratingXMLMessage();

	void CompletedXMLMessage(int);

	void PrintAnalyzedPackage(Package);

private :

	std::string FormatText(std::string);

};

#endif