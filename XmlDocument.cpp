/////////////////////////////////////////////////////////////////////
//  XmlDocument.h -XML documents Wrapper (Fixed Bugs in ver 2.0 )  //
//  ver 3.0                                                        //
//                                                                 //
//  Language:      Microsoft Visual C++ 2010					   //
//  Platform:      Toshiba Satellite R25-S3503,Win7 Pro.           //
//  Application:   CSE687 Pr2, Sp11				                   //
//  Author:        Harshal Bhakta, Syracuse University			   //
//                 (714) 876-4273, harshal.c.bhakta@gmail.com      //
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//  XmlDocument.cpp - Wrapper for XML documents                    //
//  ver 2.0                                                        //
//                                                                 //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <sstream>
#include <fstream>
#include <stack>
#include <algorithm>
#include <cctype>
#include "XmlDocument.h"
#include "XmlElementParts.h"
using namespace XmlProcessing;
using namespace TMTree;

//----< add docElement (holds doc declar and root >------------------------

void XmlDocument::addDocElement()
{
  if(!tree.isEmpty())
    throw std::exception("can add doc element only to empty tree");
  XmlNodeFactory factory;
  IXmlNode* pDocElem = factory.CreateXmlNode(IXmlNode::Element,"docElement");
  tree.makeRoot(pDocElem);
}
//----< add document declaration processing instruction >------------------

void XmlDocument::addDocDeclaration()
{
  XmlProcInstrNode* pProcInstr = new XmlProcInstrNode("xml");
  pProcInstr->addAttribute("version","1.0");
  IXmlNode* pNode = this->findElement("docElement",IXmlDocument::tag);
  pNode->addChild(pProcInstr);
}
//----< Save document to file >--------------------------------------------

bool XmlDocument::ToFile(const std::string& fileName)
{
  std::ofstream out(fileName.c_str());
  if(!out.good())
    return false;
  std::string XMLstr = this->ToString();
  out << XMLstr;
  out.flush();
  return true;
}
//---< insert element into XmlDocument >-----------------------------------

IXmlNode* XmlDocument::insertElement(IXmlNode* pParent, IXmlNode* pNewChild)
{
  pParent->addChild(pNewChild);
  return pNewChild;
}
//---< delete element from XmlDocument >-----------------------------------

IXmlNode* XmlDocument::deleteElement(IXmlNode* pParent, IXmlNode* pChild)
{
  if(pParent->removeChild(pChild))
    return pParent;
  return 0;
}
//----< wrap XML text in quotes so tokenizer won't rearrange >-------------

std::string XmlDocument::enQuoteXmlText(const std::string& XmlText)
{
  std::string temp;
  size_t i=0;
  bool inTag = false;
  bool inText = false;

  while(XmlText[i] != '\0')
  {
    if(XmlText[i] == '<')
      inTag = true;
    if(XmlText[i] == '>')
      inTag = false;
    if(isalnum(XmlText[i]))
    {
      if(!inTag && !inText)
      {
        inText = true;
        temp.push_back('"');
      }
    }
    else if(inText && XmlText[i] == '<')
    {
      inText = false;
      temp.push_back('"');
    }
    temp.push_back(XmlText[i++]);
  }
  temp.push_back('\0');
  return temp;
}
//----< display the current document >-----------------------------------

std::string XmlDocument::ToString(bool format) 
{ 
  std::ostringstream out;
  tree.verbose() = verbose_;

  // tree applies Operation instance to every node during walk

  class ShowOperation : public Operation<IXmlNode>
  {
  public:
    ShowOperation(std::ostringstream& out, bool format, bool verbose=false) 
      : out_(out), format_(format), verbose_(verbose) {}
    bool operator()(IXmlNode* pNode)
    {
      // inherits level from base Operation
      if(level() == 0)  // don't display docElement
        return false;   // don't stop walk
	  // code to ignore new-line before initial tag. ( Harshal )
	  if(pNode->ToString().find("<?") == 0) // output the text without appending "/n"
	  { out_ << pNode->ToString(); return false; }
      if(format_)       // indent it
        out_ << "\n" << std::string(2*(level()-1),' ');
      out_ << pNode->ToString();
      if(verbose())
        std::cout << "\n --pNode->ToString() is: " << pNode->ToString();
      return false;     // don't stop walk
    }
    void end(IXmlNode* pNode) 
    { 
      if(pNode->ToEndString().size() > 0)
      {
        if(format_)     // indent it
          out_ << "\n" << std::string(2*(level()-1),' ');
        out_ << "</" << pNode->value() << ">"; 
      }
    }
  private:
    std::ostringstream& out_;
    bool format_;
    bool verbose_;
  };

  ShowOperation* pOper = new ShowOperation(out, format);
  Operation<IXmlNode>* oldOp = tree.setOperation(pOper);
  tree.walk();
  tree.setOperation(oldOp);
  delete pOper;
  return out.str();
}
//----< find elements >----------------------------------------------------

