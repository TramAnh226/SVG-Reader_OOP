#include "SVGRectangle.h"

SVGRectangle::SVGRectangle() 
    : SVGElement("rect", "", SVGStyle()), 
    top_left_corner(CustomPoint(0, 0)), 
    width(0), 
    height(0) 
{}
SVGRectangle::SVGRectangle(const CustomPoint& topLeft, float w, float h) 
    : SVGElement("rect", "", SVGStyle()), 
    top_left_corner(topLeft), 
    width(w), 
    height(h) 
{}
SVGRectangle::SVGRectangle(const SVGRectangle& other) 
    : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()),
    top_left_corner(other.top_left_corner), 
    width(other.width), 
    height(other.height) 
{}
SVGRectangle& SVGRectangle::operator=(const SVGRectangle& other) {
    if (this != &other) {
        this->tag_name = other.getTagName();
        this->id = other.getId();
        this->style = other.getSVGStyle();

        this->top_left_corner = other.top_left_corner;
        this->width = other.width;
        this->height = other.height;
    }
    return *this;
}
SVGRectangle::~SVGRectangle() {}
SVGElement* SVGRectangle::clone() const {
    return new SVGRectangle(*this);
}

CustomPoint SVGRectangle::getTopLeftCorner() const {
    return top_left_corner;
}
void SVGRectangle::setTopLeftCorner(const CustomPoint& point) {
    top_left_corner = point;
}
float SVGRectangle::getWidth() const {
    return width;
}
void SVGRectangle::setWidth(float w) {
    width = w;
}
float SVGRectangle::getHeight() const {
    return height;
}
void SVGRectangle::setHeight(float h) {
    height = h;
}

void SVGRectangle::parse(SVGParser& p, XMLElement* node) {
    p.parseRectangle(this, node);
}
void SVGRectangle::render(SVGRenderer& r) const {
    r.renderRectangle(*this);
}
// void SVGRectangle::transform(Matrix* m) 
