#include "SVGSquare.h"

SVGSquare::SVGSquare() : SVGRectangle(Point(0, 0), 0, 0) {
    this->setTagName("square");
}
SVGSquare::SVGSquare(const Point& topLeft, float sideLength) : SVGRectangle(topLeft, sideLength, sideLength) {
    this->setTagName("square");
}
SVGSquare::SVGSquare(const SVGSquare& other) : SVGRectangle(other) {
    this->setTagName("square");
}
SVGSquare& SVGSquare::operator=(const SVGSquare& other) {
    if (this != &other) {
        SVGRectangle::operator=(other);
    }
    return *this;
}
SVGSquare::~SVGSquare() {}

void SVGSquare::parse(SVGParser& p) {
    p.parseSquare(*this);
}
void SVGSquare::render(SVGRenderer& r) {
    r.renderSquare(*this);
}
// void SVGSquare::transform(Matrix* m)