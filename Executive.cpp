////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Executive.cpp (ver 1.0)  -  Executive Module that Starts the Analysis & Prints Summay output.	     		  // 																					   
//		 																									  //
//	Language		:   Visual C++, 2010																	  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Executive.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "Executive.h"
#include "MetadataCreator.h"
#include "WinTools.h"
#include "Scanner.h"

using namespace Win32Tools;

/////////////////////////////////////////////////////////////////////////////////////
//  Constructor
Executive::Executive() { }

/////////////////////////////////////////////////////////////////////////////////////
//  Function to perform Parse the files and Generate XML Files.
void Executive::ParseAndGenerateXML()
{
	Display dp;
	dp.PrintRepositoryLocation();

	try 
	{
		MetadataCreator mc;
	}
	catch ( std::exception ex )
	{ 
		throw "\n\n Error Generating Creating Repository Folder. Check Access rights. \n\n";

		return;
	}

	dp.GeneratingXMLMessage();

	std::cout<<"\n";

	int package_count = PackageHolder.size();

	for each ( std::pair<std::string,std::pair<std::string,std::string>> package_elem in PackageHolder )
	{
		try
		{
			std::cout << "\n  Analyzing and Generating XML for : " << package_elem.first << "\n "
							<< std::string(55,'=');

			Package pk = ParsePacakge(package_elem.first,package_elem.second);

			GenerateXML(pk); 

			Display dp;

			dp.PrintAnalyzedPackage(pk);
		}
		catch ( char* msg )
		{ std::cout<<msg; }
		catch ( std::exception ex )
		{ std::cout<<"\n\n Error Generating XML : "<<package_elem.first<<ex.what()<<"\n\n"; package_count--;}
	}

	dp.CompletedXMLMessage(package_count);
}

/////////////////////////////////////////////////////////////////////////////////////
//  Function to generate the XML Files.
void Executive::GenerateXML(Package pk)
{
	MetadataCreator mc;

	mc.GenerateMetadataFile(pk);
}

/////////////////////////////////////////////////////////////////////////////////////
//  Funciton to parse the files per package.
Package Executive::ParsePacakge(std::string package_name,std::pair<std::string,std::string> package)
{
	
    try
    {

		Package header_pacakge = Parse(package_name,package.first);
	
	    Package source_pacakge = Parse(package_name,package.second);

		Package combo_pacakge(package_name);

		combo_pacakge.setTitle(header_pacakge.getTitle());
		combo_pacakge.setTitle(source_pacakge.getTitle());

		combo_pacakge.setDescription(header_pacakge.getDescription());
		combo_pacakge.setDescription(source_pacakge.getDescription());

		combo_pacakge.setHeader(package.first);
		combo_pacakge.setSource(package.second);

		std::vector<std::string> header_dependency = header_pacakge.getDependencyList();
		std::vector<std::string> source_dependency = source_pacakge.getDependencyList();

		for ( int i = 0 ; i < (int)header_dependency.size() ; i++)
		{ 
			combo_pacakge.addDependency(header_dependency[i]); 
		}
		for ( int j = 0 ; j < (int)source_dependency.size() ; j++)
		{ 
			combo_pacakge.addDependency(source_dependency[j]); 
		}

		return combo_pacakge;
	}
    catch(std::exception& ex)
    {
	  Package pk("Error");
      { std::cout<<"\n\n Error Processing Package : "<<package_name<<ex.what()<<"\n\n"; }
	  return pk;
    }

}

