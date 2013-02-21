#ifndef CUSTOMNAVIGATE_H
#define CUSTOMNAVIGATE_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CustomNavigate.h (ver 1.0)  -  Module to parse the directory and fetch files based on cmd arguments		  // 																					   
//		 																									  //
//	Language		:   Visual C++, 2010																	  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================

  This module takes the Directory Path,File Pattern and recurse flag as input. Based on the input arguments 
  provided, this module traveses through the Directory Structure recursively or non - recursively. Files are 
  picked up from the directories while traversing based on the file patter taken as input.

  1) Directory Path : Path of the directory to start fetching files from.
  2) Recurse : Option to allow recursvie / non-recuresive traversal.
  3) Multiple File Patterns : Option to fetch files using multiple file patterns.

  Public Interface:
  =================
  
  CustomNavigate cn;						  // Create Object
  cn.getFiles(DirsPath,FilePattern,Recurse);  // Function to return only valid files based path, pattern & recurse.
  cn.printFileInfo();						  // Print the Invalid Directory & Files Ignored (Repeated Filename).

  Build Process:
  ==============
  
  Required files

    - CustomNavigate.h, CustomNavigate.cpp
    - WinTools.h,wintools.cpp, fileInfo.h, fileInfo.cpp
	- StringConversions.h,StringConversions.cpp
  
  Build commands ( Can be used directly copying from here )

    - cl /EHsc /DTEST_CUSTOM_NAVIGATE CustomNavigate.cpp ^
WinTools.cpp fileInfo.cpp StringConversion.cpp
	 
  Maintenance History:
  ====================
  ver 1.0 : 25 Mar 11
*/

#include <string>
#include <windows.h>
#include <vector>
#include "wintools.h"

namespace Win32Tools
{

///////////////////////////////////////////////////////////////
// Class to navigate and traverse to get files.
  class CustomNavigate {

  public : 

	  std::vector<std::string> getFiles(std::string,std::vector<std::string>,bool);

	  void printFileInfo();

  private : 

	  bool validateDirectory(std::string);

	  void Walk(std::string,std::vector<std::string>,bool);

	  void addFile(std::string,std::string);

	  std::vector<std::string> files;

	  std::vector<std::string> invalidDirectoryPath;

	  std::vector<std::string> ignoredFiles;

  };

}

#endif