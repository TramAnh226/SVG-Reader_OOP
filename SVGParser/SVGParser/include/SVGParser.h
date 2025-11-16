#pragma once
#include "../../../SVGElement/SVG_H/Library.h"
#include "../../SVGGroup_Factory/SVG_H/SVGGroup.h"


using namespace tinyxml2;
using namespace std;



class SVGParser {
private:
	tinyxml2::XMLElement* currentNode = nullptr;
	void parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup);
	void parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element);
	
public:
	SVGParser() = default;
	~SVGParser() = default;

	SVGGroup* readXML(const string& filename);

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



