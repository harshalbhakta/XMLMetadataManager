/////////////////////////////////////////////////////////////////////
//  MetadataCreator.h - Generate Metadata XM: Files.               //
//  ver 1.2                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include <ctype.h>
#include "XmlDocument.h"
#include "XmlElementParts.h"
#include "WinTools.h"
#include "MetadataCreator.h"
#include <cctype>

using namespace Win32Tools;
using namespace XmlProcessing;

///////////////////////////////////////////////////////////////////////////
//  Constructor to check if the Repository Folder exists.
//  Create the folder if it does not exist.
MetadataCreator::MetadataCreator()
{ CheckRepositoryFolder(); }

///////////////////////////////////////////////////////////////////////////
//  Format the text to convert all characters to lower case.
std::string MetadataCreator::FormatToLower(std::string text)
  {
		std::string formatted = "";

		for ( int i = 0 ; i < (int)text.length() ; i++)
		{
			formatted.push_back(std::tolower(text[i]));
		}

		return formatted;
  }

///////////////////////////////////////////////////////////////////////////
//  Function to check if a file exist or not.
bool MetadataCreator::isFileExist(std::string _package_name)
{
	try 
	{
		std::ifstream ifile( REPOSITORY_LOCATION + _package_name + ".xml");
  
		if ( ifile )
		{ return true; }
  
		return false;
	}
	catch ( std::exception& ex )
	{
		std::cout<<"\n Error Accessing File :"<<REPOSITORY_LOCATION + _package_name + ".xml \n" << " Error :"<<ex.what();
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////
//  Function to check and create Repository folder if already not present.
void MetadataCreator::CheckRepositoryFolder()
{
	std::string path = REPOSITORY_LOCATION;

	Directory dir;

	if ( ! (dir.DirectoryExists(path)) )
	{
		dir.CreateDirectoryA(path);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Function to Create XML File skeleton with only the base pacakge root.
void MetadataCreator::CreateXMLFile(std::string _package_name)
{
	
  try 
  {		  _package_name = FormatToLower(_package_name);	
		  XmlDocumentFactory docFactory;
		  XmlNodeFactory nodeFactory;

		  //// Create Document.
		  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
		  pDocCreatePart->addDocDeclaration();
 
		  // Create Root Element.
		  IXmlNode* pPackageRoot = nodeFactory.CreateXmlNode(IXmlNode::Element, "Package");

		  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);
  	  
		  if ( pPackageRoot == 0 || pDocElement == 0 || pDocCreatePart == 0)
			  throw "\n\n Error Creating Xml. \n\n";

		  pDocCreatePart->insertElement(pDocElement,pPackageRoot);	// Root Element.

		  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml"); // Pacakge tag.

		  AddPackageName(_package_name);
  }
  catch ( char* msg )
  { std::cout<<"\n Error: "<<msg; throw "\n\n Error Creating XML. \n\n"; }
  catch ( std::exception Ex )
  {	throw "\n\n Error Creating XML. \n\n"; }
}

///////////////////////////////////////////////////////////////////////////
//  Function to add package name to the XML Metadata file.
void MetadataCreator::AddPackageName(std::string _package_name)
{
  try 
  {  _package_name = FormatToLower(_package_name);

  if  ( !isFileExist(_package_name) ) // Create file if not present.
  { CreateXMLFile(_package_name); }

  XmlDocumentFactory docFactory;
  XmlNodeFactory nodeFactory;

  //// Create Document.
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  
  pDocCreatePart->loadFile( REPOSITORY_LOCATION +_package_name+".xml");

  // Get docElement.
  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);

  // Create Root Element.
  IXmlNode* pPackageRoot = pDocCreatePart->findElement("Package",IXmlDocument::tag);

  IXmlNode* pPackageName = pDocCreatePart->findElement("Name",IXmlDocument::tag);

  if ( pPackageName == 0 )
  {  
	  pPackageName = nodeFactory.CreateXmlNode(IXmlNode::Element, "Name");	  
	  
	  IXmlNode* pPackageNameText = nodeFactory.CreateXmlNode(IXmlNode::Text, _package_name);

	  if ( pPackageName == 0 || pDocElement == 0 || pPackageRoot == 0 || pPackageNameText == 0 || pDocCreatePart == 0 )
		   throw "\n\n Error Adding Package Name. \n\n";

      pPackageName->addChild(pPackageNameText);

	  pDocCreatePart->insertElement(pPackageRoot,pPackageName);	

	  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");
  }

  }
  catch ( std::exception Ex )
  {	throw "\n\n Error Adding Package Name. \n\n"; }
}

//////////////////////////////////////////////////////////////////////////
//  Function to add title text to the XML Metadata file.
//  Overwrite the name if already exist in file.
void MetadataCreator::AddTitle(std::string _package_name,std::string _title)
{
   try 
   {   if  ( !isFileExist(_package_name) ) { CreateXMLFile(_package_name); }
	  _title = FormatText(_title); _title = "\"" + _title + "\"";

	  // Create Document & Load File.
	  XmlDocumentFactory docFactory; XmlNodeFactory nodeFactory;
	  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
	  pDocCreatePart->loadFile( REPOSITORY_LOCATION +_package_name+".xml");
	  
	  if ( pDocCreatePart == 0 ) { throw "\n\n Error Adding Title. \n\n"; }

	  // Get docElement & Root Element.
	  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);
	  IXmlNode* pPackageRoot = pDocCreatePart->findElement("Package",IXmlDocument::tag);

	  if ( pDocElement == 0 || pPackageRoot == 0 ) throw "\n\n Error Adding Title. \n\n";

	  IXmlNode* pPackageTitle = pDocCreatePart->findElement("Title",IXmlDocument::tag);

	  if ( pPackageTitle == 0 ) // If title not added. Add.
	  {  
		  pPackageTitle = nodeFactory.CreateXmlNode(IXmlNode::Element, "Title");
		  IXmlNode* pPackageTitleText = nodeFactory.CreateXmlNode(IXmlNode::Text, _title);

		  if ( pPackageTitle == 0 || pDocElement == 0 || pPackageRoot == 0 || pPackageTitleText == 0 || pDocCreatePart == 0 )
			   throw "\n\n Error Adding Title. \n\n";

		  pPackageTitle->addChild(pPackageTitleText);
		  pDocCreatePart->insertElement(pPackageRoot,pPackageTitle);	
		  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");
	  }
	  else // Overwrite
	  {   std::vector<IXmlNode*> children = pPackageTitle->getChildren();
		  if(children.size() > 0)
		  {
			if ( children[0] == 0 || pDocCreatePart == 0) { throw "\n\n Error Adding Title. \n\n"; }
			children[0]->value() = _title;
		  }
		  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");  
	  }
  }
  catch ( std::exception Ex )
  {	throw "\n\n Error Adding Title. \n\n"; }
}

//////////////////////////////////////////////////////////////////////////
//  Function format text to ignore the special charaters.
std::string MetadataCreator::FormatText(std::string _desc)
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

//////////////////////////////////////////////////////////////////////////
//  Function to add description text to the XML Metadata file.
//  Overwrite the name if already exist in file.
void MetadataCreator::AddDescription(std::string _package_name,std::string _desc)
{
	try
	{     if  ( !isFileExist(_package_name) ) { CreateXMLFile(_package_name); }
		  _desc = FormatText(_desc); _desc = "\"" + _desc + "\"";

		  //// Create Document.
		  XmlDocumentFactory docFactory; XmlNodeFactory nodeFactory;

		  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();  
		  pDocCreatePart->loadFile( REPOSITORY_LOCATION +_package_name+".xml");
		  if ( pDocCreatePart == 0 ) { throw "\n\n Error Adding Description. \n\n"; }

		  // Get docElement & Get Root Element.
		  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);
		  IXmlNode* pPackageRoot = pDocCreatePart->findElement("Package",IXmlDocument::tag);

		  if ( pDocElement == 0 || pPackageRoot == 0 ) { throw "\n\n Error Adding Description. \n\n"; }

		  IXmlNode* pPackageDesc = pDocCreatePart->findElement("Description",IXmlDocument::tag);

		  if ( pPackageDesc == 0 ) // IF desc. not added. add.
		  {  
			  pPackageDesc = nodeFactory.CreateXmlNode(IXmlNode::Element, "Description");
			  IXmlNode* pPackageDescText = nodeFactory.CreateXmlNode(IXmlNode::Text, _desc);

			   if ( pPackageDesc == 0 || pDocElement == 0 || pPackageRoot == 0 || pPackageDescText == 0 || pDocCreatePart == 0 )
					throw "\n\n Error Adding Description. \n\n";

			  pPackageDesc->addChild(pPackageDescText);
			  pDocCreatePart->insertElement(pPackageRoot,pPackageDesc);	
			  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");
		  }
		  else // Overwrite.
		  {   std::vector<IXmlNode*> children = pPackageDesc->getChildren();
			  if(children.size() > 0)
			  {
				if ( children[0] == 0 || pDocCreatePart == 0) { throw "\n\n Error Creating Xml. \n\n"; }
				children[0]->value() = _desc;
			  }
			  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");  
		  }
   }
  catch ( std::exception Ex )
  {	throw "\n\n Error Adding Description. \n\n"; }
}

