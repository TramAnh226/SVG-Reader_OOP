#include "SVGEllipse.h"

SVGEllipse::SVGEllipse() 
    : SVGElement("ellipse", "", SVGStyle()),
    center(0, 0),
    rx(0.0f),
    ry(0.0f)
{}
SVGEllipse::SVGEllipse(Point center, float rx, float ry)
    : SVGElement("ellipse", "", SVGStyle()),
    center(center),
    rx(rx),
    ry(ry)    
{}
SVGEllipse::SVGEllipse(const SVGEllipse& other) 
    : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()),
    center(other.center),
    rx(other.rx),
    ry(other.ry)      
{}
SVGEllipse& SVGEllipse::operator=(const SVGEllipse& other) {
    if (this != &other) {
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
        this->center = other.center;
        this->rx = other.rx;    
        this->ry = other.ry;    
    }
    return (*this);
}
SVGEllipse::~SVGEllipse() {}
SVGElement* SVGEllipse::clone() const {
    return new SVGEllipse(*this);
}

Point SVGEllipse::getCenter() const {
    return center;
}
void SVGEllipse::setCenter(const Point& center) {
    this->center = center;
}
float SVGEllipse::getRadiusX() const {
    return rx;  
}
void SVGEllipse::setRadiusX(const float& rx) {
    this->rx = rx;  
}
float SVGEllipse::getRadiusY() const {
    return ry;  
}
void SVGEllipse::setRadiusY(const float& ry) {
    this->ry = ry;  
}

void SVGEllipse::parse(SVGParser& p) {
    p.parseEllipse(*this);
}
void SVGEllipse::render(SVGRenderer& r) {
    r.renderEllipse(*this);
}
// void SVGEllipse::transform(Matrix* m)