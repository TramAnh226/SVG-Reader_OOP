#pragma once
#include "SVGElement.h"
#include "SVGRectangle.h"
#include "SVGSquare.h"
#include "SVGEllipse.h"
#include "SVGCircle.h"
#include "SVGLine.h"
#include "SVGPolyshapeBase.h"
#include "SVGPolygon.h"
#include "SVGPolyline.h"
#include "SVGText.h"
#include "SVGGroup.h"
#include "SVGDocumentContext.h"
#include "SVGFactoryPattern.h"
#include "CustomColor.h"
#include "SVGStop.h"
#include "SVGGradient.h"
#include "SVGLinearGradient.h"
#include "SVGRadialGradient.h"

// using namespace tinyxml2;
// using namespace std;
namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
	class XMLAttribute;
	class XMLNode;
	class XMLText;
	class XMLComment;
}
class SVGParser {
private:
	tinyxml2::XMLElement* currentNode = nullptr;
public:
	SVGParser() = default;
	~SVGParser() = default;

	SVGGroup* readXML(const std::string& filename, SVGFactoryPattern& factory, SVGDocumentContext& context);

	void parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory, SVGDocumentContext& context);

	void parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element);
	void parseStroke(Stroke& stroke, const std::string& styleStr);
	void parseStyle(SVGStyle& style, const std::string& styleStr);

	void parseDEFS(tinyxml2::XMLElement* xmlNode, SVGFactoryPattern& factory, SVGDocumentContext& context);
	void parseGradient(SVGGradient* grad, tinyxml2::XMLElement* node);
	void parseLinearGradient(SVGLinearGradient* grad,tinyxml2::XMLElement* xmlNode);
	void parseRadialGradient(SVGRadialGradient* grad,tinyxml2::XMLElement* xmlNode);
    
	void parseRectangle(SVGRectangle* rect, tinyxml2::XMLElement* Node);
	void parseSquare(SVGSquare* sq, tinyxml2::XMLElement* Node);
	void parseEllipse(SVGEllipse* el, tinyxml2::XMLElement* Node);
	void parseCircle(SVGCircle* circle, tinyxml2::XMLElement* Node);
	void parseLine(SVGLine* line, tinyxml2::XMLElement* Node);
	void parsePolyshape(SVGPolyshapeBase* poly, tinyxml2::XMLElement* Node);
	void parseText(SVGText* text, tinyxml2::XMLElement* Node);
	void parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node);

};



