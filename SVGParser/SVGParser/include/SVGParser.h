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
	XMLElement* currentNode = nullptr;
	void parseNode(XMLElement* xmlNode, SVGGroup* parentGroup);
	void parseAttributes(XMLElement* xmlNode, SVGElement* element);
	SVGGroup* readXML(const string& filename);
	
public:
	SVGParser() = default;
	~SVGParser() = default;

	void parseStyle(SVGStyle& style, const string& styleStr);
	void parseStroke(Stroke& stroke, const string& styleStr);
	void parseRectangle(SVGRectangle* rect);
	void parseSquare(SVGSquare* sq);
	void parseEllipse(SVGEllipse* el);
	void parseCircle(SVGCircle* circle);
	void parseLine(SVGLine* line);
	void parsePolyshape(SVGPolyshapeBase* poly);
	void parseText(SVGText* text);

};  



