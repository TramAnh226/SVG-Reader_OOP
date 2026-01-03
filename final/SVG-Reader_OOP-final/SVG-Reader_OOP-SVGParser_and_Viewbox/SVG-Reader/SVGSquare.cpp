#include "SVGSquare.h"
#include "tinyxml2.h"
#include "SVGParser.h"
#include "SVGRenderer.h"

SVGSquare::SVGSquare() : SVGRectangle(CustomPoint(0, 0), 0, 0) {
    this->setTagName("square");
}
SVGSquare::SVGSquare(const CustomPoint& topLeft, float sideLength) : SVGRectangle(topLeft, sideLength, sideLength) {
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
SVGElement* SVGSquare::clone() const {
    return new SVGSquare(*this);
}


void SVGSquare::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    parser.parseSquare(this, xmlNode);
}
void SVGSquare::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderSquare(g, this, context);
}
