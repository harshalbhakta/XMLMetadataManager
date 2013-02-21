/////////////////////////////////////////////////////////////////////
//  Scanner.h - Scan XML Metadata Package Files.				   //
//  ver 1.0                                                        //
//                                                                 //
//  Language:      Microsoft Visual C++ 2010                       //
//  Platform:      Toshiba Satellite R25-S3503,Win7 Pro.           //
//  Application:   CSE687 Pr2, Sp11				                   //
//  Author:        Harshal Bhakta, Syracuse University			   //
//                 (714) 876-4273, harshal.c.bhakta@gmail.com      //
/////////////////////////////////////////////////////////////////////

#include "Scanner.h"

using namespace XmlProcessing;

////////////////////////////////////////////////////////////////////
// Function to check if a file exist or not.
bool Scanner::isFileExist(std::string _package_name)
{
  std::ifstream ifile(_package_name);
  
  if ( ifile )
  { return true; }
  
  return false;
}

////////////////////////////////////////////////////////////////////
// Function to check if Reposiotry folder exists or not.
bool Scanner::isFolderExist()
{
	return Win32Tools::Directory::DirectoryExists(REPOSITORY_LOCATION);	
}

////////////////////////////////////////////////////////////////////
// Function to Add a file to the list of traversed files.
// Add file only if file does not exist in the list.
void Scanner::AddFileToList(std::string file)
{
	std::vector<std::string>::iterator it; bool add_flag = true;
	
	for ( it = files.begin(); it < files.end() ; it++)
	{ if ( *it == file ) { add_flag = false; } }
	
	if ( add_flag ) { files.push_back(file);  }

}

////////////////////////////////////////////////////////////////////
// Function to convert the text to lower case.
std::string Scanner::FormatToLower(std::string text)
  {
		std::string formatted = "";

		for ( int i = 0 ; i < (int)text.length() ; i++)
		{
			formatted.push_back(std::tolower(text[i]));
		}

		return formatted;
  }

////////////////////////////////////////////////////////////////////
// Recursive Function to scan through metadata repository.
// Starting at base package.
void Scanner::ScanPackageRecurse(std::string _base_package)
{
	if ( !isFolderExist() ) { throw "\n\nMetadataRepository Folder does not exist."; return; } 
	try 
	{    	_base_package = FormatToLower(_base_package);
			std::string _base_package_path = REPOSITORY_LOCATION + _base_package + ".xml" ;
			if ( isFileExist(_base_package_path) )
			{	// Ignore the already completed file to avoid circular recursion.
				if ( CompletedPackages.find(_base_package) != CompletedPackages.end() ) { return; } 
				else { CompletedPackages.insert(_base_package); }
		
				Package pk(_base_package);

				pk.setHeader(GetHeaderFile(_base_package_path));
				pk.setSource(GetSourceFile(_base_package_path));
				pk.setTitle(GetTitle(_base_package_path));

				if ( pk.getHeader() != "NA"  )
				{  AddFileToList(pk.getHeader()); }
				if ( pk.getSource() != "NA"  )
				{  AddFileToList(pk.getSource()); }

				std::vector<std::string> packages = GetPackages(_base_package_path);
				for ( int i = 0 ; i < (int)packages.size() ; i++ )
				{ pk.addDependency(packages[i]);}

				Display dp; dp.PrintScanPackage(pk);

				for ( int j = 0 ; j < (int)packages.size() ; j++ )
				{ ScanPackageRecurse(packages[j]);} // Recurse for all dependencies.
			}
			else
			{ std::cout<< "\n Package XML does not exist for : "<<_base_package; return; }
	}
	catch ( std::exception& ex )
	{ std::cout<< "\n Error Scanning Package XML for : "<<_base_package <<"\n Error : "<<ex.what() <<"\n" ; 
	  CompletedPackages.insert(_base_package); // Package Error. Add to Completed List.
	}
	catch ( ... )
	{ std::cout<< "\n Error Scanning Package XML for : "<<_base_package <<"\n" ; 
	  CompletedPackages.insert(_base_package); // Package Error. Add to Completed List.
	}
}

