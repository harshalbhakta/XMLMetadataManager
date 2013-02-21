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

#include "Display.h"

using namespace Win32Tools;

//////////////////////////////////////////////////////////////
// Function to print the Repository Location.
void Display::PrintRepositoryLocation()
{

	std::cout<<"\n\n Metadata Repository Location \n "
			  << std::string(55,'-');

	std::cout<<"\n  "<< Path::getFullPath(REPOSITORY_LOCATION) << "\n";

}

//////////////////////////////////////////////////////////////
// Function to print message before starting XML Processing.
void Display::GeneratingXMLMessage()
{
	std::cout<<"\n Generating XML Files \n "
			  << std::string(55,'-');
}

//////////////////////////////////////////////////////////////
// Function to print message aftre XML Processing is complete.
// Also print number of packages generated.
void Display::CompletedXMLMessage(int package_count)
{
	std::cout<<"\n XML Files Generated \n "
			  << std::string(55,'-');

	std::cout<<"\n  "<<package_count<< " files generated.\n";
}

////////////////////////////////////////////////////////////////
// Function to print package info. for packages being scanned.
void Display::PrintScanPackage(Package pk)
{
	std::cout<<"\n " << pk.getPackageName() << " - " << pk.getTitle() << "\n "
			  << std::string(55,'-');

	std::cout<<"\n";

	if ( pk.getHeader() != "NA" )
	{
		std::cout<<"  "<<pk.getHeader().substr(pk.getHeader().find_last_of("\\") + 1 ) <<"\n";
	}
	if ( pk.getSource() != "NA" )
	{
		std::cout<<"  "<<pk.getSource().substr(pk.getSource().find_last_of("\\") + 1 ) <<"\n";
	}

	std::vector<std::string> deps = pk.getDependencyList();

	for ( int i = 0 ; i < (int)deps.size() ; i++ )
	{
		std::cout<<"  "<<deps[i]<<"\n";
	}
}

////////////////////////////////////////////////////////////////
// Function to print formatted list of files received as vector.
void Display::PrintFiles(std::vector<std::string> files)
{
	for ( int i = 0 ; i < (int)files.size() ; i++ )
	{
		std::cout<<"\n  "<<files[i].substr(files[i].find_last_of("\\")+1);
	}
}

////////////////////////////////////////////////////////////////
// Function to print info. about the package being analyzed.
void Display::PrintAnalyzedPackage(Package pk)
{
	std::cout<<"\n  Pacakge Name: " << pk.getPackageName();
	std::cout<<"\n  Title: " << FormatText(pk.getTitle());
	std::cout<<"\n  Header: " << pk.getHeader().substr(pk.getHeader().find_last_of("\\") + 1 );
	std::cout<<"\n  Source: " << pk.getSource().substr(pk.getSource().find_last_of("\\") + 1 );

	std::vector<std::string> dep = pk.getDependencyList();

	for ( int i = 0 ; i < (int)dep.size() ; i++ )
	{
		std::cout<<"\n  Dependency: " << dep[i];
	}

	std::cout<<"\n";

}

////////////////////////////////////////////////////////////////
// Function to format text to ignore special charaters.
std::string Display::FormatText(std::string _desc)
{
	std::string formatted = "";

	for ( int i = 0 ; i < (int)_desc.length() ; i++)
	{
		if ( (_desc[i] >= 97 && _desc[i] <= 122) || (_desc[i] >= 65 && _desc[i] <= 90) || _desc[i] == 32 || _desc[i] == 10 )
		{
			formatted.push_back(_desc[i]);
		}
	}

	return formatted;
}

//-------------------------<TEST STUB>-------------------------//

#ifdef TEST_DISPLAY

void main ( int argc, char* argb[] )
{

	Package pk("Testing");
	pk.setTitle("Title for the pacakge");
	pk.setHeader("Testing.h");
	pk.setSource("Testing.cpp");
	pk.addDependency("Dependency1");
	pk.addDependency("Dependency2");
	pk.addDependency("Dependency3");

	Display dp;

	std::cout << "\n  Testing PrintScanPackage " << "\n "
			  << std::string(55,'=');

	dp.PrintScanPackage(pk);

	std::cout << "\n  Testing PrintAnalyzedPackage " << "\n "
			  << std::string(55,'=');

	dp.PrintAnalyzedPackage(pk);

	std::cout << "\n  Testing PrintFiles " << "\n "
			  << std::string(55,'=');

	std::vector<std::string> files;
	files.push_back("file1");
	files.push_back("files2");
	dp.PrintFiles(files);

	std::cout << "\n  Testing PrintRepositoryLocation " << "\n "
			  << std::string(55,'=');

	dp.PrintRepositoryLocation();

	std::cout << "\n  Testing GeneratingXMLMessage & CompletedXMLMessage " << "\n "
			  << std::string(55,'=');
	dp.GeneratingXMLMessage();				 // Print message for Generating XML.
    dp.CompletedXMLMessage(5);              // Print message for All Completed XML.


}

#endif