#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CmdLineArgs.h (ver 2.0)  -  Store and Validate Command Line Arguments.									  // 																					   
//		 					  -  Added 2 options for Modifying XML & Scanning Metadata.						  //
//	Language		:   Microsoft Visual C++ 2010															  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CmdLineArgs.h (ver 1.0)  -  Store and Validate Command Line Arguments.									  // 																					   
//		 																									  //
//	Language		:   Microsoft Visual C++ 2010															  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================

  This module defines a CmdLineArgs class. Its instance will be used to handle
  command line arguments. 

  Usage 1 : Recursively Parse files to create Metadata.

  Exactly Once : argv[1] : Recurse Option : -r Or -nr
  Exactly Once : argv[2] : File Pattern   : *.* ( Selection criteria to choose file )
  Atleast Once : argv[3] : Path Spces	   : ..\Parser ( Folder Location )
				 argv[.] : .............. : ( Folder Location )
				 argv[.] : .............. : ( Folder Location )
				 argv[.] : .............. : ( Folder Location )
				 argv[N] : .............. : ( Folder Location )

				 argv[3] To argv[N] = Folder Path Specifications.

  Usage 2 : Scan File
  
  Exactly Once : argv[1] : Scan Mode Option : -s
  Exactly Once : argv[2] : Package Name     :  Top Level Pacakge name for scanning metadata.
  
  Usage 3 : Modify Pacakge Metadata

  Exactly Once : argv[1] : Modify Option  : -m
  Exactly Once : argv[2] : Package Name   : Package name to be modified.
  Atleast Once : argv[3] : Dependency	  : Package name to be added.
				 argv[.] : .............. : Package name to be added.
				 argv[.] : .............. : Package name to be added.
				 argv[.] : .............. : Package name to be added.
				 argv[N] : .............. : Package name to be added.

				 argv[3] To argv[N] = Folder Path Specifications.

  If the criteria above does not match, we return error message.

  Public Interface:
  =================
  
  CmdLineArgs cla;							 // Create Class Object
  if ( cla.ValidateAndStoreArgs(argc,argv) ) // Validate and Analyze Arguments.
  cla.getFiles();							 // Get vector of files to analyze.
  cla.getCurrentMode();						 // Get current mode.
  cla.printManual();						 // Print manual describing arguments.
  cla.isRecurse();							 // Get Recurse Status.
  cla.getFilePattern();						 // Get File Patterns entered.
  cla.getPathSpecs();						 // Get Path for entered for analysis.
  cla.getCurrentMode();						 // Get Current Mode of analysis. ( Usage )

  Build Process:
  ==============
  Required files
    
	- CmdLineArgs.h, CmdLineArgs.cpp
	- CustomNavigate.h, CustomNavigate.cpp 
	- WinTools.h,wintools.cpp, fileInfo.h, fileInfo.cpp
	- StringConversions.h,StringConversions.cpp
  
  Build commands ( Can be used directly copying from here )
    
	- cl /EHsc /DTEST_CMDLINEARGS CmdLineArgs.cpp ^
CustomNavigate.cpp WinTools.cpp fileInfo.cpp

  Maintenance History:
  ====================
  ver 2.0 : 19 Mar 11
  - Added 2 options for Modifying XML & Scanning Metadata.
  ver 1.0 : 25 Feb 11
  - First Release.
*/
#include <string>
#include <iostream>
#include <vector>
#include "CustomNavigate.h"

using namespace Win32Tools;

////////////////////////////////////////////////////////////////////////////
//  CmdLineArgs - Class to handle the operations for command line arguments.
class CmdLineArgs{

public :

	CmdLineArgs();

	bool ValidateAndStoreArgs(int argc,char* argv[]);

	void printValidFiles();

	void printManual();

	std::vector<std::string> getFiles();

	bool isRecurse();

	std::vector<std::string> getFilePattern();

	std::string getPathSpecs();

	enum mode { PARSE_MODE , MODIFY_MODE , SCAN_MODE } ;

	mode getCurrentMode();

private : 

	mode _currentMode;
	bool recurse;
	std::string pathSpec;
	std::vector<std::string> filePattern;
	std::vector<std::string> files;

	void fetchFiles();

};

#endif