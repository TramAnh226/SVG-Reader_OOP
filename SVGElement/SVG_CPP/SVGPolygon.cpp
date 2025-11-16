#include "SVGPolygon.h"

SVGPolygon::SVGPolygon() : SVGPolyshapeBase() {
    this->setTagName("polygon");
}   
SVGPolygon::SVGPolygon(const std::vector<CustomPoint>& points) : SVGPolyshapeBase(points) {
    this->setTagName("polygon");
}
SVGPolygon::SVGPolygon(const SVGPolygon& other) : SVGPolyshapeBase(other.points) {
    this->setTagName("polygon");
    this->setId(other.getId());
    this->setSVGStyle(other.getSVGStyle());
}
SVGPolygon& SVGPolygon::operator=(const SVGPolygon& other) {
    if (this!= &other) {
        this->setPoints(other.getPoints());

        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
    }
    return (*this);
}
SVGPolygon::~SVGPolygon() {}
SVGElement* SVGPolygon::clone() const {
    return new SVGPolygon(*this);
}

void SVGPolyline::parse(SVGParser& p, XMLElement* node) {
    p.parsePolyshape(this, node);
}
void SVGPolyline::render(SVGRenderer& r) {
    r.renderPolyshapeBase(*this);
}
// void SVGPolygon::transform(Matrix* m)
