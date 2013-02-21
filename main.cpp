////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp - Source File to test XML Metadata Manage - Provider of function main()							  // 																					   
//	(ver 1.0)																								  //
//	Language		:   Microsoft Visual C++, 2010														      //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #2 - XMLMetadataManager, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* Module Operations:
* ==================
* - main() : XMLMetadataManager Application start point.

* Build Process:
* ==============
* devenv CodeMetricsAnalyzer.sln /rebuild debug
* 
* Maintenance History:
* ====================
* ver 1.0 : 15 Feb 2011
* - Initial Release
*/

#include "CmdLineArgs.h"
#include "Executive.h"
#include "Display.h"


void printHeader()
{
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl<<"	  XML Metadata Generator : XMA , v1.0	";
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl<<"C++ Metadata Generator		          Harshal Bhakta	";
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl;
}


void PerformScanMode(std::string _base_package)
{
	Executive ex;

	std::cout << "\n  Scanning Base Pacakge : " << _base_package << "\n "
							<< std::string(55,'=');

						ex.ScanMetadata(_base_package);
}


void PerformModifyMode(int argc,char* argv[])
{
	Executive ex;
	std::vector<std::string> dependencies;

	std::cout << "\n  Modification : Adding Dependencies to Package " << argv[2] << "\n"
							<< std::string(55,'=');

	std::cout << "\n  Dependencies to be added : " << "\n"
		<< std::string(55,'=');

	for ( int i = 3 ; i < argc ; i++ )
		{
			std::cout << "\nDependency Package : " << argv[i] ;
			dependencies.push_back(argv[i]);
		}

	std::cout<< "\n" ;

	ex.AddDependencyToMetadata(argv[2],dependencies);	

}

void PerformParseMode(std::vector<std::string> files)
{
	Executive ex;

	std::cout << "\n Listing Files Fetched for Generating Metatade \n "
							<< std::string(55,'=');

	Display dp;

	dp.PrintFiles(files);

	ex.AnalyzeFiles(files);

}


int main(int argc,char* argv[])
{

	CmdLineArgs cla;

	try 
	{
			printHeader();

			if ( cla.ValidateAndStoreArgs(argc,argv) ) 
			{
				
				std::vector<std::string> files;

				switch(cla.getCurrentMode())
				{
					case cla.SCAN_MODE:	PerformScanMode(argv[2]);

					break ;

					case cla.MODIFY_MODE: PerformModifyMode(argc,argv);

					break ;

					case cla.PARSE_MODE: 
						
						files = cla.getFiles(); 
						PerformParseMode(files);
					
					 break ;
				 } 	 std::cout<<" \n" ;
			}
			else 
			{
				std::cout<<std::endl<<" Invalid Arguments. \n\n" ;
				cla.printManual();
			}
	}
	catch ( char* msg )
	{ std::cout<< msg << std::endl; }
	catch ( std::exception& ex)
	{ std::cout<<"Application ended in error : "<<ex.what(); }
	catch (...)
	{ std::cout<<"Application ended in fatal error. "; }
}
