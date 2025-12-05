#include "../include/SVGParser.h"

#include <sstream>
#include <algorithm>
#include <iostream>

// using namespace tinyxml2;
// using namespace std;
SVGFactoryPattern factory;
SVGGroup* SVGParser::readXML(const std::string& filename) {
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
		std::cerr << "Cannot read SVG file: " << filename << '\n';
		return nullptr;
	}

	tinyxml2::XMLElement* rootXML = doc.RootElement();
	if (!rootXML) {
		std::cerr << " SVG file is invalid!" << '\n';
		return nullptr;
	}
	
	  
	SVGGroup* rootGroup = new SVGGroup(); 
	rootGroup->setParent(nullptr);
    rootGroup->setTagName(rootXML->Name());

	parseAttributes(rootXML, rootGroup);
    rootGroup->parse(rootXML);
	parseNode(rootXML, rootGroup);
    // them parse thuoc tinh group
    
	return rootGroup;
}



// sua thanh da hinh (bo if-else)
void SVGParser::parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup) {
    for (tinyxml2::XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {

        std::string tagName = child->Name();
        SVGElement* element = factory.getElement(tagName);

        if (!element) {
            continue;
        }
        
        parseAttributes(child, element);
        element->parse(child);             
        parentGroup->addElement(element);

        if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
            parseNode(child, group);
        }
    }
}


void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
    // parse thuoc tinh chung
    const tinyxml2::XMLAttribute* attr = xmlNode->FirstAttribute();
    while (attr) {
        std::string name = attr->Name();
        std::string value = attr->Value();

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


// void SVGParser::parseStyle(SVGStyle& style, const string& styleStr) {
// 	stringstream ss(styleStr);
// 	string item;
// 	while (getline(ss, item, ';')) {
// 		item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
// 		if (item.empty()) continue;

// 		size_t pos = item.find(':');
// 		if (pos == string::npos) continue;

// 		string key = item.substr(0, pos);
// 		string value = item.substr(pos + 1);

// 		if (key == "fill") style.setFillColor(CustomColor::fromStringToCustomColor(value));
// 		else if (key == "fill-opacity") style.setFillOpacity(stof(value));
// 		else if (key.find("stroke") != string::npos) {
//             Stroke stroke = style.getStroke();
// 			parseStroke(stroke, item);
// 			style.setStroke(stroke);
// 		}
// 	}
// }

void SVGParser::parseStroke(Stroke& stroke, const std::string& styleStr) {
	std::string s = styleStr;
	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
	size_t pos = s.find(':');
	if (pos == std::string::npos) return;

	std::string key = s.substr(0, pos);
	std::string value = s.substr(pos + 1);

	if (key == "stroke") stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
	else if (key == "stroke-width") stroke.strokeWidth = stof(value);
	else if (key == "stroke-opacity") stroke.strokeOpacity = stof(value);
}

// void SVGParser::parseRectangle(SVGRectangle* rect, tinyxml2::XMLElement* Node) {
//     float x = Node->FloatAttribute("x");
//     float y = Node->FloatAttribute("y");
//     float w = Node->FloatAttribute("width");
//     float h = Node->FloatAttribute("height");
//     rect->setTopLeftCorner(CustomPoint(x, y));
//     rect->setWidth(w);
//     rect->setHeight(h);
// }

// void SVGParser::parseSquare(SVGSquare* sq, tinyxml2::XMLElement* Node) {
//     float x = Node->FloatAttribute("x");
//     float y = Node->FloatAttribute("y");
//     float size = Node->FloatAttribute("width"); // SVG square dùng width = height
//     sq->setTopLeftCorner(CustomPoint(x, y));
//     sq->setWidth(size);
//     sq->setHeight(size);
// }

// void SVGParser::parseEllipse(SVGEllipse* el, tinyxml2::XMLElement* Node) {
//     float cx = Node->FloatAttribute("cx");
//     float cy = Node->FloatAttribute("cy");
//     float rx = Node->FloatAttribute("rx");
//     float ry = Node->FloatAttribute("ry");
//     el->setCenter(CustomPoint(cx, cy));
//     el->setRadiusX(rx);
//     el->setRadiusY(ry);
// }

// void SVGParser::parseCircle(SVGCircle* circle, tinyxml2::XMLElement* Node) {
//     float cx = Node->FloatAttribute("cx");
//     float cy = Node->FloatAttribute("cy");
//     float r = Node->FloatAttribute("r");
//     circle->setCenter(CustomPoint(cx, cy));
//     circle->setRadius(r);
// }

// void SVGParser::parseLine(SVGLine* line, tinyxml2::XMLElement* Node) {

//     float x1 = Node->FloatAttribute("x1");
//     float y1 = Node->FloatAttribute("y1");
//     float x2 = Node->FloatAttribute("x2");
//     float y2 = Node->FloatAttribute("y2");
//     line->setStartPoint(CustomPoint(x1, y1));
//     line->setEndPoint(CustomPoint(x2, y2));
// }

// void SVGParser::parsePolyshape(SVGPolyshapeBase* poly, tinyxml2::XMLElement* Node) {

//     const char* pointsStr = Node->Attribute("points");
//     if (!pointsStr) return;

//     vector<CustomPoint> pts;
//     stringstream ss(pointsStr);
//     string token;
//     while (getline(ss, token, ' ')) {
//         if (token.empty()) continue;
//         size_t comma = token.find(',');
//         if (comma == string::npos) continue;
//         float x = stof(token.substr(0, comma));
//         float y = stof(token.substr(comma + 1));
//         pts.emplace_back(x, y);
//     }
//     poly->setPoints(pts);
// }

// void SVGParser::parseText(SVGText* text, tinyxml2::XMLElement* Node) {

//     float x = Node->FloatAttribute("x");
//     float y = Node->FloatAttribute("y");
//     const char* content = Node->GetText();
//     text->setStart(CustomPoint(x, y));
//     text->setContent(content ? content : "");
// }

// void SVGParser::parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node) {
    
//     /*const char* transformStr = Node->Attribute("transform");
//     if (transformStr) {
//         group->setTransform(std::string(transformStr));
//     }*/

    
//     if (std::string(Node->Name()) == "svg") {

//         float w = Node->FloatAttribute("width");
//         if (w <= 0.0f) { w = 300.0f; }
//         group->setWidth(w);


//         float h = Node->FloatAttribute("height");
//         if (h <= 0.0f) { h = 150.0f; }
//         group->setHeight(h);


//         const char* viewBoxStr = Node->Attribute("viewBox");
//         if (viewBoxStr) {
//             group->setViewBox(std::string(viewBoxStr));
//         }
//         else {
//             group->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
//         }
//     }
// }