std::vector<IXmlNode*> XmlDocument::findElements(const std::string& arg, findType ft)
{
 std::vector<IXmlNode*> temp;

  class findElemsOperation : public Operation<IXmlNode>
  {
  public:
    findElemsOperation(std::vector<IXmlNode*>& out, const std::string& arg, XmlDocument::findType ft) 
      : out_(out), arg_(arg), ft_(ft) {}
    bool operator()(IXmlNode* pNode)
    {
      switch(ft_)
      {
      case tag:
        if(pNode->value() == arg_)
        {
          out_.push_back(pNode);
        }
        return false;     // don't stop walk
      case attribName:
        if(pNode->numAttributes() > 0)
        {
          std::vector<XmlNode::attributeItem> attribs = pNode->getAttributes();
          for(size_t i=0; i<attribs.size(); ++i)
          {
            if(attribs[i].first == arg_)
              out_.push_back(pNode);
          }
        }
        return false;     // don't stop walk
      case attribValue:
        if(pNode->numAttributes() > 0)
        {
          std::vector<XmlNode::attributeItem> attribs = pNode->getAttributes();
          for(size_t i=0; i<attribs.size(); ++i)
          {
            if(attribs[i].second == arg_)
              out_.push_back(pNode);
          }
        }
        return false;     // don't stop walk
      }
      return false;
    }
  private:
    std::vector<IXmlNode*>& out_;
    std::string arg_;
    XmlDocument::findType ft_;
  };

  findElemsOperation* pOper = new findElemsOperation(temp,arg,ft);
  Operation<IXmlNode>* oldOp = tree.setOperation(pOper);
  tree.walk();
  tree.setOperation(oldOp);
  delete pOper;
  return temp;
}
//----< find element >-----------------------------------------------------

IXmlNode* XmlDocument::findElement(const std::string& arg, findType ft, IXmlNode* pNode)
{
  IXmlNode* temp = 0;

  // Eventually this function should insure that once you've found a node
  // you can use its address in another call to this function to find the
  // next node, satisfying the same condition.

  class findElemOperation : public Operation<IXmlNode>
  {
  public:
    findElemOperation(IXmlNode*& out, const std::string& arg, XmlDocument::findType ft) 
      : out_(out), arg_(arg), ft_(ft) {}
    bool operator()(IXmlNode* pNode)
    {
      switch(ft_)
      {
      case tag:
        if(pNode->value() == arg_)
        {
          out_ = pNode;
          return true;
        }
        return false;     // don't stop walk
      case attribName:
        if(pNode->numAttributes() > 0)
        {
          std::vector<XmlNode::attributeItem> attribs = pNode->getAttributes();
          for(size_t i=0; i<attribs.size(); ++i)
          {
            if(attribs[i].first == arg_)
            {
              out_ = pNode;
              return true;
            }
          }
        }
        return false;     // don't stop walk
      case attribValue:
        if(pNode->numAttributes() > 0)
        {
          std::vector<XmlNode::attributeItem> attribs = pNode->getAttributes();
          for(size_t i=0; i<attribs.size(); ++i)
          {
            if(attribs[i].second == arg_)
            {
              out_ = pNode;
              return true;
            }
          }
        }
        return false;     // don't stop walk
      }
      return false;
    }
  private:
    IXmlNode*& out_;
    std::string arg_;
    XmlDocument::findType ft_;
  };

  findElemOperation* pOper = new findElemOperation(temp,arg,ft);
  Operation<IXmlNode>* oldOp = tree.setOperation(pOper);
  tree.walk();
  tree.setOperation(oldOp);
  delete pOper;
  return temp;
}
//----< parse comment >----------------------------------------------------

