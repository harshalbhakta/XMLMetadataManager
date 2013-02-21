////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CustomNavigate.cpp (ver 1.0)  -  Module to parse the directory and fetch files based on cmd arguments	  // 																					   
//		 																									  //
//	Language		:   Visual C++, 2010																	  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CustomNavigate.h"

using namespace Win32Tools;

/////////////////////////////////////////////////////////////////////
//  Function to fetcht the files recursively / non recursively.
//  Returns only valid files.
std::vector<std::string> CustomNavigate::getFiles(std::string path,std::vector<std::string> pattern,bool recurse)
{
	files.clear();

	invalidDirectoryPath.clear();

	ignoredFiles.clear();

	std::string full_path = Path::getFullPath(path);

	if ( validateDirectory(full_path) )  // Recurse
		{ Walk(full_path,pattern,recurse); }
	else  // Non-Recurse
		{ invalidDirectoryPath.push_back(full_path); }	

	return files;
}

///////////////////////////////////////////////////////////////
//  Function to check if the directory path is valid.
bool CustomNavigate::validateDirectory(std::string path)
{
	if ( Directory::DirectoryExists(path) )
	{ return true; }
	else 
	{ return false; }	
}

//////////////////////////////////////////////////////////////////////////////
//  Function that supports recursive call to itself for directory traversal.
void CustomNavigate::Walk(std::string full_path,std::vector<std::string> patternList, bool recurse)
{
	if ( recurse )
	{	
		for each (std::string pattern in patternList )
		{	
			std::vector<std::string> tempFiles = Directory::GetFiles(full_path,pattern);
		
			for each ( std::string file in tempFiles )
			{ 
				if ( !(full_path[full_path.length()-1] == '\\') )		
					{ addFile(full_path , file); }
			}
		} // Files matching pattern in current Dir fetched.

		std::vector<std::string> tempDirs = Directory::GetDirectories(full_path,"*.*"); // Get Childern Directory.
	
		for each ( std::string dir in tempDirs )
		{ Walk(dir,patternList,recurse);  }	// Recursive Call.
	}
	else // Non Recurse
	{
		for each (std::string pattern in patternList )
		{	
			std::vector<std::string> tempFiles = Directory::GetFiles(full_path,pattern);
		
			for each ( std::string file in tempFiles )
			{ 
				if ( !(full_path[full_path.length()-1] == '\\') )		
					{ addFile(full_path , file); } // Single Directory Files fetched.
			}
		} 
	}
}


///////////////////////////////////////////////////////////////
//  Function to add File list. Repeated file name ignored.
void CustomNavigate::addFile(std::string dirPath ,std::string file)
{
	bool ToAdd = true;

	for each ( std::string entry in files ) 
	{
	  if ( file == entry.substr(entry.find_last_of("\\")+1) ) 
		{ 
			ToAdd = false;  
			ignoredFiles.push_back(dirPath+ "\\" +file); // If file with same name already exist, ignore.
			break; 
		} 
	}
	
	if ( ToAdd )
	{ files.push_back(dirPath+ "\\" +file); }
}

///////////////////////////////////////////////////////////////
//  Print the Files Fetched & Ignored Information.
void CustomNavigate::printFileInfo()
{

	std::cout << "\n\n\n  Invalid Directory Path \n "
            << std::string(22,'=') << std::endl;

	for each ( std::string dir in invalidDirectoryPath )
	{ 
		std::cout<<std::endl<<" "<<dir;
	}

	std::cout << "\n\n\n  Files Ignored (Repeated Filename)\n "
            << std::string(22,'=') << std::endl;

	for each ( std::string file in ignoredFiles )
	{ 
		std::cout<<std::endl<<" "<<file;
	}

}

///////////////////////////////////////////////////////////////
//  Test Stub.
#ifdef TEST_CUSTOM_NAVIGATE

void main(int argc, char *argv[]) {

	std::cout << "\n  Testing CustomNavigate Module on Parent Directory " << "\n "
			  << std::string(55,'=');

	std::cout << "\n Navigate for multiple file pattern : *.cpp, Testing, *.h " << "\n "
		      << std::string(55,'-');

	std::vector<std::string> tempPattern;

	tempPattern.push_back("*.cpp");
	tempPattern.push_back("Testing");
	tempPattern.push_back("*.h");
	
	std::string tempDirs = ".."; bool recurse = false;

	std::cout << "\n Create CustomNavigate object and call getFiles. " << "\n "
		      << std::string(55,'-');

	CustomNavigate cn;

	std::vector<std::string>listFiles = cn.getFiles(tempDirs,tempPattern,recurse);
	
	std::cout << "\n Check printing file info by printFileInfo(). " << "\n "
		      << std::string(55,'-');

	cn.printFileInfo();

	std::cout << "\n\n\n  Files Accepted \n "
            << std::string(55,'-') << std::endl;

	for each ( std::string file in listFiles )
	{
		std::cout<<std::endl<<" "<<file;
	}

	std::cout<<"\n \n ";
}

#endif