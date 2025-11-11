#include "SVGParser.h"

#include <sstream>
#include <algorithm>
#include <iostream>

using namespace tinyxml2;
using namespace std;

SVGFactoryPattern factory;

SVGGroup* SVGParser::readXML(const string& filename) {
	XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
		cerr << "Cannot read SVG file: " << filename << endl;
		return nullptr;
	}

	XMLElement* rootXML = doc.RootElement();
	if (!rootXML) {
		cerr << " SVG file is invalid!" << endl;
		return nullptr;
	}
	
	  
	SVGGroup* rootGroup = new SVGGroup(); 
	rootGroup->setParent(nullptr);

	parseAttributes(rootXML, rootGroup);
	parseNode(rootXML, rootGroup);
    // them parse thuoc tinh group
    rootGroup->parse(rootXML);
	return rootGroup;
}



// sua thanh da hinh (bo if-else)
void SVGParser::parseNode(XMLElement* xmlNode, SVGGroup* parentGroup) {
    for (XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {

        string tagName = child->Name();
        SVGElement* element = factory.getElement(tagName);

        if (!element) {
            continue;
        }
        
        parseAttributes(child, element);

        currentNode = child;
        element->parse(*this);
        
        if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
            parseNode(child, group);
        }        
        parentGroup->addElement(element);
    }
}


void SVGParser::parseAttributes(XMLElement* xmlNode, SVGElement* element) {
    // parse thuoc tinh chung
    const XMLAttribute* attr = xmlNode->FirstAttribute();
    while (attr) {
        string name = attr->Name();
        string value = attr->Value();

        if (name == "id") {
            element->setId(value);
        }
        else if (name == "style") {
            SVGStyle style;
            parseStyle(style, value);
            element->setSVGStyle(style);
        }
        /*else if (name == "transform") {
            element->setTransform(value);
        }*/
        attr = attr->Next();
    }
}


void SVGParser::parseStyle(SVGStyle& style, const string& styleStr) {
	stringstream ss(styleStr);
	string item;
	while (getline(ss, item, ';')) {
		item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
		if (item.empty()) continue;

		size_t pos = item.find(':');
		if (pos == string::npos) continue;

		string key = item.substr(0, pos);
		string value = item.substr(pos + 1);

		if (key == "fill") style.setFillColor(CustomColor::fromStringToCustomColor(value));
		else if (key == "fill-opacity") style.setFillOpacity(stof(value));
		else if (key.find("stroke") != string::npos) {
            Stroke stroke = style.getStroke();
			parseStroke(stroke, item);
			style.setStroke(stroke);
		}
	}
}

void SVGParser::parseStroke(Stroke& stroke, const string& styleStr) {
	string s = styleStr;
	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
	size_t pos = s.find(':');
	if (pos == string::npos) return;

	string key = s.substr(0, pos);
	string value = s.substr(pos + 1);

	if (key == "stroke") stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
	else if (key == "stroke-width") stroke.strokeWidth = stof(value);
	else if (key == "stroke-opacity") stroke.strokeOpacity = stof(value);
}

void SVGParser::parseRectangle(SVGRectangle* rect) {
    if (!currentNode) return;
    float x = currentNode->FloatAttribute("x");
    float y = currentNode->FloatAttribute("y");
    float w = currentNode->FloatAttribute("width");
    float h = currentNode->FloatAttribute("height");
    rect->setTopLeftCorner(CustomPoint(x, y));
    rect->setWidth(w);
    rect->setHeight(h);
}

void SVGParser::parseSquare(SVGSquare* sq) {
    if (!currentNode) return;
    float x = currentNode->FloatAttribute("x");
    float y = currentNode->FloatAttribute("y");
    float size = currentNode->FloatAttribute("width"); // SVG square dùng width = height
    sq->setTopLeftCorner(CustomPoint(x, y));
    sq->setWidth(size);
    sq->setHeight(size);
}

void SVGParser::parseEllipse(SVGEllipse* el) {
    if (!currentNode) return;

    float cx = currentNode->FloatAttribute("cx");
    float cy = currentNode->FloatAttribute("cy");
    float rx = currentNode->FloatAttribute("rx");
    float ry = currentNode->FloatAttribute("ry");
    el->setCenter(CustomPoint(cx, cy));
    el->setRadiusX(rx);
    el->setRadiusY(ry);
}

void SVGParser::parseCircle(SVGCircle* circle) {
    if (!currentNode) return;
    float cx = currentNode->FloatAttribute("cx");
    float cy = currentNode->FloatAttribute("cy");
    float r = currentNode->FloatAttribute("r");
    circle->setCenter(CustomPoint(cx, cy));
    circle->setRadius(r);
}

void SVGParser::parseLine(SVGLine* line) {
    if (!currentNode) return;
    float x1 = currentNode->FloatAttribute("x1");
    float y1 = currentNode->FloatAttribute("y1");
    float x2 = currentNode->FloatAttribute("x2");
    float y2 = currentNode->FloatAttribute("y2");
    line->setStartPoint(CustomPoint(x1, y1));
    line->setEndPoint(CustomPoint(x2, y2));
}

void SVGParser::parsePolyshape(SVGPolyshapeBase* poly) {
    if (!currentNode) return;
    const char* pointsStr = currentNode->Attribute("points");
    if (!pointsStr) return;

    vector<CustomPoint> pts;
    stringstream ss(pointsStr);
    string token;
    while (getline(ss, token, ' ')) {
        if (token.empty()) continue;
        size_t comma = token.find(',');
        if (comma == string::npos) continue;
        float x = stof(token.substr(0, comma));
        float y = stof(token.substr(comma + 1));
        pts.emplace_back(x, y);
    }
    poly->setPoints(pts);
}

void SVGParser::parseText(SVGText* text) {
    if (!currentNode) return;
    float x = currentNode->FloatAttribute("x");
    float y = currentNode->FloatAttribute("y");
    const char* content = currentNode->GetText();
    text->setStart(CustomPoint(x, y));
    text->setContent(content ? content : "");
}

