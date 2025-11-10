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

	return rootGroup;
}




void SVGParser::parseNode(XMLElement* xmlNode, SVGGroup* parentGroup) {
    for (XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {

        string tagName = child->Name();
        SVGElement* element = factory.getElement(tagName);
        parseAttributes(child, element);

        if (auto rect = dynamic_cast<SVGRectangle*>(element)) {
            parseRectangle(rect, child);
        }
        else if (auto circ = dynamic_cast<SVGCircle*>(element)) {
            parseCircle(circ, child);
        }
        else if (auto ell = dynamic_cast<SVGEllipse*>(element)) {
            parseEllipse(ell, child);
        }
        else if (auto line = dynamic_cast<SVGLine*>(element)) {
            parseLine(line, child);
        }
        else if (auto poly = dynamic_cast<SVGPolyshapeBase*>(element)) {
            parsePolyshape(poly, child);
        }
        else if (auto txt = dynamic_cast<SVGText*>(element)) {
            parseText(txt, child);
        }
        /*else if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
            parseNode(child, group);
        }*/

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

		if (key == "fill") style.setFillColor(Color::fromString(value));
		else if (key == "fill-opacity") style.setFillOpacity(stof(value));
		else if (key.find("stroke") != string::npos) {
            Stroke stroke = style.getStroke();
			parseStroke(stroke, item);
			style.setStroke(style.getStroke());
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

	if (key == "stroke") stroke.strokeColor = Color::fromString(value);
	else if (key == "stroke-width") stroke.strokeWidth = stof(value);
	else if (key == "stroke-opacity") stroke.strokeOpacity = stof(value);
}

void SVGParser::parseRectangle(SVGRectangle* rect, XMLElement* xmlNode) {
    float x = xmlNode->FloatAttribute("x");
    float y = xmlNode->FloatAttribute("y");
    float w = xmlNode->FloatAttribute("width");
    float h = xmlNode->FloatAttribute("height");
    rect->setTopLeftCorner(Point(x, y));
    rect->setWidth(w);
    rect->setHeight(h);
}

void SVGParser::parseSquare(SVGSquare* sq, XMLElement* xmlNode) {
    float x = xmlNode->FloatAttribute("x");
    float y = xmlNode->FloatAttribute("y");
    float size = xmlNode->FloatAttribute("width"); // SVG square dùng width = height
    sq->setTopLeftCorner(Point(x, y));
    sq->setWidth(size);
    sq->setHeight(size);
}

void SVGParser::parseEllipse(SVGEllipse* el, XMLElement* xmlNode) {
    float cx = xmlNode->FloatAttribute("cx");
    float cy = xmlNode->FloatAttribute("cy");
    float rx = xmlNode->FloatAttribute("rx");
    float ry = xmlNode->FloatAttribute("ry");
    el->setCenter(Point(cx, cy));
    el->setRadiusX(rx);
    el->setRadiusY(ry);
}

void SVGParser::parseCircle(SVGCircle* circle, XMLElement* xmlNode) {
    float cx = xmlNode->FloatAttribute("cx");
    float cy = xmlNode->FloatAttribute("cy");
    float r = xmlNode->FloatAttribute("r");
    circle->setCenter(Point(cx, cy));
    circle->setRadius(r);
}

void SVGParser::parseLine(SVGLine* line, XMLElement* xmlNode) {
    float x1 = xmlNode->FloatAttribute("x1");
    float y1 = xmlNode->FloatAttribute("y1");
    float x2 = xmlNode->FloatAttribute("x2");
    float y2 = xmlNode->FloatAttribute("y2");
    line->setStartPoint(Point(x1, y1));
    line->setEndPoint(Point(x2, y2));
}

void SVGParser::parsePolyshape(SVGPolyshapeBase* poly, XMLElement* xmlNode) {
    const char* pointsStr = xmlNode->Attribute("points");
    if (!pointsStr) return;

    vector<Point> pts;
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

void SVGParser::parseText(SVGText* text, XMLElement* xmlNode) {
    float x = xmlNode->FloatAttribute("x");
    float y = xmlNode->FloatAttribute("y");
    const char* content = xmlNode->GetText();
    text->setStart(Point(x, y));
    text->setContent(content ? content : "");
}