//////////////////////////////////////////////////////////////////////////
//  Function to add header name to the XML Metadata file.
//  Overwrite the name if already exist in file.
void MetadataCreator::AddHeader(std::string _package_name,std::string _header)
{

   try 
   {  if  ( !isFileExist(_package_name) ) { CreateXMLFile(_package_name); }
      _header = "\"" + _header + "\""; 

  //// Create Document.
  XmlDocumentFactory docFactory; XmlNodeFactory nodeFactory;
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  pDocCreatePart->loadFile( REPOSITORY_LOCATION +_package_name+".xml");

  if ( pDocCreatePart == 0 ) { throw "\n\n Error Adding Header. \n\n"; }

  // Get docElement & Get Root Element.
  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);
  IXmlNode* pPackageRoot = pDocCreatePart->findElement("Package",IXmlDocument::tag);

  if ( pDocElement == 0 || pPackageRoot == 0 ) { throw "\n\n Error Adding Header. \n\n"; }

  IXmlNode* pPackageHeader = pDocCreatePart->findElement("Header",IXmlDocument::tag);

  if ( pPackageHeader == 0 ) // if header is not added. add.
  {  
	  pPackageHeader = nodeFactory.CreateXmlNode(IXmlNode::Element, "Header");
	  IXmlNode* pPackageHeaderText = nodeFactory.CreateXmlNode(IXmlNode::Text, _header);

	  if ( pPackageHeader == 0 || pDocElement == 0 || pPackageRoot == 0 || pPackageHeaderText == 0 || pDocCreatePart == 0)
			throw "\n\n Error Adding Header. \n\n";

      pPackageHeader->addChild(pPackageHeaderText);
	  pDocCreatePart->insertElement(pPackageRoot,pPackageHeader);	
	  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");
  }
  else // overwrite.
  {   std::vector<IXmlNode*> children = pPackageHeader->getChildren();
	  if(children.size() > 0)
	  {
		if ( children[0] == 0 || pDocCreatePart == 0) { throw "\n\n Error Adding Header. \n\n"; }
		children[0]->value() = _header;
	  }
	  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml"); }
  }
  catch ( std::exception Ex )
  {	throw "\n\n Error Adding Header. \n\n"; }
}

