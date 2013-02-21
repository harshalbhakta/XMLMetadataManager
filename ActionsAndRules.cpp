////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - Added Customized rules for XML Metadata Detection.									  // 																					   
//	ver 1.2																									  //	
//																											  //
//	Language		:   Visual C++, 2010																	  //
//	Platform		:   Toshiba Satellite (R25-S3503), Windows 7 Professional								  //
//	Application		:	CSE687 - Project #1 - CodeMetricsAnalyzer, Spring 2011								  //
//	Author			:   Harshal Bhakta, Syracuse University.												  //
//	E-mail			:   harshal.c.bhakta@gmail.com															  //
//	Cell			:   714-876-4273																		  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.cpp - implements new parsing rules and actions //
//  ver 1.1                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include "ActionsAndRules.h"
#include "Package.h"
#include <cctype>

///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
IsGeneralTitle::IsGeneralTitle(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Rule to check if comment contains the Title.
  bool IsGeneralTitle::doTest(ITokCollection* pTc)
  {
        ITokCollection& tc = *pTc;

		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("//") == 0 )
			{ 
				for ( int j = 0 ; j < (int)tc[i].length() ; j++)
				{
				   if ( ( tc[i].at(j) ) != '/' )
				   {
						doActions(pTc);return true; 
				   }
				}
			}
		}
		return false;
  }

 ///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  doIsGeneralTitle::doIsGeneralTitle(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Function store the Title in the Package.
  void doIsGeneralTitle::doAction(ITokCollection* pTc)
  {
	 ITokCollection& tc = *pTc;

	 int token_index = 0;

		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("//") == 0 )
			{  	token_index = i; break; }
		}

	 std::string comment = tc[token_index]; 

	 ptrPackage->setTitle(comment);
  }

///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  IsTitle::IsTitle(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Rule to check if comment contains the Title.
bool IsTitle::doTest(ITokCollection* pTc)
  {
        ITokCollection& tc = *pTc;
		
		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("//") == 0 && tc[i].find(ptrPackage->getPackageName()) != -1 )
			{ 
				doActions(pTc);
				return true; 
			}
		}

		return false;
  }

 ///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  doIsTitle::doIsTitle(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Function store the Title in the Package.
  void doIsTitle::doAction(ITokCollection* pTc)
  {
	 ITokCollection& tc = *pTc;

	 int token_index = 0;

		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("//") == 0 && tc[i].find(ptrPackage->getPackageName()) != -1 )
			{  	token_index = i; break; }
		}

	 std::string comment = tc[token_index]; 

	 ptrPackage->setTitleForce(comment);
  }

  ///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  IsGeneralDescription::IsGeneralDescription(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Rule to check if comment contains module description.
bool IsGeneralDescription::doTest(ITokCollection* pTc)
  {
        ITokCollection& tc = *pTc;

		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("/*") != -1 )
			{ 
				doActions(pTc);
				return true; 
			}
		}

		return false;
  }

 ///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
doIsGeneralDescription::doIsGeneralDescription(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Function to store the description in the Package.
  void doIsGeneralDescription::doAction(ITokCollection* pTc)
  {
	 ITokCollection& tc = *pTc;

	 int token_index = 0;

		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("/*") == 0 )
			{  	token_index = i; break; }
		}

	 std::string desc = tc[token_index]; 
	
	 try 
	 {
		desc = desc.substr(2); 

		desc = desc.substr(0,desc.length()-2);

		desc = desc.substr(desc.find_first_not_of("\n"));

		desc = desc.substr(desc.find_first_not_of(' '));
	 }
	 catch(std::exception ex)
	 { desc = tc[token_index]; }

	 ptrPackage->setDescription(desc);
  }

///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  IsDescription::IsDescription(Package& pak) : ptrPackage(&pak)
  { }

 ///////////////////////////////////////////////////////////////
// Function to check if special keyword for module exist.
bool IsDescription::isModuleDescKeyword(const std::string& tok)
{
const static std::string keys[] = { "Module","Package","History","Maintain","Version","Release","Build","Operations"};

for(int i=0; i<8; ++i)
	if(tok.find(keys[i]) != -1)
		return true;
return false;
}

