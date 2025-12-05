#include "SVGText.h"
#include "tinyxml2.h"
#include "SVGRenderer.h"

SVGText::SVGText()
    : SVGElement("text", "", SVGStyle()),
    font_size(12.0f),
    start(CustomPoint(0.0f, 0.0f)),
    content(L"")
{
}
SVGText::SVGText(float font_size, const CustomPoint& start, const std::wstring& content)
    : SVGElement("text", "", SVGStyle()),
    font_size(font_size),
    start(start),
    content(content)
{
}
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
//SVGText::~SVGText() {}
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

//void SVGText::parse(tinyxml2::XMLElement* node) {
//    SVGElement::parse(node);
//    // p.parseText(this, node);
//    float x = node->FloatAttribute("x");
//    float y = node->FloatAttribute("y");
//    const char* content = node->GetText();
//    this->setStart(CustomPoint(x, y));
//    const char* content_char = node->GetText();
//
//    // Chuyển đổi char* sang wstring để lưu trữ
//    if (content_char) {
//        std::string content_str(content_char);
//        this->setContent(std::wstring(content_str.begin(), content_str.end()));
//    }
//    else {
//        this->setContent(L"");
//    }
//}
void SVGText::parse(tinyxml2::XMLElement* node) {
    // 1. Gọi hàm cơ sở để parse ID và Style/Fill/Stroke
    SVGElement::parse(node);

    // 2. Lấy thuộc tính hình học (x, y)
    float x = node->FloatAttribute("x");
    float y = node->FloatAttribute("y");
    this->setStart(CustomPoint(x, y));

    // 3. Lấy thuộc tính font-size (Nếu tồn tại)
    // font-size thường là thuộc tính riêng biệt (standalone attribute)
    float fontSize = node->FloatAttribute("font-size");

    // Nếu font-size không được tìm thấy (trả về 0.0f) hoặc không hợp lệ, 
    // ta nên đặt giá trị mặc định hợp lý (ví dụ: 12.0f)
    if (fontSize <= 0.0f) {
        fontSize = 12.0f; // Giá trị mặc định an toàn
    }
    this->setFontSize(fontSize);

    // 4. Lấy nội dung text
    const char* content_char = node->GetText();

    // Chuyển đổi char* sang wstring để lưu trữ (cho GDI+)
    if (content_char) {
        std::string content_str(content_char);
        this->setContent(std::wstring(content_str.begin(), content_str.end()));
    }
    else {
        this->setContent(L"");
    }
}
void SVGText::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
    r.renderText(g, this);
}
// void SVGText::transform(Matrix* m) {
// Trong SVGText.cpp

// Đảm bảo đã include <iostream> và các headers cần thiết

void SVGText::printDebugAttributes(std::ostream& os) const {
    // 1. Gọi hàm lớp cơ sở để in ID và TagName
    SVGElement::printDebugAttributes(os);

    // 2. In thuộc tính RIÊNG của SVGText
    os << "  --- TEXT INFO ---" << std::endl;
    os << "  Font Size: " << this->getFontSize() << std::endl; // Lấy font_size
    os << "  Start Point: (" << this->getStart().x << ", " << this->getStart().y << ")" << std::endl;
    // ... (in các thuộc tính riêng khác nếu cần)
    os << "-------------------" << std::endl;

    // 3. Gọi hàm tiện ích để in Style/Stroke
    printStyleDebug(os);
}