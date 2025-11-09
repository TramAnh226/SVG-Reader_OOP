#include "SVGPolygon.h"

SVGPolygon::SVGPolygon() : SVGPolyshapeBase() {
    this->setTagName("polygon");
}   
SVGPolygon::SVGPolygon(const std::vector<Point>& points) : SVGPolyshapeBase(points) {
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
SVGPolygon::~SVGPolygon() {

}

void SVGPolygon::parse(SVGParser& p) {
    p.parsePolygon(*this);
}
void SVGPolygon::render(SVGRenderer& r) {
    r.renderPolygon(*this);
}
// void SVGPolygon::transform(Matrix* m)
