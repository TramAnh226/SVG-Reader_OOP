//#include "SVGGroup.h"
////#include "SVGElement.h"
//#include "SVGFactoryPattern.h"
//#include <algorithm>
//#include "SVGParser.h"
//#include "SVGRenderer.h"
//
//
//SVGGroup::SVGGroup() :
//    SVGElement(),
//    ElementArray{},
//    parent(nullptr) {
//}
//
//
//SVGGroup::SVGGroup(const SVGGroup& other) : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()), parent(other.parent) {
//    for (auto element : other.ElementArray) {
//        SVGElement* newElement = element->clone();
//        if (newElement) {
//            this->ElementArray.push_back(newElement);
//        }
//    }
//}
//
//
//void swap(SVGGroup& first, SVGGroup& second) noexcept {
//    using std::swap;
//    swap(first.ElementArray, second.ElementArray);
//    swap(first.parent, second.parent);
//}
//
//
//SVGGroup& SVGGroup::operator= (SVGGroup other) noexcept {
//    swap(*this, other);
//    return *this;
//}
//
//
//SVGGroup::~SVGGroup() {
//    for (auto element : ElementArray)
//        delete element;
//    ElementArray.clear();
//}
//
//
//void SVGGroup::setParent(SVGGroup* parent) {
//    this->parent = parent;
//}
//
//
//void SVGGroup::setElementArray(const std::vector<SVGElement*>& SVGElementArray) {
//
//
//    for (auto elem : ElementArray)
//        delete elem;
//    ElementArray.clear();
//
//
//    for (auto element : SVGElementArray) {
//        SVGElement* newElement = element->clone();
//        if (newElement) {
//            this->ElementArray.push_back(newElement);
//        }
//    }
//}
//
//
//void SVGGroup::addElement(SVGElement* ele) {
//    this->ElementArray.push_back(ele);
//}
//
//
//SVGGroup* SVGGroup::getParent() {
//    return this->parent;
//}
//
//
//const std::vector<SVGElement*>& SVGGroup::getSVGElementArray() const {
//    return this->ElementArray;
//}
//
//SVGElement* SVGGroup::clone() const {
//    return new SVGGroup(*this);
//}
//
//
////void SVGGroup::parse(tinyxml2::XMLElement* node) {
////    SVGElement::parse(node);
////    // parser.parseGroup(this, node);
////    if (std::string(node->Name()) == "svg") {
////
////        float w = node->FloatAttribute("width");
////        if (w <= 0.0f) { w = 300.0f; }
////        this->setWidth(w);
////
////
////        float h = node->FloatAttribute("height");
////        if (h <= 0.0f) { h = 150.0f; }
////        this->setHeight(h);
////
////
////        const char* viewBoxStr = node->Attribute("viewBox");
////        if (viewBoxStr) {
////            this->setViewBox(std::string(viewBoxStr));
////        }
////        else {
////            this->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
////        }
////    }
////}
//void SVGGroup::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
//    SVGElement::parse(parser, xmlNode);
//    parser.parseGroup(this, xmlNode);
//}
//void SVGGroup::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
//    r.renderGroup(g, this, context);
//}
//// Trong SVGGroup.cpp
//// ...
//#include <iostream>
//
//// Trong SVGGroup.cpp
//void SVGGroup::printStyleDebug(std::ostream& os) const {
//    os << "ViewBox: " << this->viewBox << '\n';
//}
//void SVGGroup::printDebugAttributes(std::ostream& os, int idx) const {
//    // 1. In các thuộc tính chung (TagName, ID)
//    os << "ViewBox: " << this->viewBox << '\n';
//    SVGElement::printDebugAttributes(os, idx);
//
//    // 2. In các thuộc tính Style/Stroke của Group này
//    printStyleDebug(os);
//
//    os << "--- START CHILDREN DEBUG ---" << std::endl;
//    // 3. Duyệt đệ quy
//    for (const auto& element : ElementArray) {
//        if (element) {
//            // Gọi hàm đa hình của phần tử con
//            //if (element->getTagName() == "text") {
//            //    os << "Font_size: " << element.
//            //}
//            element->printDebugAttributes(os, idx);
//        }
//    }
//    os << "--- END GROUP: " << this->tag_name << " ---\n" << std::endl;
//}
//
//void SVGGroup::resolve(const SVGDocumentContext& context) {
//    // 1. Element này (Group) tự resolve Style của nó
//    this->getSVGStyle().resolveGradient(context);
//
//    // 2. Gọi đệ quy cho tất cả các Element con
//    for (SVGElement* child : ElementArray) {
//        if (child) {
//            child->resolve(context);
//        }
//    }
//}



