#include "SVGGroup.h"
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

// Setters
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
// --- Hàm Hợp nhất (Merge) thuộc tính kế thừa ---
void SVGGroup::mergeInheritedAttributes(const std::unordered_map<std::string, std::string>& parentMap) {
    // Chỉ chèn thuộc tính từ cha nếu Group này CHƯA có thuộc tính đó (ưu tiên Group hiện tại)
    for (const auto& pair : parentMap) {
        if (inheritedAttributes.find(pair.first) == inheritedAttributes.end()) {
            inheritedAttributes.insert(pair);
        }
    }
}

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
const std::unordered_map<std::string, std::string>& SVGGroup::getInheritedAttributes() const {
    return inheritedAttributes;
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
