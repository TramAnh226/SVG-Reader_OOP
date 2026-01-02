#include "tinyxml2.h"
#include "Stroke.h"
#include "SVGElement.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include <iostream>

SVGElement::SVGElement() {
    this->tag_name = "";
    this->id = "";
    this->style = SVGStyle();

    this->transformMatrix = new Gdiplus::Matrix();
}
SVGElement::SVGElement(const std::string& tag, const std::string& identifier, const SVGStyle& svg_style) {
    this->tag_name = tag;
    this->id = identifier;
    this->style = svg_style;

    this->transformMatrix = new Gdiplus::Matrix();
}

// it is unnescessary write copy constructor, copy assignment and destructor 
// because they are delete and default => force us: override at derived classes

// 1. Copy Constructor
SVGElement::SVGElement(const SVGElement& other)
    : tag_name(other.tag_name), id(other.id), style(other.style)
{
    // Sử dụng Clone() để sao chép ma trận an toàn
    if (other.transformMatrix) {
        this->transformMatrix = other.transformMatrix->Clone();
    }
    else {
        this->transformMatrix = new Gdiplus::Matrix();
    }
}
// 2. Copy Assignment Operator
SVGElement& SVGElement::operator=(const SVGElement& other) {
    if (this != &other) {
        // Sao chép thuộc tính đơn giản và style
        this->tag_name = other.tag_name;
        this->id = other.id;
        this->style = other.style;

        // Xóa ma trận cũ
        delete this->transformMatrix;

        // Clone ma trận mới
        if (other.transformMatrix) {
            this->transformMatrix = other.transformMatrix->Clone();
        }
        else {
            this->transformMatrix = new Gdiplus::Matrix();
        }
    }
    return *this;
}

SVGElement::~SVGElement() {
    if (transformMatrix) {
        delete transformMatrix;
        transformMatrix = nullptr;
    }
}

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

void SVGElement::setTransformMatrix(Gdiplus::Matrix* newMatrix) {
    // Dọn dẹp đối tượng ma trận cũ (nếu nó tồn tại)
    if (transformMatrix != nullptr) {
        delete transformMatrix;
    }
    // Gán con trỏ ma trận mới
    transformMatrix = newMatrix;
}

void SVGElement::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    parser.parseAttributes(xmlNode, this);
}

void SVGElement::resolve(const SVGDocumentContext& context) {
    // 1. Element này (ví dụ: Circle, Rect, Text) tự resolve Style của nó
    this->getSVGStyle().resolveGradient(context);

    // 2. Không cần gọi đệ quy vì element này không có con.
    // Lớp SVGGroup sẽ chịu trách nhiệm gọi đệ quy cho các con của nó.
}