#include "SVGGroup.h"
//#include "SVGElement.h"
#include "SVGFactoryPattern.h"
#include <algorithm>
#include "SVGParser.h"
#include "SVGRenderer.h"


SVGGroup::SVGGroup() :
    SVGElement(),
    ElementArray{},
    parent(nullptr) {
}

float SVGGroup::getWidth() {
    return this->width;
}
float SVGGroup::getHeight() {
    return this->height;
}

// --- Setters ---

void SVGGroup::setWidth(float w) {
    this->width = w;
}
void SVGGroup::setHeight(float h) {
    this->height = h;
}
SVGGroup::SVGGroup(const SVGGroup& other) : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()), parent(other.parent),
                                            width(other.width),
                                            height(other.height),
                                            viewBox(other.viewBox),
                                            inheritedAttributes(other.inheritedAttributes) {
    for (auto element : other.ElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) {
            this->ElementArray.push_back(newElement);
        }
    }
}


void swap(SVGGroup& first, SVGGroup& second) noexcept {
    using std::swap;
    swap(first.ElementArray, second.ElementArray);
    swap(first.parent, second.parent);
    swap(first.width, second.width);
    swap(first.height, second.height);
    swap(first.viewBox, second.viewBox);
    swap(first.inheritedAttributes, second.inheritedAttributes);
}


SVGGroup& SVGGroup::operator= (SVGGroup other) noexcept {
    swap(*this, other);
    return *this;
}


SVGGroup::~SVGGroup() {
    for (auto element : ElementArray)
        delete element;
    ElementArray.clear();
}


void SVGGroup::setParent(SVGGroup* parent) {
    this->parent = parent;
}


void SVGGroup::setElementArray(const std::vector<SVGElement*>& SVGElementArray) {


    for (auto elem : ElementArray)
        delete elem;
    ElementArray.clear();


    for (auto element : SVGElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) {
            this->ElementArray.push_back(newElement);
        }
    }
}


void SVGGroup::addElement(SVGElement* ele) {
    this->ElementArray.push_back(ele);
}


SVGGroup* SVGGroup::getParent() {
    return this->parent;
}


const std::vector<SVGElement*>& SVGGroup::getSVGElementArray() const {
    return this->ElementArray;
}

SVGElement* SVGGroup::clone() const {
    return new SVGGroup(*this);
}


void SVGGroup::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    SVGElement::parse(parser, xmlNode);
    parser.parseGroup(this, xmlNode);
}
void SVGGroup::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderGroup(g, this, context);
}

void SVGGroup::printDebugAttributes(std::ostream& os, int idx) const {
    // 1. In các thuộc tính chung (TagName, ID)
    SVGElement::printDebugAttributes(os, idx);

    // 2. In các thuộc tính Style/Stroke của Group này
    printStyleDebug(os);

    os << "--- START CHILDREN DEBUG ---" << std::endl;

    // 3. Duyệt đệ quy
    for (const auto& element : ElementArray) {
        if (element) {
            // Gọi hàm đa hình của phần tử con
            //if (element->getTagName() == "text") {
            //    os << "Font_size: " << element.
            //}
            element->printDebugAttributes(os, idx);
        }
    }
    os << "--- END GROUP: " << this->tag_name << " ---\n" << std::endl;
}

