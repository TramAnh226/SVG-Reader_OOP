#include "SVGCircle.h"
SVGCircle::SVGCircle() {
    SVGEllipse("circle", Point(0, 0), 0, 0);  
}       
SVGCircle::SVGCircle(Point center, float radius) {
    SVGEllipse("circle", center, radius, radius);  
}
SVGCircle::SVGCircle(const SVGCircle& other) {
    SVGEllipse::SVGEllipse(other);   
}
SVGCircle::SVGCircle& operator=(const SVGCircle& other) {
    if (this != &other) {
        SVGEllipse::operator=(other);  
    }
    return *this;
}   
SVGCircle::~SVGCircle() {

}

Point SVGCircle::getCenter() {
    return this->center;
}
void SVGCircle::setCenter(const Point& center) {
    this->center = center;
}
float SVGCircle::getRadius() {
    return this->rx;
}
void SVGCircle::setRadius(const float& radius) {
    this->rx = radius;
    this->ry = radius;    
}

void SVGCircle::parse(SVGParser& p) override {
    p.parseCircle(*this);
}
void SVGCircle::render(SVGRenderer& r) override {
    r.renderCircle(*this);
}
// void SVGCircle::transform(Matrix* m) override {
