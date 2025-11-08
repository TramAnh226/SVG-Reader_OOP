#include "SVGElement.h"

SVGElement::SVGElement() : tag_name(""), id(""), style() {}     
SVGElement::SVGElement(const string& tag, const string& identifier, const SVGStyle& svg_style)
    : tag_name(tag), id(identifier), style(svg_style) {}
string SVGElement::getTagName() const {
    return tag_name;
}
void SVGElement::setTagName(const string& tag) {
    tag_name = tag;
}       
string SVGElement::getId() const {
    return id;
}
void SVGElement::setId(const string& identifier) {
    id = identifier;
}   
SVGStyle SVGElement::getSVGStyle() const {
    return style;
}
void SVGElement::setSVGStyle(const SVGStyle& svg_style) {
    style = svg_style;
}
// polymorphism and separation of concerns
// virtual void SVGElement::parse(SVGParser& parser) const = 0; // pure virtual function
// virtual void SVGElement::render(SVGRenderer& renderer) const = 0; // pure virtual function       
// subsequent development
// virtual void SVGElement::transform(Matrix* m) = 0; // pure virtual function