//////////////////////////////////////////////////////////////////////////
//  Function to add source name to the XML Metadata file.
//  Overwrite the name if already exist in file.
void MetadataCreator::AddSource(std::string _package_name,std::string _source)
{ 
  try
  {  if  ( !isFileExist(_package_name) ) { CreateXMLFile(_package_name); }
     _source = "\"" + _source + "\"";

   //// Create Document.
  XmlDocumentFactory docFactory; XmlNodeFactory nodeFactory;
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  pDocCreatePart->loadFile( REPOSITORY_LOCATION +_package_name+".xml");

  if ( pDocCreatePart == 0 ) { throw "\n\n Error Adding Soruce. \n\n"; }

  // Get docElement & Get Root Element.
  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);
  IXmlNode* pPackageRoot = pDocCreatePart->findElement("Package",IXmlDocument::tag);

  if ( pDocElement == 0 || pPackageRoot == 0 ) { throw "\n\n Error Adding Soruce. \n\n"; }

  IXmlNode* pPackageSource = pDocCreatePart->findElement("Source",IXmlDocument::tag);

  if ( pPackageSource == 0 ) // if source not added.
  {   pPackageSource = nodeFactory.CreateXmlNode(IXmlNode::Element, "Source");
	  IXmlNode* pPackageSourceText = nodeFactory.CreateXmlNode(IXmlNode::Text, _source);
      
	  if ( pPackageSource == 0 || pDocElement == 0 || pPackageRoot == 0 || pPackageSourceText == 0 || pDocCreatePart == 0)
					throw "\n\n Error Adding Soruce. \n\n";
	  
	  pPackageSource->addChild(pPackageSourceText);
	  pPackageRoot->addChild(pPackageSource);
	  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");
  }
  else // overwrite.
  {
	  std::vector<IXmlNode*> children = pPackageSource->getChildren();
	  if(children.size() > 0)
	  {
		if ( children[0] == 0 || pDocCreatePart == 0) { throw "\n\n Error Creating Xml. \n\n"; }
		children[0]->value() = _source;
	  }
	  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml"); }
  }
  catch ( std::exception Ex )
  {	throw "\n\n Error Adding Soruce. \n\n"; }
}

