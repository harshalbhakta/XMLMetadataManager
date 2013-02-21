#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
/////////////////////////////////////////////////////////////////////
//  XmlDocument.h -XML documents Wrapper (Fixed Bugs in ver 2.0 )  //
//  ver 3.0                                                        //
//                                                                 //
//  Language:      Visual C++ 2010                                 //
//  Platform:      Toshiba Satellite R25-S3503,Win7 Pro.           //
//  Application:   CSE687 Pr2, Sp11				                   //
//  Author:        Harshal Bhakta, Syracuse University			   //
//                 (714) 876-4273, harshal.c.bhakta@gmail.com      //
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//  XmlDocument.h - Wrapper for XML documents                      //
//  ver 2.0                                                        //
//                                                                 //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This package provides an XmlDocument class.  Its function is to build
  and internal parse tree representation of an XML document, and support
  loading, editing, and saving documents.

  Build Process:
  ==============
  Required files
    - XmlDocument.h, XmlDocument.cpp, XmlElementParts.h, XmlElementParts.cpp,
      XmlNodes.h, XmlNodes.cpp, MTree.h, MTree.cpp, Tokenizer.h, Tokenizer.cpp
  Build commands
    - cl /EHsc XmlDocument.cpp XmlElementParts.cpp XmlNodes.cpp Tokenizer.cpp

  Maintenance History:
  ====================
  ver 3.0 : 18 Mar 11 (Harshal-v3)
  - Fixed bug to ignore new-line formatting before tag 
    <?xml version="1.0" encoding="utf-8"?> 
	std::string XmlDocument::ToString(bool format); 
  - Fixed bug to ignore the additional "\n" & " " 
    appended for formatting.
	Modified : void XmlDocument::ProcessText(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
  - Fixed bug to handle Single Line Element.
	Added : void XmlDocument::ProcessSingleLineElement(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
	Modified : void XmlDocument::parseParts(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
  ver 2.0 : 25 Apr 10
  - Extracted IXmlDocument and IXmlNode interfaces, supported by changes
    to the XmlDocument implementation - predominately changing XmlNode to
    IXmlNode with a few code changes required by that.
  - Added tests in the test stub using the new interfaces.
  - Minor refactoring of the design
  ver 1.3 : 18 Mar 10
  - In response to bug reported by Karthick Soundararaj, added checks for
    empty XmlNodeStack before popping and checking top.  Bug caused unhandled
    structured exception for some cases of invalid XML.
  - also added attribute check in XmlProcInstrNode::addAttribue(...) in the
    XmlNode package
  ver 1.2 : 27 Feb 10
  - provided enQuote function to prevent tokenizer from rearranging XML
    text node's text
  - fixed bug in findelement
  ver 1.1 : 22 Feb 10
  - made Parse() a private function
  - reduced the number of find functions by using an enumeration:
    enum findType { tag, attribName, attribValue }; 
  ver 1.0 : 13 Feb 10
  - first release

*/
#include <stack>
#include <vector>
#include "XmlNodes.h"
#include "MTree.h"
#include "XmlElementParts.h"
#include "IXmlDocument.h"

namespace XmlProcessing
{
  class XmlDocument : public IXmlDocument
  {
  public:
    //enum findType { tag, attribName, attribValue };
    XmlDocument() : verbose_(false) {}
    bool& verbose() { return verbose_; }
    // compiler generated copy and assignment are correct for XmlDocument
    void loadFile(const std::string& filename);
    void loadString(const std::string& xmlStr);
    void clear();
    void addDocElement();
    void addDocDeclaration();
    std::vector<IXmlNode*> findElements(const std::string& arg, findType ft);
    IXmlNode* findElement(const std::string& arg, findType ft, IXmlNode* pNode=0);
    IXmlNode* insertElement(IXmlNode* pParent, IXmlNode* pNewChild);
    IXmlNode* deleteElement(IXmlNode* pParent, IXmlNode* pChild);
    std::string ToString(bool format=true);
    bool ToFile(const std::string& fileName);
  private:
    std::string load;
    std::string enQuoteXmlText(const std::string& XmlText);
    void parse(bool fromFile=true);
    void parseParts(std::stack<IXmlNode*>&, XmlParts&);
    void ProcessComment(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
    void ProcessText(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
    void ProcessProcInstr(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
    void ProcessElement(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
    void ProcessEndElement(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
	void ProcessSingleLineElement(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts);
    TMTree::MTree<IXmlNode> tree;
    bool format_;
    bool verbose_;
  };
  //----< save filename for toker attachment >-----------------------------

  inline void XmlDocument::loadFile(const std::string& filename)
  {
    load = filename;
    parse();
  }
  //----< save XML string for toker attachment >---------------------------

  inline void XmlDocument::loadString(const std::string& XmlText)
  {
    load = XmlText;
    parse(false);
  }
  //----< remove all elements of the XML parse tree >----------------------

  inline void XmlDocument::clear() { tree.clear(); }
}
#endif
