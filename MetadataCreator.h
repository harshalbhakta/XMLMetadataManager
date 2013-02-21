#ifndef METADATACREATOR_H
#define METADATACREATOR_H
/////////////////////////////////////////////////////////////////////
//  MetadataCreator.h - Generate Metadata XM: Files.               //
//  ver 1.2                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  
  This module will be responsible for performing the task related to 
  generation of XML Files. The module will basically operate on 
  Pacakge objects and based on the data store in the object will 
  hold various funcitons responsible for generating parts of the 
  XML file. All the task related to the XML Metadata, ex. Creation 
  and Modification will be supported by this class.

  By default, This module overwrites existing files.

  Public functions provided for Package name, Title, Desc, Header and 
  Source to allow user to overwrite specific information of existing files.

  Public Interface:
  =================
  
	 MetadataCreator();                         // Construct Object.
     GenerateMetadataFile(Package);			    // Generate XML File using Pacakge Object.
	 CreateXMLFile(std::string);			    // Function to generate XML file with empty skelaton.
	 AddPackageName(std::string);				// Function to add/edit pacakge name.
	 AddTitle(std::string,std::string);			// Function to add/edit title text.
	 AddDescription(std::string,std::string);	// Function to add/edit desc. text.
	 AddHeader(std::string,std::string);		// Function to add/edit header name.
	 AddSource(std::string,std::string);		// Function to add/edit source name.
	 AddDependency(std::string,std::string);	// Function to add dependency name.
	 ShowXMLFile(std::string);					// Function to display generated XML.			
	 
  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp,
      tokenizer.h, tokenizer.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      ConfigureParser.h, ConfigureParser.cpp

  Build commands 
    
    - cl /EHsc /DTEST_METADATA_CREATOR MetadataCreator.cpp Package.cpp  ^
WinTools.cpp fileInfo.cpp XmlDocument.cpp XmlElementParts.cpp ^
XmlDocumentFactories.cpp XmlNodes.cpp MNode.cpp MTree.cpp Tokenizer.cpp

  Maintenance History:
  ====================
  ver 1.0 : 19 Mar 11
  - first release

*/

#include <iostream>
#include <string>
#include "Package.h"

/////////////////////////////////////////////////////////////////////
//  Reposiotry Folder Location.
#define REPOSITORY_LOCATION "MetadataRepository\\"

///////////////////////////////////////////////////////////////////////////
//  Class responsible for creation and modification of XML Metadata files.
class MetadataCreator {

public : 

	void GenerateMetadataFile(Package);

	void CreateXMLFile(std::string);

	void AddPackageName(std::string);

	void AddTitle(std::string,std::string);

	void AddDescription(std::string,std::string);

	void AddHeader(std::string,std::string);

	void AddSource(std::string,std::string);

	void AddDependency(std::string,std::string);

	void ShowXMLFile(std::string);

	MetadataCreator();

private : 

	bool isFileExist(std::string);	

	std::string FormatText(std::string);

	void CheckRepositoryFolder();

	std::string FormatToLower(std::string);
	
};

#endif