// --- Hàm Hợp nhất (Merge) thuộc tính kế thừa ---
void SVGGroup::mergeInheritedAttributes(const std::unordered_map<std::string, std::string>& parentMap) {
    // Chỉ chèn thuộc tính từ cha nếu Group này CHƯA có thuộc tính đó (ưu tiên Group hiện tại)
    for (const auto& pair : parentMap) {
        if (inheritedAttributes.find(pair.first) == inheritedAttributes.end()) {
            inheritedAttributes.insert(pair);
        }
    }
}

// Đảm bảo hàm parseViewBoxNumbers được khai báo/định nghĩa trong file này hoặc file .h
// Giả định hàm parseViewBoxNumbers được định nghĩa và có thể truy cập
//std::vector<float> parseViewBoxNumbers(const std::string& viewBoxValue) {
//    std::vector<float> values;
//    std::stringstream ss(viewBoxValue);
//    std::string segment;
//
//    // Các giá trị trong viewBox có thể được phân cách bởi khoảng trắng hoặc dấu phẩy.
//    // Dùng std::stringstream để tách các số.
//    while (ss >> segment) {
//        try {
//            // Loại bỏ dấu phẩy nếu có (trường hợp: "10, 20, 100, 100")
//            size_t commaPos = segment.find(',');
//            if (commaPos != std::string::npos) {
//                segment.erase(commaPos, 1);
//            }
//
//            // Chuyển đổi chuỗi thành float
//            float value = std::stof(segment);
//            values.push_back(value);
//        }
//        catch (...) {
//            // Bỏ qua nếu không phải là số hợp lệ
//        }
//    }
//
//    // SVG ViewBox luôn có 4 tham số
//    if (values.size() == 4) {
//        return values;
//    }
//    else {
//        return {}; // Trả về rỗng nếu không đủ 4 giá trị
//    }
//}
//void SVGGroup::setViewBox(const std::string& vb) {
//    this->viewBox = vb; // Lưu lại chuỗi gốc
//
//    // Gọi hàm phân tích để tách 4 số
//    std::vector<float> values = parseViewBoxNumbers(vb);
//
//    if (values.size() == 4) {
//        // [0] = min-x
//        // [1] = min-y
//        // [2] = width
//        // [3] = height
//
//        // 1. Gán tọa độ bắt đầu ViewBox
//        this->viewBoxX = values[0];
//        this->viewBoxY = values[1];
//
//        // 2. Gán kích thước ViewBox (chỉ cập nhật nếu giá trị hợp lệ)
//        if (values[2] > 0.0f) {
//            this->viewBoxW = values[2];
//        }
//        if (values[3] > 0.0f) {
//            this->viewBoxH = values[3];
//        }
//    }
//    // Nếu không có 4 giá trị, chúng ta giữ nguyên các giá trị mặc định (viewBoxX/Y = 0, width/height = 0).
//}

void SVGGroup::setViewBox(const std::string& vb) {
    this->viewBox = vb;

    // Xóa các dấu phẩy (,) và thay bằng khoảng trắng để dễ dàng phân tích
    std::string cleanVb = vb;
    std::replace(cleanVb.begin(), cleanVb.end(), ',', ' ');

    std::stringstream ss(cleanVb);
    std::vector<float> values;
    float val;

    // Đọc tối đa 4 giá trị float
    while (ss >> val) {
        values.push_back(val);
    }

    if (values.size() == 4) {
        this->viewBoxX = values[0];
        this->viewBoxY = values[1];
        this->viewBoxW = values[2];
        this->viewBoxH = values[3];
    }
}

void SVGGroup::resolve(const SVGDocumentContext& context) {
    // 1. Element này (Group) tự resolve Style của nó
    this->getSVGStyle().resolveGradient(context);

    // 2. Gọi đệ quy cho tất cả các Element con
    for (SVGElement* child : ElementArray) {
        if (child) {
            child->resolve(context);
        }
    }
}