////////////////////////////////////////////////////////////////////
// Funciton to fetch the header file from the XML.
std::string Scanner::GetHeaderFile(std::string _package_path)
{
  std::string header = "\"NA\"";

  XmlDocumentFactory docFactory;

  //// Create Document.
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  
  pDocCreatePart->loadFile(_package_path);

  IXmlNode* pPackageHeader = pDocCreatePart->findElement("Header",IXmlDocument::tag);

  if ( pPackageHeader != 0 )
  {  
	  std::vector<IXmlNode*> children = pPackageHeader->getChildren();
   
	  if(children.size() > 0)
	   header = children[0]->value();
  }

  header = header.substr(1,header.length()-2);

  return header;

}

////////////////////////////////////////////////////////////////////
// Funciton to fetch the Source file from the XML.
std::string Scanner::GetSourceFile(std::string _package_path)
{
  std::string source = "\"NA\"";

  XmlDocumentFactory docFactory;

  //// Create Document.
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  
  pDocCreatePart->loadFile(_package_path);

  IXmlNode* pPackageSource = pDocCreatePart->findElement("Source",IXmlDocument::tag);

  if ( pPackageSource != 0 )
  {  
	  std::vector<IXmlNode*> children = pPackageSource->getChildren();
   
	  if(children.size() > 0)
	   source = children[0]->value();
  }

  source = source.substr(1,source.length()-2);

  return source;
}

////////////////////////////////////////////////////////////////////
// Funciton to fetch the Title from the XML.
std::string Scanner::GetTitle(std::string _package_path)
{

  std::string title = "\"NA\"";

  XmlDocumentFactory docFactory;

  //// Create Document.
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  
  pDocCreatePart->loadFile(_package_path);

  IXmlNode* pPackageHeader = pDocCreatePart->findElement("Title",IXmlDocument::tag);

  if ( pPackageHeader != 0 )
  {  
	  std::vector<IXmlNode*> children = pPackageHeader->getChildren();
   
	  if(children.size() > 0)
	   title = children[0]->value();
  }

  title = title.substr(1,title.length()-2);

  return title;

}


////////////////////////////////////////////////////////////////////
// Funciton to fetch the dependency packages from the XML.
std::vector<std::string> Scanner::GetPackages(std::string _package_path)
{
  std::vector<std::string> packages;

  XmlDocumentFactory docFactory;

  //// Create Document.
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  
  pDocCreatePart->loadFile(_package_path);

  std::vector<IXmlNode*> pPackageDepList = pDocCreatePart->findElements("DepPackage",IXmlDocument::tag);

  for(size_t i=0; i < (size_t)pPackageDepList.size(); ++i)
  {
    std::vector<IXmlNode*> children = pPackageDepList[i]->getChildren();
    
	if(children.size() > 0)
	{  
		std::string dep = children[0]->value() ; 
		if ( dep.length() > 2 )
		{ dep = dep.substr(1,dep.length()-2); }
		packages.push_back(dep);
	}
  }

  return packages;
}

////////////////////////////////////////////////////////////////////
// Funciton to fetch the files traversed for all the packages.
std::vector<std::string> Scanner::GetFiles()
{ return files; }

//--------------TEST STUB----------------//

#ifdef TEST_SCANNER

int main(int argc,char* argv[])
{

	std::cout << "\n\n Analyzing from Base Pacakge : " << "Main\n "
              << std::string(21,'=') << std::endl;

	Scanner sc;

	sc.ScanPackageRecurse("Main");

	std::vector<std::string> files = sc.GetFiles();

	std::cout << "\n\n Scanning from Base Pacakge : " << "Main\n "
              << std::string(21,'=') << std::endl;

	std::vector<std::string>::iterator it;

	for (it=files.begin(); it!=files.end(); it++)
        std::cout << "\n " << *it;

	std::cout << "\n ";
}

#endif