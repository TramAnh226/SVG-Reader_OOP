#include "SVGLine.h"
#include "tinyxml2.h" 
#include "SVGParser.h"
#include "SVGRenderer.h"

SVGLine::SVGLine()
    : SVGElement("line", "", SVGStyle()),
    start_point(0, 0),
    end_point(0, 0)
{
}
SVGLine::SVGLine(CustomPoint start, CustomPoint end)
    : SVGElement("line", "", SVGStyle()),
    start_point(start),
    end_point(end)
{
}
SVGLine::SVGLine(const SVGLine& other)
    : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()),
    start_point(other.start_point),
    end_point(other.end_point)
{
}
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
SVGElement* SVGLine::clone() const {
    return new SVGLine(*this);
}

CustomPoint SVGLine::getStartPoint() const {
    return start_point;
}
void SVGLine::setStartPoint(const CustomPoint& p) {
    start_point = p;
}
CustomPoint SVGLine::getEndPoint() const {
    return end_point;
}
void SVGLine::setEndPoint(const CustomPoint& p) {
    end_point = p;
}
void SVGLine::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    parser.parseLine(this, xmlNode);
}
void SVGLine::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderLine(g, this, context);
}
