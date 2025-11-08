#include "SVGPolygon.h"
SVGPolygon::SVGPolygon() {
    SVGPolyshapeBase("polygon", "", SVGStyle());
}   
SVGPolygon::SVGPolygon(const std::vector<Point>& points) {
    SVGPolyshapeBase("polygon", "", SVGStyle());
    this->points = points;
}
SVGPolygon::SVGPolygon(const SVGPolygon& other) {
    SVGPolyshapeBase::SVGPolyshapeBase(other);
    this->points = other.points;
}
SVGPolygon& SVGPolygon::operator=(const SVGPolygon& other) {
    if (this != &other) {
        SVGPolyshapeBase::operator=(other);
        this->points = other.points;
    }
    return *this;
}
SVGPolygon::~SVGPolygon() {

}

void SVGPolygon::parse(SVGParser& p) {
    p.parsePolygon(*this);
}
void SVGPolygon::render(SVGRenderer& r) {
    r.renderPolygon(*this);
}
// void SVGPolygon::transform(Matrix* m) {
