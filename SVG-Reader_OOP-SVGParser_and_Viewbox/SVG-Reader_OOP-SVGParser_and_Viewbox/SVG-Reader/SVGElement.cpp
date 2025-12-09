#include "SVGElement.h"
#include "tinyxml2.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include <iostream>

SVGElement::SVGElement() {
    this->tag_name = "";
    this->id = "";
    this->style = SVGStyle();
}
SVGElement::SVGElement(const std::string& tag, const std::string& identifier, const SVGStyle& svg_style) {
    this->tag_name = tag;
    this->id = identifier;
    this->style = svg_style;
}

// it is unnescessary write copy constructor, copy assignment and destructor 
// because they are delete and default => force us: override at derived classes

// SVGElement::SVGElement(const SVGElement&);
// SVGElement& SVGElement::operator=(const SVGElement&); 
// SVGElement::~SVGElement();

std::string SVGElement::getTagName() const {
    return tag_name;
}
void SVGElement::setTagName(const std::string& tag) {
    this->tag_name = tag;
}
std::string SVGElement::getId() const {
    return id;
}
void SVGElement::setId(const std::string& identifier) {
    this->id = identifier;
}
//SVGStyle SVGElement::getSVGStyle() const {
//    return style;
//}
// Phiên bản không const (để sửa đổi)
SVGStyle& SVGElement::getSVGStyle() {
    return style;
}
// Phiên bản const (cho các hàm const)
const SVGStyle& SVGElement::getSVGStyle() const {
    return style;
}
void SVGElement::setSVGStyle(const SVGStyle& svg_style) {
    this->style = svg_style;
}

// polymorphism and separation of concerns
//void SVGElement::parse(tinyxml2::XMLElement* node) {
//    // Gọi hàm phân tích cú pháp thuộc tính chung (ID, STYLE, FILL...)
//    // Dạng 1: Nếu parseAttributes là hàm static trong SVGParser:
//    // SVGParser::parseAttributes(node, this); 
//
//    // Dạng 2: Nếu bạn dùng một hàm thành viên (non-static) của SVGParser
//    // SVGParser parser; 
//    // parser.parseAttributes(node, this); 
//
//    // Giả sử parseAttributes là static và bạn đã include SVGParser.h:
//    extern SVGParser g_parser; // Nếu g_parser là biến toàn cục
//    g_parser.parseAttributes(node, this);
//}
void SVGElement::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    parser.parseAttributes(xmlNode, this);
}

// ... (Các hàm khác)
// void SVGElement::render(SVGRenderer&, Gdiplus::Graphics&) const {}     
// virtual void SVGElement::transform(Matrix* m) = 0; // pure virtual function
// Trong SVGElement.cpp (thêm hàm này)

// Yêu cầu include <iostream> và <sstream>
// Thêm vào cuối SVGElement.cpp

void SVGElement::printDebugAttributes(std::ostream& os) const {
    os << "\n--- DEBUG ELEMENT ---" << std::endl;
    os << "Tag: " << this->tag_name << ", ID: " << this->id << std::endl;

    // Gọi hàm tiện ích để in Style/Stroke
    this->printStyleDebug(os);
}
void SVGElement::printStyleDebug(std::ostream& os) const {
    // Hàm này chỉ in các thuộc tính Style của chính đối tượng này
    os << "Fill: " << this->style.getFillColor().fromCustomColorToString() << std::endl;
    os << "Fill Opacity: " << this->style.getFillOpacity() << std::endl;

    Stroke* strokeObj = this->style.getStroke();

    if (strokeObj != nullptr) {
        os << "  --- STROKE INFO ---" << std::endl;
        os << "  Width: " << strokeObj->strokeWidth << std::endl;
        os << "  Opacity: " << strokeObj->strokeOpacity << std::endl;
        os << "  Color (RGB): " << strokeObj->strokeColor.fromCustomColorToString() << std::endl;
        os << "---------------------" << std::endl;
    }
    else {
        os << "Stroke: NONE" << std::endl;
    }
}

void SVGElement::resolve(const SVGDocumentContext& context) {
    // 1. Element này (ví dụ: Circle, Rect, Text) tự resolve Style của nó
    this->getSVGStyle().resolveGradient(context);

    // 2. Không cần gọi đệ quy vì element này không có con.
    // Lớp SVGGroup sẽ chịu trách nhiệm gọi đệ quy cho các con của nó.
}
