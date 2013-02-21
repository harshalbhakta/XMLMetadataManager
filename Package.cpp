////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Package.cpp (ver 1.0)  -  Class to hold information about each package ( .h + .cpp ) file.				  // 																					   
//		 																									  //
//	Language		:   Visual C++, 2010																	  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Package.h"
#include <algorithm>
#include <iostream>

///////////////////////////////////////////////////////////////
// Constructor to Initailize values. ( Initail Value : "NA" )
Package::Package(std::string _package_name) : 
package_name(_package_name),header("NA"),source("NA"),title("NA"),description("NA")
{ }

///////////////////////////////////////////////////////////////
// Function to add dependency to the list.
// Add Dependecy only if it is a new entry.
void Package::addDependency(std::string _package)
{
	if ( std::find(dependency_list.begin(),dependency_list.end(),_package) == dependency_list.end() )
	{
		dependency_list.push_back(_package);
	}
}

///////////////////////////////////////////////////////////////
// Function to get Package name.
 std::string Package::getPackageName() 
 { return package_name; };
 
///////////////////////////////////////////////////////////////
// Function to set Package name.
 void Package::setPackageName(std::string _package_name) 
 { package_name = _package_name; };

///////////////////////////////////////////////////////////////
// Function to get Header File.
 std::string Package::getHeader() 
 { return header; };
 
///////////////////////////////////////////////////////////////
// Function to set Header File.
 void Package::setHeader(std::string _header) 
 { header = _header; };
	
///////////////////////////////////////////////////////////////
// Function to get Source File.
 std::string Package::getSource() 
 { return source; } ;
 
///////////////////////////////////////////////////////////////
// Function to set Source File.
 void Package::setSource(std::string _source) 
 { source = _source; };

///////////////////////////////////////////////////////////////
// Function to get Title Text.
 std::string Package::getTitle() 
 { return title; };

 ///////////////////////////////////////////////////////////////
// Function to set Title Text. (Set value for all cases.)
 void Package::setTitleForce(std::string _title)
 {
	 title = _title; 
 }

///////////////////////////////////////////////////////////////
// Function to set Title Text. (Set only if value is initial.)
// Value once set will not be over-written by this function.
 void Package::setTitle(std::string _title) 
 { 
	 if ( title == "NA" && _title != "NA" )
	   title = _title; 
 };

///////////////////////////////////////////////////////////////
// Function to get Desc. Text. 
 std::string Package::getDescription() 
 { return description; };
 
 ///////////////////////////////////////////////////////////////
// Function to set Desc. Text. ( Set in all cases )
 void Package::setDescriptionForce(std::string _description)
 {
	description = _description; 
 }

///////////////////////////////////////////////////////////////
// Function to set Desc. Text. ( Set only if value is initial. )
// Value once set will not be over-written by this function.
 void Package::setDescription(std::string _description) 
 { 
	 if ( description == "NA" && _description != "NA" )
	   description = _description; 
 };

///////////////////////////////////////////////////////////////
// Function to get Dependency list.
 std::vector<std::string> Package::getDependencyList() 
 { return dependency_list; }

//
//----< test stub >--------------------------------------------

#ifdef TEST_PACKAGE

#include <string>

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing Pacakge Module " << "\n "
			  << std::string(55,'=');

	std::cout << "\n  Creating object of \"Package\" " << "\n ";

	Package pk("Parser");
	pk.setSource("Parser.cpp");
	pk.setHeader("Parser.h");
	pk.setTitle("Module to parse files.");
	pk.setDescription("Module Description.");
	pk.addDependency("Toker");
	pk.addDependency("Semi");

	std::cout << "\n  Object of \"Package\" filled using Set Methods. " << "\n ";

	std::cout << "\n  Testing display of \"Package\" using Get Methods. " << "\n \n ";

	std::cout<<" Package Name = "<<pk.getPackageName() << "\n" ;
	std::cout<<"  Header Name = "<<pk.getHeader() << "\n" ;
	std::cout<<"  Source Name = "<<pk.getSource() << "\n" ;
	std::cout<<"  Title = "<<pk.getTitle() << "\n" ;
	std::cout<<"  Description = "<<pk.getDescription() << "\n" ;
	std::cout<<"  Dependencies : " ;

	std::vector<std::string> dep = pk.getDependencyList();

	for ( int i = 0 ; i < (int)dep.size() ; i++)
	{  std::cout<<"  "<<dep[i] << ",  " ; }

	std::cout<<" \n \n ";
}

#endif