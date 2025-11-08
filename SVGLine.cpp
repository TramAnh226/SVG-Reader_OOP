#include "SVGLine.h"
SVGLine::SVGLine() {
    SVGElement("line", "", SVGStyle());
    start_point = Point(0, 0);
    end_point = Point(0, 0);        
}
SVGLine::SVGLine(Point start, Point end) {
    SVGElement("line", "", SVGStyle());
    start_point = start;
    end_point = end;        
}
SVGLine::SVGLine(const SVGLine& other) {
    SVGElement::SVGElement(other);
    start_point = other.start_point;
    end_point = other.end_point;
}
SVGLine& SVGLine::operator=(const SVGLine& other) {
    if (this != &other) {
        SVGElement::operator=(other);
        start_point = other.start_point;
        end_point = other.end_point;
    }
    return *this;
}
SVGLine::~SVGLine() {}

Point SVGLine::getStartPoint() const {
    return start_point;
}
void SVGLine::setStartPoint(const Point& p) {
    start_point = p;
}
Point SVGLine::getEndPoint() const {
    return end_point;
}
void SVGLine::setEndPoint(const Point& p) {
    end_point = p;
}

void SVGLine::parse(SVGParser& p) {
    p.parseLine(*this);
}
void SVGLine::render(SVGRenderer& r) {
    r.renderLine(*this);
}
// void SVGLine::transform(Matrix* m) {