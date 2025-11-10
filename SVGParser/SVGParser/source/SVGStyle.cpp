#include "SVGStyle.h"

SVGStyle::SVGStyle() {
    this->fillColor = Color(0, 0, 0);
    this->fillOpacity = 0.0f;
    this->stroke = Stroke();
}
SVGStyle::SVGStyle(Color color, float opacity, Stroke stroke) {
    this->fillColor = color;
    this->fillOpacity = opacity;
    this->stroke = stroke;
} 
SVGStyle::SVGStyle(const SVGStyle& other) {
    if (this != &other) {
        this->fillColor = other.fillColor;
        this->fillOpacity = other.fillOpacity;
        this->stroke = other.stroke;
    }
    
}
SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        fillColor = other.fillColor;
        fillOpacity = other.fillOpacity;
        stroke = other.stroke;
    }       
    return (*this);
}
SVGStyle::~SVGStyle() {}

Color SVGStyle::getFillColor() const {
    return fillColor;       
}
void SVGStyle::setFillColor(const Color& color) {
    fillColor = color;              
}
float SVGStyle::getFillOpacity() const {
    return fillOpacity;

}
void SVGStyle::setFillOpacity(const float& opacity) {
    fillOpacity = opacity;
}
Stroke SVGStyle::getStroke() const {
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
// void transform(Matrix*);         