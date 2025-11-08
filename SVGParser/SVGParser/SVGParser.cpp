#include "SVGParser.h"
#include <iostream>
using namespace std;
using namespace tinyxml2;

SVGNode* SVGParser::readXML(const string& filename) {
	XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
		cerr << "Cannot read svg file!\n";
		return nullptr;
	}

	XMLElement* root = doc.RootElement();
	if (!root) {
		cerr << "SVG file is invalid!\n";
		return nullptr;
	}

	SVGNode* rootNode = new SVGNode(root->Name());
	parseAttributes(root, rootNode);
	parseNode(root, rootNode);

	return rootNode;
}

void SVGParser::parseNode(XMLElement* xmlNode, SVGNode* parentNode) {
	for (XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {
		SVGNode* newNode = new SVGNode(child->Name());

		//doc thuoc tinh gan vao newNode
		parseAttributes(child, newNode);
		
		//them node con vao node cha
		parentNode->addChild(newNode);

		//de quy xu ly tiep cac node con
		parseNode(child, newNode);
	}
}

void SVGParser::parseAttributes(XMLElement* xmlNode, SVGNode* svgNode) {
	const XMLAttribute* attr = xmlNode->FirstAttribute();
	while (attr != nullptr) {
		svgNode->setAttribute(attr->Name(), attr->Value());
		attr = attr->Next();
	}
}
