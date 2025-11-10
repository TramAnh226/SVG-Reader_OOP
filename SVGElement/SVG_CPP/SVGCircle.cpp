#include "SVGCircle.h"

SVGCircle::SVGCircle() : SVGEllipse() {
    this->setTagName("circle");
}       
SVGCircle::SVGCircle(Point center, float radius) : SVGEllipse(center, radius, radius){
    this->setTagName("circle");
}
SVGCircle::SVGCircle(const SVGCircle& other) : SVGEllipse(other) {
    this->setTagName("circle");
}
SVGCircle& SVGCircle::operator=(const SVGCircle& other) {
    if (this!= &other) {
        this->setCenter(other.getCenter());
        this->setRadius(other.getRadius());
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
    }
    return (*this);
}   
SVGCircle::~SVGCircle() {

}

Point SVGCircle::getCenter() const {
    return this->center;
}
void SVGCircle::setCenter(const Point& center) {
    this->center = center;
}
float SVGCircle::getRadius() const {
    return this->rx;
}
void SVGCircle::setRadius(const float& radius) {
    this->rx = radius;
    this->ry = radius;    
}

void SVGCircle::parse(SVGParser& p) {
    p.parseCircle(*this);
}
void SVGCircle::render(SVGRenderer& r) {
    r.renderCircle(*this);
}
// void SVGCircle::transform(Matrix* m)