void XmlDocument::ProcessComment(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts)
{
  // concatenate parts tokens into comment string
  std::string text;
  int index = 3;
  while(parts[index] != "--")
  {
    text += parts[index] + " ";
    ++index;
  }
  // make comment node as child of stack top
  XmlCommentNode* pComment = new XmlCommentNode(text);
  XmlNodeStack.top()->addChild(pComment);

  // display for testing
  if(verbose())
  {
    std::cout << "    XmlCommentNode.ToString() = " << pComment->ToString() << "\n";
    std::cout << "    -- made child of stack top: ";
    std::cout << XmlNodeStack.top()->value() << "\n\n";
  }
}
//----< parse processing instruction >-------------------------------------

void XmlDocument::ProcessProcInstr(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts)
{
  XmlProcInstrNode* pProcInstr = new XmlProcInstrNode(parts[2]);

  // collect attributes from part and add to XmlNode
  int index = 3;
  while(index < parts.length()-3)
  {
    pProcInstr->addAttribute(parts[index],parts[index+2]);
    index += 3;
  }
  // add Proc Intr XmlNode as child of stack top
  XmlNodeStack.top()->addChild(pProcInstr);

  // display for testing
  if(verbose())
  {
    std::cout << "    XmlProcessingInstructionNode.ToString() = " << pProcInstr->ToString() << "\n";
    std::cout << "    -- made child of stack top: ";
    std::cout << XmlNodeStack.top()->value() << "\n\n";
  }
}
//----< process start element >--------------------------------------------

void XmlDocument::ProcessElement(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts)
{
  XmlElementNode* pElement = new XmlElementNode(parts[1]);

  // collect attributes from part and add to XmlNode
  int index = 2;
  while(index < parts.length()-3)
  {
    pElement->addAttribute(parts[index],parts[index+2]);
    index += 3;
  }
  XmlNodeStack.top()->addChild(pElement);

  // display for testing
  if(verbose())
  {
    std::cout << "    XmlElementNode.ToString() = " << pElement->ToString() << "\n";
    std::cout << "    made child of stack top: " << XmlNodeStack.top()->value() << "\n";
    std::cout << "    -- new element pushed on stack top\n\n";
  }

  XmlNodeStack.push(pElement);
}
//----< process end element >----------------------------------------------

void XmlDocument::ProcessEndElement(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts)
{
  // pop
  if(XmlNodeStack.empty())
    throw std::exception("Invalid XML");
  XmlNodeStack.pop();

  // display for testing
  if(verbose())
  {
    if(XmlNodeStack.empty())
      throw std::exception("Invalid XML");
    std::cout << "    -- popping stack, revealing: ";
    std::cout << XmlNodeStack.top()->value() << "\n\n";
  }
}

//----< process Single Line element > ( Harshal ) ----------------------------------------------

void XmlDocument::ProcessSingleLineElement(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts)
{
	ProcessElement(XmlNodeStack,parts);

	ProcessEndElement(XmlNodeStack,parts);
}

//----< process body text >------------------------------------------------

void XmlDocument::ProcessText(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts)
{
  // concatenate parts to form body text
  std::string text;
  for(int i=0; i<parts.length(); ++i)
  {
    text += parts[i];
    if(i != parts.length()-1)
      text += " ";
  }

  // ignore \n & " " used for formatting. ( Harshal )
  if ( text.find_last_of("\n") != -1 && text.find_last_of("\n") != 0 ) 
  { text = text.substr(0,text.find_last_of("\n")) + "\""; }

  // make Text node and make child of stack top
  XmlTextNode* pText = new XmlTextNode(text);
  XmlNodeStack.top()->addChild(pText);

  // display for testing
  if(verbose())
  {
    std::cout << "    XmlTextNode.ToString() = " << pText->ToString() << "\n";
    std::cout << "    -- make child of stack top: ";
    std::cout << XmlNodeStack.top()->value() << "\n\n";
  }
}
//----< Parse Helper >-----------------------------------------------------

void XmlDocument::parseParts(std::stack<IXmlNode*>& XmlNodeStack, XmlParts& parts)
{
  if(parts[0] == "<")  // not a text node
  {
    if(parts[1] == "!")  // comment
    {
      ProcessComment(XmlNodeStack,parts);
      return;
    }
    if(parts[1] == "?")  // Processing Instruction
    {
      ProcessProcInstr(XmlNodeStack,parts);
      return;
    }
    if(parts[1] == "/")  // end of element
    {
      ProcessEndElement(XmlNodeStack,parts);
      return;
    }
	if( parts.length() > 1 && parts[parts.length()-1] == ">" && parts[parts.length()-2] == "/") // Single Line element ( Harshal )
	{
	  ProcessSingleLineElement(XmlNodeStack,parts);
      return;
	}
    // must be XmlElement
    ProcessElement(XmlNodeStack,parts);
    return;
  }
  // must be a text node
  ProcessText(XmlNodeStack,parts);
}
//----< Parse XML parts >--------------------------------------------------

