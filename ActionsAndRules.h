#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - Added Customized rules for Pass 1 & Pass 2.											  // 																					   
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
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 1.1                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable.

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
  parser.parse();                 // and parse it

  Build Process:
  ==============
  Required files
    -  ActionsAndRules.h, ActionsAndRules.cpp
	-  Parser.h, Parser.cpp, SemiExpression.h, SemiExpression.cpp, 
	-  tokenizer.h, tokenizer.cpp, Package.h , Package.cpp

  Build commands

    - cl /EHsc /DTEST_ACTIONSANDRULES ^
ActionsAndRules.cpp Parser.cpp SemiExpression.cpp Tokenizer.cpp Package.cpp

  Maintenance History:
  ====================
  ver 1.2 : 25 Mar 11
  - Add Custom Rules & Actions for XML Metadata detection.
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include "Package.h"
#include <queue>
#include <string>
#include "Parser.h"
#include "ITokCollection.h"

/////////////////////////////////////////////////////////////////////////////////////
////////////////////// Rules & Action for XML Metadata Manager //////////////////////
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// rule to detect Title for the Package.
class IsGeneralTitle : public IRule
{

Package* ptrPackage;

public:

  IsGeneralTitle(Package&);

  bool doTest(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// action for semi ending in }
class doIsGeneralTitle : public IAction
{

Package* ptrPackage;

public:

  doIsGeneralTitle(Package&);

  void doAction(ITokCollection*);

};


///////////////////////////////////////////////////////////////
// rule to detect Title for the Package.
class IsTitle : public IRule
{

Package* ptrPackage;

public:

  IsTitle(Package&);

  bool doTest(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// action for semi ending in }
class doIsTitle : public IAction
{

Package* ptrPackage;

public:

  doIsTitle(Package&);

  void doAction(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// rule to detect Description for the Package.
class IsGeneralDescription : public IRule
{

Package* ptrPackage;

public:

  IsGeneralDescription(Package&);

  bool doTest(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// action to be taken when description is detected.
class doIsGeneralDescription : public IAction
{

Package* ptrPackage;

public:

  doIsGeneralDescription(Package&);

  void doAction(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// rule to detect Description for the Package.
class IsDescription : public IRule
{

Package* ptrPackage;

public:

  IsDescription(Package&);

  bool isModuleDescKeyword(const std::string&);

  bool doTest(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// action to be taken when description is detected.
class doIsDescription : public IAction
{

Package* ptrPackage;

public:

  doIsDescription(Package&);

  void doAction(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// rule to detect Dependency for the Package.
class IsDependency : public IRule
{

Package* ptrPackage;

public:

  IsDependency(Package&);

  bool doTest(ITokCollection*);

};

///////////////////////////////////////////////////////////////
// action to be taken when dependecy is detected.
class doIsDependency : public IAction
{

Package* ptrPackage;

public:

  doIsDependency(Package&);

  std::string FormatToLower(std::string);

  void doAction(ITokCollection*);

};

#endif
