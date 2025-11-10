#pragma once
#include "SVGGroup.h"
#include <string>
#include "tinyxml2.h"
#include "SVGFactoryPattern.h"
#include "SVGRectangle.h"
#include "SVGCircle.h"
#include "SVGEllipse.h"
#include "SVGLine.h"
#include "SVGPolygon.h"
#include "SVGPolyline.h"
#include "SVGPolyshapeBase.h"
#include "SVGSquare.h"
#include "SVGText.h"

using namespace tinyxml2;
using namespace std;

class SVGParser {
private:
	void parseNode(XMLElement* xmlNode, SVGGroup* parentGroup);
	void parseAttributes(XMLElement* xmlNode, SVGElement* element);
	SVGGroup* readXML(const string& filename);
	void parseRectangle(SVGRectangle* rect, XMLElement* xmlNode);
	void parseSquare(SVGSquare* sq, XMLElement* xmlNode);
	void parseEllipse(SVGEllipse* el, XMLElement* xmlNode);
	void parseCircle(SVGCircle* circle, XMLElement* xmlNode);
	void parseLine(SVGLine* line, XMLElement* xmlNode);
	void parsePolyshape(SVGPolyshapeBase* poly, XMLElement* xmlNode);
	void parseText(SVGText* text, XMLElement* xmlNode);
public:
	SVGParser() = default;
	~SVGParser() = default;

	void parseStyle(SVGStyle& style, const string& styleStr);
	void parseStroke(Stroke& stroke, const string& styleStr);

};

