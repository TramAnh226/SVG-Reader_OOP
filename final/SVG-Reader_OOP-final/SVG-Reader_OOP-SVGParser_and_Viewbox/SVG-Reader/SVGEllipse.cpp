#include "SVGEllipse.h"
#include "tinyxml2.h" 
#include "SVGParser.h"
#include "SVGRenderer.h"

SVGEllipse::SVGEllipse()
    : SVGElement("ellipse", "", SVGStyle()),
    center(0, 0),
    rx(0.0f),
    ry(0.0f)
{
}
SVGEllipse::SVGEllipse(CustomPoint center, float rx, float ry)
    : SVGElement("ellipse", "", SVGStyle()),
    center(center),
    rx(rx),
    ry(ry)
{
}
SVGEllipse::SVGEllipse(const SVGEllipse& other) {
    this->setTagName(other.getTagName());
    this->setId(other.getId());
    this->setSVGStyle(other.getSVGStyle());
    this->center = other.center;
    this->rx = other.rx;
    this->ry = other.ry;
}
SVGEllipse& SVGEllipse::operator=(const SVGEllipse& other) {
    if (this != &other) {
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
        this->center = other.center;
        this->rx = other.rx;
        this->ry = other.ry;
    }
    return (*this);
}
SVGEllipse::~SVGEllipse() {}
SVGElement* SVGEllipse::clone() const {
    return new SVGEllipse(*this);
}

CustomPoint SVGEllipse::getCenter() const {
    return center;
}
void SVGEllipse::setCenter(const CustomPoint& center) {
    this->center = center;
}
float SVGEllipse::getRadiusX() const {
    return rx;
}
void SVGEllipse::setRadiusX(const float& rx) {
    this->rx = rx;
}
float SVGEllipse::getRadiusY() const {
    return ry;
}
void SVGEllipse::setRadiusY(const float& ry) {
    this->ry = ry;
}

void SVGEllipse::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    parser.parseEllipse(this, xmlNode);
}

void SVGEllipse::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderEllipse(g, this, context);
}