///////////////////////////////////////////////////////////////
// Rule to check if comment contains module description.
bool IsDescription::doTest(ITokCollection* pTc)
  {
        ITokCollection& tc = *pTc;

		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("/*") != -1 && isModuleDescKeyword(tc[i]) )
			{ 
				doActions(pTc);
				return true; 
			}
		}

		return false;
  }

 ///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  doIsDescription::doIsDescription(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Function to store the description in the Package.
  void doIsDescription::doAction(ITokCollection* pTc)
  {
	 ITokCollection& tc = *pTc;

	 int token_index = 0;

		for ( int i = 0 ; i < tc.length() ; i++ )
		{
			if ( tc[i].find("/*") == 0 && tc[i].find("Module") != -1 )
			{  	token_index = i; break; }
		}

	 std::string desc = tc[token_index]; 
	
	 try 
	 {
		desc = desc.substr(2); 

		desc = desc.substr(0,desc.length()-2);

		desc = desc.substr(desc.find_first_not_of("\n"));

		desc = desc.substr(desc.find_first_not_of(' '));
	 }
	 catch(std::exception ex)
	 { desc = tc[token_index]; }

	 ptrPackage->setDescriptionForce(desc);
  }


///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  IsDependency::IsDependency(Package& pak) : ptrPackage(&pak)
  { }

///////////////////////////////////////////////////////////////
// Rule to check if a dependency is detected.
bool IsDependency::doTest(ITokCollection* pTc)
  {
        ITokCollection& tc = *pTc;

		if ( tc.find("#") < tc.length()-2 && tc[tc.find("#")+1] == "include" && tc[tc.find("#")+2] != "<" )
		{ 
			doActions(pTc);
			return true; 
		}

		return false;
  }

 ///////////////////////////////////////////////////////////////
// Constructor to get pointer to pacakge object.
  doIsDependency::doIsDependency(Package& pak) : ptrPackage(&pak)
  { }

 ///////////////////////////////////////////////////////////////
// Function to convert the text to lower case.
  std::string doIsDependency::FormatToLower(std::string text)
  {
		std::string formatted = "";

		for ( int i = 0 ; i < (int)text.length() ; i++)
		{
			formatted.push_back(std::tolower(text[i]));
		}

		return formatted;
  }

///////////////////////////////////////////////////////////////
// Function to add dependency to the package.
  void doIsDependency::doAction(ITokCollection* pTc)
  {
	 ITokCollection& tc = *pTc;

	 std::string token = "";

	 if ( tc.find("include") < tc.length() - 1 )
	 { token = tc[tc.find("include") + 1]; }

	 if ( token[0] == '"' && token[token.length()-1] == '"' )
	 {
		 token = token.substr(1,token.length()-2);

		 token = FormatToLower(token);
		 
		 if ( token.find_last_of("\\") != -1 && token.find_last_of("\\") < token.length() )
		 {
			token = token.substr(token.find_last_of("\\") + 1 );
		 }

		 if ( token.length() > 2 && token.find(".h") == token.length() - 2 )
		 { token = token.substr(0,token.length()-2); }
		 else if ( token.length() > 4 && token.find(".cpp") == token.length() - 4 )
		 { token = token.substr(0,token.length()-4); }

		 if ( token != FormatToLower(ptrPackage->getPackageName()) )
		 { ptrPackage->addDependency(token); }
	 }
  }

//// Test Stub

#ifdef TEST_ACTIONSANDRULES

#include <iostream>
#include "ActionsAndRules.h"
#include "Tokenizer.h"
#include "SemiExpression.h"

int main(int argc, char* argv[])
{
   std::cout << "\n  Testing ActionsAndRules class\n "
            << std::string(30,'=') << std::endl;

  try
  {
    Package pk("Test");


	std::cout << "\n\n  Create Rule and Action \n\n";
    IsTitle title(pk);
	doIsTitle pDoIsTitle(pk);
    title.addAction(&pDoIsTitle);

	std::cout << "\n\n  Create Toker and Semi \n\n";

    Toker toker("../ActionsAndRules.h");
    SemiExp se(&toker);

	std::cout << "\n\n  Add rule to the parser\n\n";

    Parser parser(&se);
    parser.addRule(&title);
    
	std::cout << "\n\n  Parser Parsing File\n\n";
    while(se.get())
      parser.parse();
    
	std::cout << "\n\n  Parser finished Parsing File\n\n";

    std::cout << "\n\n";

	std::getchar();
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
}
#endif
