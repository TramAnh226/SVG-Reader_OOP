#pragma once
#include "../../../SVGElement/SVG_H/Library.h"

using namespace tinyxml2;
using namespace std;

class SVGGroup;

class SVGParser {
private:
	XMLElement* currentNode = nullptr;
	void parseNode(XMLElement* xmlNode, SVGGroup* parentGroup);
	void parseAttributes(XMLElement* xmlNode, SVGElement* element);
	SVGGroup* readXML(const string& filename);
	
public:
	SVGParser() = default;
	~SVGParser() = default;

	void parseStyle(SVGStyle& style, const string& styleStr);
	void parseStroke(Stroke& stroke, const string& styleStr);
	void parseRectangle(SVGRectangle* rect, XMLElement* Node);
	void parseSquare(SVGSquare* sq, XMLElement* Node);
	void parseEllipse(SVGEllipse* el, XMLElement* Node);
	void parseCircle(SVGCircle* circle, XMLElement* Node);
	void parseLine(SVGLine* line, XMLElement* Node);
	void parsePolyshape(SVGPolyshapeBase* poly, XMLElement* Node);
	void parseText(SVGText* text, XMLElement* Node);
	void parseGroup(SVGGroup* group, XMLElement* Node);

};  



