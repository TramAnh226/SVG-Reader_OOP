//#include "SVGFactoryPattern.h"
//#include "SVGCircle.h"
//#include "SVGRectangle.h"
//#include "SVGLine.h"
//#include "SVGPolygon.h"
//#include "SVGPolyline.h"
//#include "SVGSquare.h"
//#include "SVGText.h"
//#include "SVGPath.h"
//#include "SVGGroup.h"
//#include "SVGLinearGradient.h"
//#include "SVGRadialGradient.h"
//
//SVGFactoryPattern::SVGFactoryPattern() {
//    ElementID["circle"] = 1;
//    ElementID["ellipse"] = 2;
//    ElementID["line"] = 3;
//    ElementID["polygon"] = 4;
//    ElementID["polyline"] = 5;
//    ElementID["rect"] = 6;
//    ElementID["square"] = 7;
//    ElementID["text"] = 8;
//    ElementID["g"] = 9;
//    ElementID["path"] = 10;
//
//    ElementID["linearGradient"] = 11;
//    ElementID["radialGradient"] = 12;
//}
//
//
//SVGElement* SVGFactoryPattern::getElement(std::string tagname) {
//    auto it = ElementID.find(tagname);
//    if (it == ElementID.end()) {
//        throw std::out_of_range("Cannot find the type of object for tag: " + tagname);
//    }
//
//
//    int numid = it->second;
//
//
//    switch (numid) {
//    case 1:
//        return new SVGCircle();
//    case 2:
//        return new SVGEllipse();
//    case 3:
//        return new SVGLine();
//    case 4:
//        return new SVGPolygon();
//    case 5:
//        return new SVGPolyline();
//    case 6:
//        return new SVGRectangle();
//    case 7:
//        return new SVGSquare();
//    case 8:
//        return new SVGText();
//    case 9:
//        return new SVGGroup();
//        // Các tag không phải element hình học phải trả về nullptr
//    case 10:
//        return new SVGPath();
//    case 11: // linearGradient
//    case 12: // radialGradient
//        return nullptr;
//    default:
//        throw std::runtime_error("Found tag - " + tagname + " but mapped ID is invalid");
//    }
//}
//
//
//const std::unordered_map<std::string, int>& SVGFactoryPattern::getElementID() const {
//    return this->ElementID;
//}
//
//SVGGradient* SVGFactoryPattern::getGradientType(const std::string& tagName) {
//    auto it = ElementID.find(tagName);
//
//    if (it == ElementID.end()) {
//        return nullptr; // Không phải tag Gradient hợp lệ
//    }
//
//    int numid = it->second;
//
//    switch (numid) {
//    case 11:
//        return new SVGLinearGradient();
//    case 12:
//        return new SVGRadialGradient();
//    default:
//        return nullptr; // Các ID khác (hình học) không được xử lý ở đây
//    }
//}


#include "SVGFactoryPattern.h"
#include "SVGCircle.h"
#include "SVGRectangle.h"
#include "SVGLine.h"
#include "SVGPolygon.h"
#include "SVGPolyline.h"
#include "SVGSquare.h"
#include "SVGText.h"
#include "SVGPath.h"
#include "SVGGroup.h"
#include "SVGLinearGradient.h"
#include "SVGRadialGradient.h"

SVGFactoryPattern::SVGFactoryPattern() {
    ElementID["circle"] = 1;
    ElementID["ellipse"] = 2;
    ElementID["line"] = 3;
    ElementID["polygon"] = 4;
    ElementID["polyline"] = 5;
    ElementID["rect"] = 6;
    ElementID["square"] = 7;
    ElementID["text"] = 8;
    ElementID["g"] = 9;
    ElementID["path"] = 10;

    ElementID["linearGradient"] = 11;
    ElementID["radialGradient"] = 12;
}


SVGElement* SVGFactoryPattern::getElement(std::string tagname) {
    auto it = ElementID.find(tagname);
    if (it == ElementID.end()) {
        throw std::out_of_range("Cannot find the type of object for tag: " + tagname);
    }


    int numid = it->second;


    switch (numid) {
    case 1:
        return new SVGCircle();
    case 2:
        return new SVGEllipse();
    case 3:
        return new SVGLine();
    case 4:
        return new SVGPolygon();
    case 5:
        return new SVGPolyline();
    case 6:
        return new SVGRectangle();
    case 7:
        return new SVGSquare();
    case 8:
        return new SVGText();
    case 9:
        return new SVGGroup();
        // Các tag không phải element hình học phải trả về nullptr
    case 10:
        return new SVGPath();
    case 11: // linearGradient
    case 12: // radialGradient
        return nullptr;
    default:
        throw std::runtime_error("Found tag - " + tagname + " but mapped ID is invalid");
    }
}


const std::unordered_map<std::string, int>& SVGFactoryPattern::getElementID() const {
    return this->ElementID;
}

SVGGradient* SVGFactoryPattern::getGradientType(const std::string& tagName) {
    auto it = ElementID.find(tagName);

    if (it == ElementID.end()) {
        return nullptr; // Không phải tag Gradient hợp lệ
    }

    int numid = it->second;

    switch (numid) {
    case 11:
        return new SVGLinearGradient();
    case 12:
        return new SVGRadialGradient();
    default:
        return nullptr; // Các ID khác (hình học) không được xử lý ở đây
    }
}