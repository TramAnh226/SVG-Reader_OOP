#include "SVGPolyshapeBase.h"
SVGPolyshapeBase::SVGPolyshapeBase() {
    SVGElement("polyshapebase", "", SVGStyle());
    list = std::vector<Point>();
}
SVGPolyshapeBase::SVGPolyshapeBase(const std::vector<Point>& points) {
    SVGElement("polyshapebase", "", SVGStyle());
    list = points;
}
SVGPolyshapeBase::SVGPolyshapeBase(const SVGPolyshapeBase& other) {
    SVGElement::SVGElement(other);
    list = other.list;
}
SVGPolyshapeBase& SVGPolyshapeBase::operator=(const SVGPolyshapeBase& other) {
    if (this != &other) {
        SVGElement::operator=(other);
        list = other.list;
    }
    return *this;
}
SVGPolyshapeBase::~SVGPolyshapeBase() {

}

std::vector<Point> SVGPolyshapeBase::getList() const {
    return list;
}
void SVGPolyshapeBase::setList(const std::vector<Point>& points) {
    list = points;
}

void SVGPolyshapeBase::parse(SVGParser& p) {
    p.parsePolyshapeBase(*this);
}
void SVGPolyshapeBase::render(SVGRenderer& r) {
    r.renderPolyshapeBase(*this);
}
// void SVGPolyshapeBase::transform(Matrix* m) {