//////////////////////////////////////////////////////////////////////////
//  Function to add depenency name to the XML Metadata file.
//  ignore already existing entry. does not add duplicate entry.
void MetadataCreator::AddDependency(std::string _package_name,std::string _dependency)
{
   try
   {  _dependency = FormatToLower(_dependency); _dependency = "\"" + _dependency + "\""; _package_name = FormatToLower(_package_name);
	   if  ( !isFileExist(_package_name) ) { CreateXMLFile(_package_name); }
	   
	    //// Create Document.
		XmlDocumentFactory docFactory; XmlNodeFactory nodeFactory;
		IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
		pDocCreatePart->loadFile( REPOSITORY_LOCATION +_package_name+".xml");

		if ( pDocCreatePart == 0 ) { throw "\n\n Error Adding Dependency. \n\n"; }

  // Get docElement & Get Root Element.
  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);
  IXmlNode* pPackageRoot = pDocCreatePart->findElement("Package",IXmlDocument::tag);

  if ( pDocElement == 0 || pPackageRoot == 0 ) { throw "\n\n Error Adding Dependency. \n\n"; }

  std::vector<IXmlNode*> pPackageDepList = pDocCreatePart->findElements("DepPackage",IXmlDocument::tag);
  
  bool is_add = true;

  for(size_t i=0; i<pPackageDepList.size(); ++i)
  {
    std::vector<IXmlNode*> children = pPackageDepList[i]->getChildren();
    if(children.size() > 0 && (_dependency == (children[0]->value()) ) )
	{ is_add = false; break; }
  }
  if ( is_add ) // Ignore if the entry already exist in XML.
  {  
	  IXmlNode* pPackageDependency = nodeFactory.CreateXmlNode(IXmlNode::Element, "DepPackage");
	  IXmlNode* pPackageDependencyText = nodeFactory.CreateXmlNode(IXmlNode::Text, _dependency);
      
	  if ( pPackageDependency == 0 || pDocElement == 0 || pPackageRoot == 0 || pPackageDependencyText == 0 || pDocCreatePart == 0 )
					throw "\n\n Error Adding Dependency. \n\n";

	  pPackageDependency->addChild(pPackageDependencyText);
	  pPackageRoot->addChild(pPackageDependency);	  
	  pDocCreatePart->ToFile( REPOSITORY_LOCATION +_package_name+".xml");
  }
  }
  catch ( std::exception Ex )
  {	throw "\n\n Error Adding Dependency. \n\n"; }
}