/////////////////////////////////////////////////////////////////////////////////////
//  Function that uses ConfigurParser module to actually parse the files.
Package Executive::Parse(std::string package_name,std::string file)
{
	   ConfigParseMetadata configHeader;
       Parser* pHeaderParser = configHeader.Build();

       if(pHeaderParser)
       {
			if( file != "NA" && !configHeader.Attach(file))
			{
				Package pk("Error");
				std::cout << "\n  could not open file " << file << std::endl;
				return pk;
			}
			else if ( file != "NA" )
			{
			// now that parser is built, use it
			 while(pHeaderParser->next())
			   pHeaderParser->parse();

			 return configHeader.GetPacakge();
			}
			else 
			{
				Package pk(package_name);
				return pk;
			}
	   }
       else
       {
        Package pk("Error");
        std::cout << "\n\n  Parser not built\n\n";
		return pk;
       }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//  Function to extract package name from the filename. ( Parser -> parser.h / parser.cpp )
std::string Executive::ExtractPackageName(std::pair<std::string,std::string> package)
{
	std::string currentPackage = "";

	if ( package.first != "NA" )
	{
		currentPackage = package.first.substr(package.first.find_last_of("\\") + 1);

		currentPackage = FormatToLower(currentPackage);

		if ( currentPackage.substr(currentPackage.length() - 2 ) == ".h" ) 
		{ currentPackage = currentPackage.substr(0,currentPackage.length()-2); }
	}
	else
	{
		currentPackage = package.second.substr(package.second.find_last_of("\\") + 1);

		currentPackage = FormatToLower(currentPackage);

		 if ( currentPackage.substr(currentPackage.length() - 4 ) == ".cpp" ) 
		 { currentPackage = currentPackage.substr(0,currentPackage.length()-4); }
	}

	currentPackage = FormatToLower(currentPackage);

	return currentPackage;
}

///////////////////////////////////////////////////////////////
// Function to convert the text to lower case.
  std::string Executive::FormatToLower(std::string text)
  {
		std::string formatted = "";

		for ( int i = 0 ; i < (int)text.length() ; i++)
		{
			formatted.push_back(std::tolower(text[i]));
		}

		return formatted;
  }

/////////////////////////////////////////////////////////////////////////////////////
//  Function to extract package Name and add initial package to Repository.
void Executive::CreatePacakgesForAnalysis(std::vector<std::string> files)
{
	for each ( std::string file in files)
	{
		try 
		{
			std::string currentPackage = file.substr(file.find_last_of("\\") + 1);

			currentPackage = FormatToLower(currentPackage);

			if ( currentPackage.substr(currentPackage.length() - 2 ) == ".h" ) 
			{ 
				currentPackage = currentPackage.substr(0,currentPackage.length()-2); 
			
				AddHeaderToPackage(currentPackage,file);			
			}
			else if ( currentPackage.substr(currentPackage.length() - 4 ) == ".cpp"  ) 
			{ 
				currentPackage = currentPackage.substr(0,currentPackage.length()-4); 

				AddSourceToPackage(currentPackage,file);
			}
		}
		catch ( std::exception ex)
		{ std::cout<<"Error Processing File : "<<file<<":"<<ex.what(); }
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//  Function to add the header to the map -> package.
void Executive::AddHeaderToPackage(std::string package,std::string header)
{
	if ( PackageHolder.find(package) == PackageHolder.end() )
	{
		std::pair<std::string,std::string> header_source;
		header_source.first = header;
		header_source.second = "NA";

		PackageHolder[package] = header_source;
	}
	else
	{
		PackageHolder[package].first = header;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//  Function to add the source to the map -> package.
void Executive::AddSourceToPackage(std::string package,std::string source)
{
	if ( PackageHolder.find(package) == PackageHolder.end() )
	{
		std::pair<std::string,std::string> header_source;
		header_source.first = "NA";
		header_source.second = source;

		PackageHolder[package] = header_source;
	}
	else
	{
		PackageHolder[package].second = source;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//  Controller function to perform parsing and generation of XML Files.
void Executive::AnalyzeFiles(std::vector<std::string> files)
{
	CreatePacakgesForAnalysis(files);

	ParseAndGenerateXML();
}

/////////////////////////////////////////////////////////////////////////////////////
//  Function to start scanning using a base package as start point.
void Executive::ScanMetadata(std::string _package_name)
{
	Scanner sc;

	sc.ScanPackageRecurse(_package_name);
}

/////////////////////////////////////////////////////////////////////////////////////
//  Funciton to support modification of XML by using MetadataCreator class.
void Executive::AddDependencyToMetadata(std::string _package_name,std::vector<std::string> dependencies)
{
	MetadataCreator mc;

	std::cout << "\n  Display Packge File Before Adding Dependencies \n "
            << std::string(21,'=') << std::endl;

	mc.ShowXMLFile(_package_name);

	for each ( std::string dependency in dependencies)
	{
		try 
		{
			mc.AddDependency(_package_name,dependency);
		}
		catch ( char* msg )
		{ std::cout<< "\n\n Error Adding Dependency -> " << dependency << "\n Error : "<<msg;}
		catch ( std::exception& ex )
		{ std::cout<< "\n\n Error Adding Dependency -> " << dependency << "\n Error : "<<ex.what();}
	}

	std::cout << "\n  Display Packge File After Adding Dependencies \n "
            << std::string(21,'=') << std::endl;

	mc.ShowXMLFile(_package_name);
}

/////////////////////////////////////////////////////////////////////////////////////
//  Test Stub

#ifdef TEST_EXECUTIVE

void main ( int argc , char *argv[] )
{

	std::cout << "\n  Testing Creation of XML " << "\n "
			  << std::string(55,'=');

	std::vector<std::string> files;

	files.push_back(Path::getFullPath("..\\Parser.h"));
	files.push_back(Path::getFullPath("..\\Parser.cpp"));

	std::vector<std::string> ptrFiles = files;

	Executive ex; 

	ex.AnalyzeFiles(files);

	std::cout << "\n  Testing Scanning " << "\n "
			  << std::string(55,'=');

	ex.ScanMetadata("Parser");

	std::cout << "\n  Testing Modification by adding TestPackge " << "\n "
			  << std::string(55,'=');

	std::vector<std::string> deps;
	deps.push_back("TestPackge");

	ex.AddDependencyToMetadata("Parser",deps);

	std::getchar();
}

#endif