#include "SVGText.h"

SVGText::SVGText() 
    : SVGElement("text", "", SVGStyle()), 
    font_size(12.0f), 
    start(Point(0.0f, 0.0f)), 
    content("") 
{}
SVGText::SVGText(float font_size, const Point& start, const std::string& content) 
    : SVGElement("text", "", SVGStyle()), 
    font_size(font_size), 
    start(start), 
    content(content) 
{}
SVGText::SVGText(const SVGText& other) {
    this->tag_name = other.tag_name;
    this->id = other.id;
    this->style = other.style;

    this->font_size = other.font_size;
    this->start = other.start;
    this->content = other.content;
}
SVGText& SVGText::operator=(const SVGText& other) {
    if (this != &other) {
        this->tag_name = other.tag_name;
        this->id = other.id;
        this->style = other.style;

        this->font_size = other.font_size;
        this->start = other.start;
        this->content = other.content;
    }
    return *this;
}
SVGText::~SVGText() {}
SVGElement* SVGText::clone() const {
    return new SVGText(*this);
}

float SVGText::getFontSize() const {
    return font_size;
}
void SVGText::setFontSize(float font_size) {
    this->font_size = font_size;
}
Point SVGText::getStart() const {
    return start;
}
void SVGText::setStart(const Point& start) {
    this->start = start;
}
std::string SVGText::getContent() const {
    return content;
}
void SVGText::setContent(const std::string& content) {
    this->content = content;
}

void SVGText::parse(SVGParser& p) {
    p.parseText(*this);
}
void SVGText::render(SVGRenderer& r) {
    r.renderText(*this);
}
// void SVGText::transform(Matrix* m) {