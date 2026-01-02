#include "SVGPolyline.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGPolyline::SVGPolyline() : SVGPolyshapeBase() {
    this->setTagName("polyline");
}
SVGPolyline::SVGPolyline(const std::vector<CustomPoint>& points) : SVGPolyshapeBase(points) {
    this->setTagName("polyline");
}
SVGPolyline::SVGPolyline(const SVGPolyline& other) : SVGPolyshapeBase(other.getPoints()) {
    this->setTagName("polyline");
}
SVGPolyline& SVGPolyline::operator=(const SVGPolyline& other) {
    if (this != &other) {
        this->setPoints(other.getPoints());

        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
    }
    return (*this);
}
SVGPolyline::~SVGPolyline() {}
SVGElement* SVGPolyline::clone() const {
    return new SVGPolyline(*this);
}
void SVGPolyline::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderPolyline(g, this, context);
}
