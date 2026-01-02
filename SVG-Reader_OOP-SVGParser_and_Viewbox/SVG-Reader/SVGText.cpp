#include "SVGText.h"
#include "tinyxml2.h"
#include "SVGParser.h"
#include "SVGRenderer.h"

SVGText::SVGText()
    : SVGElement("text", "", SVGStyle()),
    font_size(12.0f),
    start(CustomPoint(0.0f, 0.0f)),
    content(L""),
    dx(0.0f), 
    dy(0.0f),
    font_family("Times New Roman")
{
}
SVGText::SVGText(float font_size, const CustomPoint& start, const std::wstring& content)
    : SVGElement("text", "", SVGStyle()),
    font_size(font_size),
    start(start),
    content(content),
    font_family("Times New Roman"),
    dx(0.0f), 
    dy(0.0f)
{
}
SVGText::SVGText(const SVGText& other) {
    this->tag_name = other.tag_name;
    this->id = other.id;
    this->style = other.style;

    this->font_size = other.font_size;
    this->start = other.start;
    this->content = other.content;
    this->dx = other.dx;
    this->dy = other.dy;
}
SVGText& SVGText::operator=(const SVGText& other) {
    if (this != &other) {
        this->tag_name = other.tag_name;
        this->id = other.id;
        this->style = other.style;

        this->font_size = other.font_size;
        this->start = other.start;
        this->content = other.content;

        this->dx = other.dx;
        this->dy = other.dy;
    }
    return *this;
}

// Getter cho font-family
std::string SVGText::getFontFamily() const {
    return font_family;
}

// Setter cho font-family
void SVGText::setFontFamily(const std::string& family) {
    font_family = family;
}

// Getters 
TextAnchor SVGText::getTextAnchor() const {
    return text_anchor;
}

FontStyles SVGText::getFontStyle() const {
    return font_style;
}

FontWeight SVGText::getFontWeight() const {
    return font_weight;
}


// Setters 
void SVGText::setTextAnchor(TextAnchor anchor) {
    this->text_anchor = anchor;
}
void SVGText::setFontStyle(FontStyles style) {
    this->font_style = style;
}
void SVGText::setFontWeight(FontWeight weight) {
    this->font_weight = weight;
}

SVGElement* SVGText::clone() const {
    return new SVGText(*this);
}

float SVGText::getFontSize() const {
    return font_size;
}
void SVGText::setFontSize(float font_size) {
    this->font_size = font_size;
}
CustomPoint SVGText::getStart() const {
    return start;
}
void SVGText::setStart(const CustomPoint& start) {
    this->start = start;
}
std::wstring SVGText::getContent() const {
    return content;
}
void SVGText::setContent(const std::wstring& content) {
    this->content = content;
}
float SVGText::getDx() const { return dx; }
void SVGText::setDx(float val) { dx = val; }
float SVGText::getDy() const { return dy; }
void SVGText::setDy(float val) { dy = val; }

void SVGText::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    // chạy trường hợp đặc biệt parseText trong parseAttributes ở SVGParser
}
void SVGText::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderText(g, this, context);
}
