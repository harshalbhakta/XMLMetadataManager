#ifndef PACKAGE_H
#define PACKAGE_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Package.h (ver 1.0)  -  Class to hold information about each package ( .h + .cpp ) file.					  // 																					   
//		 																									  //
//	Product 		:   Microsoft Visual C++ 2010															  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #2 - XMLMetadataManager, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================

  This module defines a Package class that will hold the information related to each package.

  Information to be held : 
  1. Package Name.
  2. Header File.
  3. Source File.
  4. List of Dependencies.

  Above mentioned data will be held for a package, which involves ( .h + .cpp )

  Object of Package class will be used to basically gather and print the information for a package.

  Public Interface:
  =================
  
	Package pk("Parser");					   // Create object with Package Name as Constructor Argument.
	pk.setSource("Parser.cpp");				   // Set Source File. 
	pk.setHeader("Parser.h");				   // Set Header File.
	pk.setTitle("Module to parse files.");	   // Set Title for the package. (Overwrite only if "NA")
	pk.setDescription("Module Description.");  // Set Description for the package. (Overwrite only if "NA")
	pk.setTitleForce("Module to parse file."); // Set Title for the package. (Overwrite for all cases)
	pk.setDescriptionForce("Module Desc.");    // Set Description for the package.  (Overwrite for all cases)
	pk.addDependency("Toker");				   // Add Depdendency.
	pk.addDependency("Semi");				   // Add another dependency.
	pk.getPackageName();					   // Get Pacakge Name.
	pk.getHeader();                            // Get Header Name.
	pk.getSource();							   // Get Source Name.
	pk.getTitle();							   // Get Title Text.
	pk.getDescription();					   // Get vector of dependency files.

  Build Process:
  ==============
  
  Required files
    - Package.h, Package.cpp
  
  Build commands
    - cl /EHsc /DTEST_PACKAGE Package.cpp

  Maintenance History:
  ====================
  ver 1.0 : 25 Mar 11
  
*/

#include <string>
#include <vector>

///////////////////////////////////////////////////////////////
// Class to hold information related to Pacakge.
// A list will be maintined.
class Package
{

public :

	void setTitleForce(std::string);
	void setDescriptionForce(std::string);

	std::string getPackageName();
	void setPackageName(std::string);

	std::string getHeader();
	void setHeader(std::string);
	
	std::string getSource();
	void setSource(std::string);

	std::string getTitle();
	void setTitle(std::string);

	std::string getDescription();
	void setDescription(std::string);

	void addDependency(std::string);
	std::vector<std::string> getDependencyList();

	Package(std::string);

private : 

std::string package_name;
std::string header;
std::string source;
std::string title;
std::string description;

std::vector<std::string> dependency_list;

};

#endif