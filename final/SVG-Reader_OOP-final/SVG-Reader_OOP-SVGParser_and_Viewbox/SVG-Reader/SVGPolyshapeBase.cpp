#include "tinyxml2.h"
#include "SVGPolyshapeBase.h"
#include "SVGParser.h"

SVGPolyshapeBase::SVGPolyshapeBase()
    : SVGElement("polyshapebase", "", SVGStyle()),
    points(std::vector<CustomPoint>())
{
}
SVGPolyshapeBase::SVGPolyshapeBase(const std::vector<CustomPoint>& points)
    : SVGElement("polyshapebase", "", SVGStyle()),
    points(points)
{
}
SVGPolyshapeBase::SVGPolyshapeBase(const SVGPolyshapeBase& other)
    : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()),
    points(other.points)
{
}
SVGPolyshapeBase& SVGPolyshapeBase::operator=(const SVGPolyshapeBase& other) {
    if (this != &other) {
        this->setTagName(other.getTagName());
        this->setId(other.getId());
        this->setSVGStyle(other.getSVGStyle());
        points = other.points;
    }
    return *this;
}
SVGPolyshapeBase::~SVGPolyshapeBase() {}

std::vector<CustomPoint> SVGPolyshapeBase::getPoints() const {
    return points;
}
void SVGPolyshapeBase::setPoints(const std::vector<CustomPoint>& points) {
    this->points = points;
}

void SVGPolyshapeBase::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    parser.parsePolyshape(this, xmlNode);
}