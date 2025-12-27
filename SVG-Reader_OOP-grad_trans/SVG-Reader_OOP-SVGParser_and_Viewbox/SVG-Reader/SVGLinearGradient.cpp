//#include "tinyxml2.h"
//#include <iostream>
//#include "SVGLinearGradient.h"
//#include "SVGParser.h"
//
//// Giá trị mặc định theo chuẩn SVG: (0%, 0%) đến (100%, 0%) nếu gradientUnits="objectBoundingBox"
//SVGLinearGradient::SVGLinearGradient()
//    : SVGGradient(), x1(0.0f), y1(0.0f), x2(1.0f), y2(0.0f)
//{
//}
//
//SVGLinearGradient::SVGLinearGradient(const SVGLinearGradient& other)
//    : SVGGradient(other), 
//    x1(other.x1), y1(other.y1), x2(other.x2), y2(other.y2)
//{
//}
//
//SVGLinearGradient& SVGLinearGradient::operator=(const SVGLinearGradient& other) {
//    if (this != &other) {
//        SVGGradient::operator=(other); 
//        this->x1 = other.x1;
//        this->y1 = other.y1;
//        this->x2 = other.x2;
//        this->y2 = other.y2;
//    }
//    return *this;
//}
//
//SVGLinearGradient::SVGLinearGradient(float x1, float y1, float x2, float y2)
//    : SVGGradient(), x1(x1), y1(y1), x2(x2), y2(y2)
//{
//}
//
//SVGGradient* SVGLinearGradient::clone() const {
//    return new SVGLinearGradient(*this);
//}
//
//float SVGLinearGradient::getX1() const { return this->x1; }
//float SVGLinearGradient::getY1() const { return this->y1; }
//float SVGLinearGradient::getX2() const { return this->x2; }
//float SVGLinearGradient::getY2() const { return this->y2; }
//
//void SVGLinearGradient::setX1(float v) { this->x1 = v; }
//void SVGLinearGradient::setY1(float v) { this->y1 = v; }
//void SVGLinearGradient::setX2(float v) { this->x2 = v; }
//void SVGLinearGradient::setY2(float v) { this->y2 = v; }
//
//// --- Parsing ---
//
//void SVGLinearGradient::parse(SVGParser& p,tinyxml2::XMLElement* node) {
//    p.parseLinearGradient(this, node);
//}


#include "tinyxml2.h"
#include <iostream>
#include "SVGLinearGradient.h"
#include "SVGParser.h"

// Giá trị mặc định theo chuẩn SVG: (0%, 0%) đến (100%, 0%) nếu gradientUnits="objectBoundingBox"
SVGLinearGradient::SVGLinearGradient()
    : SVGGradient(), x1(0.0f), y1(0.0f), x2(1.0f), y2(0.0f)
{
}

SVGLinearGradient::SVGLinearGradient(const SVGLinearGradient& other)
    : SVGGradient(other),
    x1(other.x1), y1(other.y1), x2(other.x2), y2(other.y2)
{
}

SVGLinearGradient& SVGLinearGradient::operator=(const SVGLinearGradient& other) {
    if (this != &other) {
        SVGGradient::operator=(other);
        this->x1 = other.x1;
        this->y1 = other.y1;
        this->x2 = other.x2;
        this->y2 = other.y2;
    }
    return *this;
}

SVGLinearGradient::SVGLinearGradient(float x1, float y1, float x2, float y2)
    : SVGGradient(), x1(x1), y1(y1), x2(x2), y2(y2)
{
}

SVGGradient* SVGLinearGradient::clone() const {
    return new SVGLinearGradient(*this);
}

float SVGLinearGradient::getX1() const { return this->x1; }
float SVGLinearGradient::getY1() const { return this->y1; }
float SVGLinearGradient::getX2() const { return this->x2; }
float SVGLinearGradient::getY2() const { return this->y2; }

void SVGLinearGradient::setX1(float v) { this->x1 = v; }
void SVGLinearGradient::setY1(float v) { this->y1 = v; }
void SVGLinearGradient::setX2(float v) { this->x2 = v; }
void SVGLinearGradient::setY2(float v) { this->y2 = v; }

// --- Parsing ---

void SVGLinearGradient::parse(SVGParser& p, tinyxml2::XMLElement* node) {
    p.parseLinearGradient(this, node);
}