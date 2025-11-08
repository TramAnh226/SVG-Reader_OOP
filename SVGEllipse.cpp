#include "SVGEllipse.h"

SVGEllipse::SVGEllipse() {
    SVGElemment("ellipse", "", SVGStyle());
    center = Point(0, 0);
    rx = 0;
    ry = 0; 
}
SVGEllipse::SVGEllipse(Point center, float rx, float ry) {
    SVGElemment("ellipse", "", SVGStyle());
    this->center = center;
    this->rx = rx;      
    this->ry = ry;      
}
SVGEllipse::SVGEllipse(const SVGEllipse& other) {
    SVGElemment::SVGElemment(other);
    this->center = other.center;
    this->rx = other.rx;    
    this->ry = other.ry;    
}   
SVGEllipse& SVGEllipse::operator=(const SVGEllipse& other) {
    if (this != &other) {
        SVGElemment::operator=(other);
        this->center = other.center;
        this->rx = other.rx;    
        this->ry = other.ry;    
    }
    return *this;
}
SVGEllipse::~SVGEllipse() {

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
// void SVGEllipse::transform(Matrix* m) {