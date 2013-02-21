#ifndef EXECUTIVE_H
#define EXECUTIVE_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Executive.h (ver 1.0)  -  Executive Module that Starts the Analysis & Prints Summay output.	     		  // 																					   
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

  This module acts as the executive controller. It fetches the list of the files from 
  the CmdLineAgrs. It further uses the ConfigureParser module to parse the files. Once the parsing is 
  done the module uses the MetadataCreator module to generate the XML Files. The module also uses the scanner
  module to scan the files if the command line arguments indicate to do so.

  Works with CmdLineArgs, ConfigureParser & Display module for analysis.

  Public Interface:
  =================
  
  Executive();									  // Create Object
  AnalyzeFiles(std::vector<std::string>);		  // Function to analyze the files and generate Metadata.
  ScanMetadata(std::string);					  // Funciton to start scanning using a base package.
  AddDependencyToMetadata(string,vector<string>); // Function to add dependency to a existing file.

  Build Process:
  ==============
  Required files
    - WinTools.h,DataStore.h,Tokenizer.h,SemiExpression.h,
	  Parser.h,ActionsAndRules.h,ConfigureParser.h,
	  Executive.h,DataStore.h, Show.h
  
  Build commands
    
	- cl /EHsc /DTEST_EXECUTIVE Executive.cpp ConfigureParser.cpp parser.cpp WinTools.cpp ^
semiexpression.cpp tokenizer.cpp ActionsAndRules.cpp Package.cpp MetadataCreator.cpp Display.cpp ^
Scanner.cpp fileInfo.cpp XmlDocument.cpp XmlElementParts.cpp XmlDocumentFactories.cpp XmlNodes.cpp MNode.cpp MTree.cpp

  Maintenance History:
  ====================
  ver 1.0 : 25 Feb 11
  - first release
*/
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include "Package.h"

////////////////////////////////////////////////////////////////////////////////////////
// Class that will act as the controller handling all the activities of the application.
class Executive{

public :

	Executive();

	void AnalyzeFiles(std::vector<std::string>);

	void ScanMetadata(std::string);

	void AddDependencyToMetadata(std::string,std::vector<std::string>);

private : 

	Package ParsePacakge(std::string,std::pair<std::string,std::string>);

	void CreatePacakgesForAnalysis(std::vector<std::string>);

	void AddHeaderToPackage(std::string,std::string);			

	void AddSourceToPackage(std::string,std::string);	

	void ParseAndGenerateXML();

	std::string FormatToLower(std::string);

	Package Parse(std::string,std::string );

	std::string ExtractPackageName(std::pair<std::string,std::string>);

	void GenerateXML(Package); 

    std::map<std::string,std::pair<std::string,std::string>> PackageHolder;	 
};

#endif