//////////////////////////////////////////////////////////////////////////
//  Function to generate XML file from Package Object.
void MetadataCreator::GenerateMetadataFile(Package pk)
{
	try
	{
		CreateXMLFile(FormatToLower(pk.getPackageName()));

		AddTitle(FormatToLower(pk.getPackageName()),pk.getTitle());

		AddDescription(FormatToLower(pk.getPackageName()),pk.getDescription());

		AddHeader(FormatToLower(pk.getPackageName()),pk.getHeader());

		AddSource(FormatToLower(pk.getPackageName()),pk.getSource());

		std::vector<std::string> dep = pk.getDependencyList();

		for ( int i = 0 ; i < (int)dep.size() ; i++ )
		{  AddDependency(FormatToLower(pk.getPackageName()),dep[i]); }

	}
	catch ( char* msg )
	{
		std::cout<<"\n "<<msg;
		throw "\n\n Error Generating XML : "; 
	}
	catch ( std::exception ex )
	{ 
		throw "\n\n Error Generating XML : "; 
	}
}

//////////////////////////////////////////////////////////////////////////
//  Function to display the XML file.
void MetadataCreator::ShowXMLFile(std::string _package_name)
{
	try 
	{	_package_name = FormatToLower(_package_name);
		if ( isFileExist(_package_name) )
		{
			std::cout<<"\n";

			XmlDocumentFactory docFactory;

			IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();

			pDocCreatePart->loadFile( REPOSITORY_LOCATION + _package_name + ".xml" );

			if ( pDocCreatePart == 0 )
				throw "\n XML File does not exist or error opening file.";

			std::cout<<pDocCreatePart->ToString();
		
			std::cout<<"\n";
		}
		else 
		{ throw "\n XML File does not exist or error opening file."; }
	}
	catch (char* msg)
	{ throw msg; }
	catch (std::exception& ex )
	{ std::cout<<"\n Error : " << ex.what(); throw "\n XML File does not exist or error opening file."; }
}

//////////////////////////////////////////////////////////////////////////
//  Test Stub.

#ifdef TEST_METADATA_CREATOR

int main(int argc, char* argv[])
{
	 std::cout << "\n  Testing Create New File with all functions \n "
            << std::string(21,'=') << std::endl;

	 MetadataCreator mc;	 

	 mc.CreateXMLFile("Parser");	

	 std::cout << "\n  Testing Create New Filw With Empty Package Tag \n "
            << std::string(21,'=') << std::endl;

	 mc.AddTitle("Parser","Testing");

	 //mc.AddTitle("Parser","Package to parse \files modified.");

	 mc.AddDescription("Parser","Parser Description");

	 mc.AddDescription("Parser","Parser Description Modified.");

	 mc.AddHeader("Parser","Parser.h");

	 mc.AddSource("Parser","Parser.cpp");

	 mc.AddDependency("Parser","Toker");

	 mc.AddDependency("Parser","Semi");

	 mc.AddDependency("Parser","Toker");

	 XmlDocumentFactory docFactory;

	 IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
     
	 std::string str = "Parser.xml";

	 pDocCreatePart->loadFile( REPOSITORY_LOCATION + str );

	 pDocCreatePart->ToFile( REPOSITORY_LOCATION + str );

	 pDocCreatePart->loadFile( REPOSITORY_LOCATION + str );

	 std::cout<<"\n\n File Written is : \n";

	 std::cout<<pDocCreatePart->ToString();

	 std::cout << "\n\n  Testing Create New File from Package \n "
            << std::string(21,'=') << std::endl;

	 Package pk("Semi");
	 pk.setTitle("Package to generate Semi Expression");
	 pk.setDescription("Semi package description.");
	 pk.setHeader("C:\\Semi.h");
	 pk.setSource("C:\\Semi.cpp");
	 pk.addDependency("Toker");
	 pk.addDependency("Testing");
	 pk.addDependency("Testing2");

	 mc.GenerateMetadataFile(pk);

	 pDocCreatePart = docFactory.CreateXmlDocument();
     
	 str = "Semi.xml";

	 pDocCreatePart->loadFile(REPOSITORY_LOCATION + str );
	 
	 std::cout<<"\n\n File Written is : \n";

	 std::cout<<pDocCreatePart->ToString();
}

#endif