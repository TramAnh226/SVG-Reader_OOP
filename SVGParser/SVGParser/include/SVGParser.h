#pragma once
#include "../../../SVGElement/SVG_H/Library.h"

using namespace tinyxml2;
using namespace std;

class SVGGroup;

class SVGParser {
private:
	tinyxml2::XMLElement* currentNode = nullptr;
	void parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup);
	void parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element);
	SVGGroup* readXML(const string& filename);
	
public:
	SVGParser() = default;
	~SVGParser() = default;

	void parseStyle(SVGStyle& style, const string& styleStr);
	void parseStroke(Stroke& stroke, const string& styleStr);
	void parseRectangle(SVGRectangle* rect, tinyxml2::XMLElement* Node);
	void parseSquare(SVGSquare* sq, tinyxml2::XMLElement* Node);
	void parseEllipse(SVGEllipse* el, tinyxml2::XMLElement* Node);
	void parseCircle(SVGCircle* circle, tinyxml2::XMLElement* Node);
	void parseLine(SVGLine* line, tinyxml2::XMLElement* Node);
	void parsePolyshape(SVGPolyshapeBase* poly, tinyxml2::XMLElement* Node);
	void parseText(SVGText* text, tinyxml2::XMLElement* Node);
	void parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node);

};  



