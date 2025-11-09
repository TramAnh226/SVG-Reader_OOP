#include "SVGLine.h"

SVGLine::SVGLine() 
    : SVGElement("line", "", SVGStyle()),
    start_point(0, 0),
    end_point(0, 0)      
{}
SVGLine::SVGLine(Point start, Point end) 
    : SVGElement("line", "", SVGStyle()),
    start_point(start),
    end_point(end)    
{}
SVGLine::SVGLine(const SVGLine& other)
    : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()),
    start_point(other.start_point),
    end_point(other.end_point)
{}
SVGLine& SVGLine::operator=(const SVGLine& other) {
    if (this != &other) {
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
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