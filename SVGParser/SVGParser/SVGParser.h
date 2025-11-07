#pragma once
#include "SVGNode.h"
#include <string>
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace std;

class SVGParser {
private: 
	void parseNode(XMLElement* xmlNode, SVGNode* parentNode);
	void parseAttributes(XMLElement* xmlNode, SVGNode* svgNode);
public: 
	SVGNode* readXML(const string& filename);
};