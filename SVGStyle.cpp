#include "SVGStyle.h"

SVGStyle::SVGStyle() : fillColor(Color(0, 0, 0)), fillOpacity(1.0f), stroke(Stroke()) {}
SVGStyle::SVGStyle(Color fillColor, float fillOpacity, Stroke stroke)
    : fillColor(fillColor), fillOpacity(fillOpacity), stroke(stroke) {} 
SVGStyle::SVGStyle(const SVGStyle& other)
    : fillColor(other.fillColor), fillOpacity(other.fillOpacity), stroke(other.stroke) {}
SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        fillColor = other.fillColor;
        fillOpacity = other.fillOpacity;
        stroke = other.stroke;
    }       
    return *this;
}
SVGStyle::~SVGStyle() {}
Color SVGStyle::getFillColor() {
    return fillColor;       
}
void SVGStyle::setFillColor(const Color& color) {
    fillColor = color;              
}
float SVGStyle::getFillOpacity() {
    return fillOpacity;

}
void SVGStyle::setFillOpacity(const float& opacity) {
    fillOpacity = opacity;
}
Stroke SVGStyle::getStroke() {
    return stroke;      
}
void SVGStyle::setStroke(const Stroke& s) {
    stroke = s;         
}
void SVGStyle::parse(SVGParser& p) {
    p.parseStyle(*this);
}
void SVGStyle::render(SVGRenderer& r) {
    r.renderStyle(*this);
}           