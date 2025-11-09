#include "SVGPolyshapeBase.h"

SVGPolyshapeBase::SVGPolyshapeBase() 
    : SVGElement("polyshapebase", "", SVGStyle()),
    points(std::vector<Point>())
{}
SVGPolyshapeBase::SVGPolyshapeBase(const std::vector<Point>& points)
    : SVGElement("polyshapebase", "", SVGStyle()),
    points(points)
{}
SVGPolyshapeBase::SVGPolyshapeBase(const SVGPolyshapeBase& other) 
    : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()),
    points(other.points)
{}
SVGPolyshapeBase& SVGPolyshapeBase::operator=(const SVGPolyshapeBase& other) {
    if (this != &other) {
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
        points = other.points;
    }
    return *this;
}
SVGPolyshapeBase::~SVGPolyshapeBase() {

}

std::vector<Point> SVGPolyshapeBase::getPoints() const {
    return points;
}
void SVGPolyshapeBase::setPoints(const std::vector<Point>& points) {
    points = points;
}

void SVGPolyshapeBase::parse(SVGParser& p) {
    p.parsePolyshapeBase(*this);
}
void SVGPolyshapeBase::render(SVGRenderer& r) {
    r.renderPolyshapeBase(*this);
}
// void SVGPolyshapeBase::transform(Matrix* m)