void XmlDocument::parse(bool fromFile)
{
  // set up Toker, XmlParts, XmlNode Stack

  Toker toker;

  // Enquoting XML text would most logically be done in Tokenizer.
  // For now, it's done here to avoid tinkering with that package.

  if(fromFile)  // Read file into a string and enQuote its text parts
  {
    std::ifstream in(load.c_str());
    if(!in.good())
      throw std::exception("\n\n can't open file\n\n");
    std::ostringstream out;
    std::filebuf* pFilebuf = in.rdbuf();
    out << pFilebuf;
    toker.attach(enQuoteXmlText(out.str()),false);
  }
  else  // just enQuote string and use it
    toker.attach(enQuoteXmlText(load),false);
  toker.setMode(Toker::xml);
  XmlParts parts(&toker);
  std::stack<IXmlNode*> XmlNodeStack;

  XmlNodeStack.push(new XmlElementNode("docElement"));

  if(verbose())
    std::cout << "  -- pushing docElement onto XmlNodeStack\n\n";

  // begin parsing

  while(parts.get())
  {
    if(verbose())
      std::cout << "  parts:" << parts.show().c_str() << std::endl;

    parseParts(XmlNodeStack, parts);
  }
  if(verbose())
    std::cout << "  -- entire parse tree can now be popped off XmlNodeStack with docElement";

  if(XmlNodeStack.empty())
    throw std::exception("Invalid XML");
  IXmlNode* pDocElement = XmlNodeStack.top();
  tree.makeRoot(pDocElement);
}

#ifdef TEST_XMLDOCUMENT

//----< XML Parsing Start >------------------------------------------------

int main(int argc, char* argv[])
{
 
  std::cout << "\n  Testing Creation of XmlDocument\n "
            << std::string(21,'=') << std::endl;

  XmlDocumentFactory docFactory;
  //XmlNodeFactory nodeFactory;

  //// Create Document.
  IXmlDocument* pDoc = docFactory.CreateXmlDocument();
  pDoc->loadFile("..\\AlternateTest.xml");

  pDoc->ToFile("..\\AlternateTest.xml");

  std::cout << "\n\n";
}

#endif

#ifdef TEST_CREATE_UPDATE_XMLDOCUMENT

