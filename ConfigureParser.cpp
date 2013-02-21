////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - Added ConfigParsePassTwo for second pass.											  // 																					   
//	ver 1.2             Added More Rules & Actions.															  //	
//																											  //
//	Language		:   Visual C++, 2010																	  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 1.1                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

///////////////////////////////////////////////////////////////////
///////////////////// XML Metadata Manager ///////////////////////
///////////////////////////////////////////////////////////////////

//----< constructor initializes pointers to all parts >--------------

ConfigParseMetadata::ConfigParseMetadata() 
  : pToker(0), pSemi(0), pParser(0), 
    pIsTitle(0), pDoIsTitle(0), pIsDescription(0), pDoIsDescription(0),
	pIsDependency(0),pDoIsDependency(0), pIsGeneralTitle(0),pDoIsGeneralTitle(0),
	pIsGeneralDescription(0),pDoIsGeneralDescription(0), pPackage(0) {}

//----< destructor releases all parts >------------------------------

ConfigParseMetadata::~ConfigParseMetadata()
{
  // when Builder goes out of scope, everything must be deallocated
  
  // Deallocate Variables.
  delete pPackage;

  // Deallocate Rules & Actions.
  delete pIsTitle;
  delete pDoIsTitle;

  delete pIsGeneralDescription;
  delete pIsGeneralTitle;

  delete pDoIsGeneralDescription;
  delete pDoIsGeneralTitle;

  delete pIsDescription;
  delete pDoIsDescription;

  delete pIsDependency;
  delete pDoIsDependency;

  // Deallocate Parser, Semi and Toker.
  delete pParser;
  delete pToker;
  delete pSemi;
}

//----< attach toker to a file stream or stringstream >------------
//----< Set the package name based on the file parsed >------------

bool ConfigParseMetadata::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;

  std::string package_name = name;

  package_name = package_name.substr(package_name.find_last_of("\\") + 1 );
  package_name = package_name.substr(0,package_name.find("."));

  pPackage->setPackageName(package_name);

  return pToker->attach(name, isFile);
}
//
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseMetadata::Build()
{
  try
  {
    // configure to detect and act on pass two

    pToker = new Toker; pToker->returnComments();
    pSemi = new SemiExp(pToker); pSemi->makeCommentSemiExp();
    pParser = new Parser(pSemi);

	pPackage = new Package("");

	// configure to add rules and action.

	pIsTitle = new IsTitle(*pPackage); pDoIsTitle = new doIsTitle(*pPackage);
	pIsTitle->addAction(pDoIsTitle); pParser->addRule(pIsTitle);

	pIsGeneralTitle = new IsGeneralTitle(*pPackage); pDoIsGeneralTitle = new doIsGeneralTitle(*pPackage);
	pIsGeneralTitle->addAction(pDoIsGeneralTitle); pParser->addRule(pIsGeneralTitle);

	pIsDescription = new IsDescription(*pPackage); pDoIsDescription = new doIsDescription(*pPackage);
	pIsDescription->addAction(pDoIsDescription); pParser->addRule(pIsDescription);

	pIsGeneralDescription = new IsGeneralDescription(*pPackage); pDoIsGeneralDescription = new doIsGeneralDescription(*pPackage);
	pIsGeneralDescription->addAction(pDoIsGeneralDescription); pParser->addRule(pIsGeneralDescription);

	pIsDependency = new IsDependency(*pPackage); pDoIsDependency = new doIsDependency(*pPackage);
	pIsDependency->addAction(pDoIsDependency); pParser->addRule(pIsDependency);

    return pParser;
  }
  catch(std::exception& ex)
  { std::cout << "\n\n  " << ex.what() << "\n\n"; return 0; }
}

//----< Function returning pPackage. >------------

Package ConfigParseMetadata::GetPacakge() 
{ 
	return *pPackage; 
}

#ifdef TEST_ANALYZER_PARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureAnalyzer module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseMetadata configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

	  std::cout << "\n\n  Parser Parsing File\n\n";

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";

	  std::cout << "\n\n  Parsing Complete\n\n";

	  Package pk = configure.GetPacakge();

    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
//
    std::cout << "\n\n";
  }
  std::getchar();
}

#endif
