////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CmdLineArgs.cpp (ver 2.0)  -  Store and Validate Command Line Arguments.									  // 																					   
//		 					  -  Added 2 options for Modifying XML & Scanning Metadata.						  //
//	Language		:   Microsoft Visual C++ 2010															  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CmdLineArgs.cpp (ver 1.0)  -  Store and Validate Command Line Arguments.									  // 																					   
//		 																									  //
//	Language		:   Microsoft Visual C++ 2010															  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CmdLineArgs.h"

///////////////////////////////////////////////////////////////
//  CmdLineArgs - Constructor
CmdLineArgs::CmdLineArgs()
	: recurse(false),pathSpec("."),filePattern(1,"*.cpp"),_currentMode(PARSE_MODE)
{}

///////////////////////////////////////////////////////////////
//  Validate the Command line arguments.
bool CmdLineArgs::ValidateAndStoreArgs(int argc,char* argv[])
{
	try 
	{
		if ( argc == 3 && (std::string)argv[1] == "-s" )
		{ _currentMode = SCAN_MODE; return true; } // Scan

		if ( argc > 3 && (std::string)argv[1] == "-m" )
		{ _currentMode = MODIFY_MODE; return true; } // Modify

		if ( argc < 4 ) { return false; }

		std::string recurseOption = argv[1];

		if ( recurseOption == "-r" ) // Fetch Files
			{ recurse = true; }
		else if ( recurseOption == "-nr") 
			{ recurse = false;}
		else 
			{ return false; }

		pathSpec = argv[2];

		filePattern.clear();

		for(int i = 3; i < argc;i++)
		{ filePattern.push_back(argv[i]); }

		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl<<"	 GATHERING FILES FOR ANALYSIS				";
		std::cout<<std::endl<<"--------------------------------------------------------";

		fetchFiles();

		return true;
	}
	catch (std::exception ex)
	{
		printManual();

		throw " Error Processing Arguments " ;
		
		return false;
	}
}

///////////////////////////////////////////////////////////////
//  Return recurse flag.
bool CmdLineArgs::isRecurse()
{ return recurse; }

///////////////////////////////////////////////////////////////
//  Return File Pattern.
std::vector<std::string> CmdLineArgs::getFilePattern()
{ return filePattern; }

///////////////////////////////////////////////////////////////
//  Return Path Specs.
std::string CmdLineArgs::getPathSpecs()
{ return pathSpec;}

///////////////////////////////////////////////////////////////
//  Return Vector of String.
std::vector<std::string> CmdLineArgs::getFiles()
{
	return files;
}

///////////////////////////////////////////////////////////////
//  Fetch Files based on the arguments entered.
void CmdLineArgs::fetchFiles()
{
		CustomNavigate cn;

		files = cn.getFiles(pathSpec,filePattern,recurse);
}

///////////////////////////////////////////////////////////////
//  Print the valid files fetched.
void CmdLineArgs::printValidFiles()
{
	for each ( std::string file in files )
	{ std::cout<<std::endl<<" "<<file; }
}

///////////////////////////////////////////////////////////////
//  Print the manual when invalid arguments are entered.
void CmdLineArgs::printManual()
{
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl<<"--- Usage 1 : xmm -r|-nr DirPath pat1 pat2 patN      ---";
		std::cout<<std::endl<<"---      -r : Recursively fetch files.		     ---";
		std::cout<<std::endl<<"---     -nr : Non Recursively fetch files.           ---";
		std::cout<<std::endl<<"--- DirPath : Base Dir. Relative or Absolute Path    ---";
		std::cout<<std::endl<<"---    pat1 : File Filter Pattern 1. (Eg. *.cpp *.h) ---";
		std::cout<<std::endl<<"---    pat2 : File Filter Pattern 2. (Eg. *.cpp *.h) ---";
		std::cout<<std::endl<<"---	    : (Atleast One File Pattern Required.)   ---";
		std::cout<<std::endl<<"---    patN : File Filter Pattern N. (Eg. *.cpp *.h) ---";
		std::cout<<std::endl<<"--- Usage 2 : xmm -s Package			     ---";
		std::cout<<std::endl<<"---      -s : scan files.		             ---";
		std::cout<<std::endl<<"--- Package : Top level package name.                ---";
		std::cout<<std::endl<<"--- Usage 3 : xmm -m Package Dep1 Dep2 ... DepN      ---";
		std::cout<<std::endl<<"--- Package : Package name to add dependency.        ---";
		std::cout<<std::endl<<"---    Dep1 : Dependency Package Name		     ---";
		std::cout<<std::endl<<"---    Dep2 : Dependency Package Name		     ---";
		std::cout<<std::endl<<"---	    : (Atleast one Dep. Package Required.)  ---";
		std::cout<<std::endl<<"---    DepN : Dependency Package Name		    ---";
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl<<"--------------------------------------------------------";
		std::cout<<std::endl;
}

///////////////////////////////////////////////////////////////
//  Get current mode.
CmdLineArgs::mode CmdLineArgs::getCurrentMode()
{
	return _currentMode;
}

/////////////////////////////////////////////////////////////////////
//  Test Stub.
//  Usage 1 Command Line Argument : -r .. *.cpp *.h  ( Current )
//  Usage 2 Command Line Argument : -s PackageName
//  Usage 3 Command Line Argument : -m PcakgeName Dep1 Dep2 .. DepN
#ifdef TEST_CMDLINEARGS
int main(int argc, char* argv[])
{
	std::cout << "\n  Testing CmdLineArgs Module\n "
            << std::string(55,'=');

	std::cout << "\n Create object and Test argc - argv input" << "\n "
		      << std::string(55,'-');

	CmdLineArgs cla;

	std::cout << "\n Testing is arguments are valid " << "\n ";
	if ( cla.ValidateAndStoreArgs(argc,argv) ) 
	{
		std::cout << "\n Arguments are valid " << "\n ";

		std::cout << "\n Testing Current Mode " << "\n ";
		switch(cla.getCurrentMode())
		{
			case cla.SCAN_MODE:
				std::cout << "\n  Argument Status : Scan \n "
					<< std::string(22,'=');

				std::cout << "\n Package To Scan : " << argv[2] <<"\n";

			break ;

			case cla.MODIFY_MODE:

				std::cout << "\n  Argument Status : Modfiy \n "
					<< std::string(22,'=');

				std::cout << "\n Add Dependency to Package : " << argv[2] << "\n" ;
				
				for ( int i = 3 ; i < argc ; i++ )
					{
					std::cout << "		  Dependency Package : " << argv[i] << "\n" ;		
					}

			break ;

			case cla.PARSE_MODE:

					std::cout << "\n  Argument Status : PARSE \n "
					<< std::string(22,'=');

					std::cout<<std::endl<<" Arguments Entered are valid." ;

					std::cout<<std::endl<<" Rescurse Option = "<<( cla.isRecurse() == 0 ? "False" : "True ");

					std::cout<<std::endl<<" Directory = "<<cla.getPathSpecs();

					std::vector<std::string> filePattern(cla.getFilePattern());

					std::cout<<std::endl<<" File Pattern = ";

					for ( int i = 0 ; i < (int)filePattern.size() ; i++ )
					{
						std::cout<<" "<<filePattern[i]<<" ";
					}

					cla.printValidFiles();
			 break ;
		}

	}
	else 
	{
		std::cout<<std::endl<<" Invalid Arguments. \n\n" ;

		cla.printManual();
	}
}

#endif