int main(int argc, char* argv[])
{
 
	  std::cout << "\n  Testing Creation of XmlDocument\n "
            << std::string(21,'=') << std::endl;

  XmlDocumentFactory docFactory;
  XmlNodeFactory nodeFactory;

  //// Create Document.
  IXmlDocument* pDoc = docFactory.CreateXmlDocument();
  pDoc->addDocDeclaration();

  // Create Root Element.
  IXmlNode* pRoot = nodeFactory.CreateXmlNode(IXmlNode::Element, "Package");
  pRoot->addAttribute("name","Parser");

  // Create Tile Tag with Text Node.
  IXmlNode* pTitleNode = nodeFactory.CreateXmlNode(IXmlNode::Element, "Title");
  IXmlNode* pTextNode = nodeFactory.CreateXmlNode(IXmlNode::Text, "..\\Package to parse files using Toker & Semi.");
  pTitleNode->addChild(pTextNode);
  pRoot->addChild(pTitleNode);	

  // Create Desc Tag with Text Node.
  IXmlNode* pDescNode = nodeFactory.CreateXmlNode(IXmlNode::Element, "Description");
  IXmlNode* pDescTextNode = nodeFactory.CreateXmlNode(IXmlNode::Text, "Deatils go here.");
  pDescNode->addChild(pDescTextNode);
  pRoot->addChild(pDescNode);	

  // Create Header Tag with Text Node.
  IXmlNode* pHeaderNode = nodeFactory.CreateXmlNode(IXmlNode::Element, "Header");
  IXmlNode* pHeaderTextNode = nodeFactory.CreateXmlNode(IXmlNode::Text, "Parser.h");
  pHeaderNode->addChild(pHeaderTextNode);
  pRoot->addChild(pHeaderNode);	

  // Create Source Tag with Text Node.
  IXmlNode* pSourceNode = nodeFactory.CreateXmlNode(IXmlNode::Element, "Source");
  IXmlNode* pSourceTextNode = nodeFactory.CreateXmlNode(IXmlNode::Text, "Parser.cpp");
  pSourceNode->addChild(pSourceTextNode);
  pRoot->addChild(pSourceNode);	

  // Create Dependency Tag with Text Node.
  IXmlNode* pDepNode = nodeFactory.CreateXmlNode(IXmlNode::Element, "DepPackage");
  IXmlNode* pDepTextNode = nodeFactory.CreateXmlNode(IXmlNode::Text, "Semi");
  pDepNode->addChild(pDepTextNode);
  pRoot->addChild(pDepNode);	

  // Create Dependency Tag with Text Node.
  IXmlNode* pDepNode2 = nodeFactory.CreateXmlNode(IXmlNode::Element, "DepPackage");
  IXmlNode* pDepTextNode2 = nodeFactory.CreateXmlNode(IXmlNode::Text, "Toker");
  pDepNode2->addChild(pDepTextNode2);
  pRoot->addChild(pDepNode2);	

  IXmlNode* pDocElem = pDoc->findElement("docElement",IXmlDocument::tag);
  	  
  pDoc->insertElement(pDocElem,pRoot);	

  std::cout << std::endl;

  pDoc->ToFile("Test.xml");

  std::cout << "\n  Testing Adding Dependency to XmlDocument\n "
            << std::string(21,'=') << std::endl;

  IXmlDocument* pDocRead = docFactory.CreateXmlDocument();
  pDocRead->loadFile("Test.xml");

  IXmlNode* foundElement = pDocRead->findElement("Package",XmlDocument::tag);

  // Create Dependency Tag with Text Node.
  IXmlNode* pDepNode3 = nodeFactory.CreateXmlNode(IXmlNode::Element, "DepPackage");
  IXmlNode* pDepTextNode3 = nodeFactory.CreateXmlNode(IXmlNode::Text, "NewTokerTest");
  pDepNode3->addChild(pDepTextNode3);
  
  foundElement->addChild(pDepNode3);

  pDocRead->ToFile("Test.xml");

  std::cout << "\n  Testing Create New Filw With Empty Package Tag \n "
            << std::string(21,'=') << std::endl;

  //// Create Document.
  IXmlDocument* pDocCreatePart = docFactory.CreateXmlDocument();
  pDocCreatePart->addDocDeclaration();
 
  // Create Root Element.
  IXmlNode* pPackageRoot = nodeFactory.CreateXmlNode(IXmlNode::Element, "Package");

  IXmlNode* pDocElement = pDocCreatePart->findElement("docElement",IXmlDocument::tag);
  	  
  pDocCreatePart->insertElement(pDocElement,pPackageRoot);	

  pDocCreatePart->ToFile("TestPart.xml"); 

  std::cout << "\n  Testing Adding Individual tags. \n "
            << std::string(21,'=') << std::endl;

  pDocCreatePart->loadFile("TestPart.xml");

  pPackageRoot = pDocCreatePart->findElement("Package",IXmlDocument::tag);

  IXmlNode* pTitleNode2 = nodeFactory.CreateXmlNode(IXmlNode::Element, "Title");
  IXmlNode* pTitleTextNode2 = nodeFactory.CreateXmlNode(IXmlNode::Text, "Package to parse files.");
  pTitleNode2->addChild(pTitleTextNode2);

  pPackageRoot->addChild(pTitleNode2);

  std::cout<<"Check Value for Text : "<<pTitleNode2->getChildren()[0]->ToString();

  pDocCreatePart->ToFile("TestPart.xml");

}

#endif

#ifdef TEST_LOAD_SAVE

int main(int argc, char* argv[])
{
 
	  std::cout << "\n  Testing Creation of XmlDocument\n "
            << std::string(21,'=') << std::endl;

	  XmlDocumentFactory docFactory;

	  IXmlDocument* pDocRead = docFactory.CreateXmlDocument();
      pDocRead->loadFile("Parser.xml");

	  IXmlNode* pDocElement = pDocRead->findElement("Source",IXmlDocument::tag);
	  std::cout<<pDocElement->getChildren()[0]->ToString();

	  pDocRead->ToFile("ParserNew.xml");
}

#endif

