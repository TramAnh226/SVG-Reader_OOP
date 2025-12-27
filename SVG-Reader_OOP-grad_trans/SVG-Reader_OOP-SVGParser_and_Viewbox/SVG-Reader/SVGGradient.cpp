#include "tinyxml2.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include "SVGGradient.h"
#include "SVGStop.h"
#include "SVGDocumentContext.h"
#include "SVGParser.h"

//SVGGradient::SVGGradient()
//    : gradientUnits("objectBoundingBox"), spreadMethod("pad"), gradientTransform(nullptr) {
//}

SVGGradient::SVGGradient()
    : gradientUnits("objectBoundingBox"), spreadMethod("pad"), gradientTransform() {
}

//SVGGradient::SVGGradient(const SVGGradient& other) {
//    this->gradientID = other.gradientID;
//    this->stopArray = other.stopArray;
//    this->gradientUnits = other.gradientUnits;
//    this->spreadMethod = other.spreadMethod;
//    // SỬA: Clone ma trận thay vì gán con trỏ trực tiếp
//    this->gradientTransform = (other.gradientTransform) ? other.gradientTransform->Clone() : nullptr;
//}

SVGGradient::SVGGradient(const SVGGradient& other) {
    this->gradientID = other.gradientID;
    this->stopArray = other.stopArray;
    this->gradientUnits = other.gradientUnits;
    this->spreadMethod = other.spreadMethod;
    this->gradientTransform = other.gradientTransform;
}

//SVGGradient& SVGGradient::operator=(const SVGGradient& other) {
//    if (this != &other) {
//        delete this->gradientTransform; // Xóa cũ
//        this->gradientID = other.gradientID;
//        this->stopArray = other.stopArray;
//        this->gradientUnits = other.gradientUnits;
//        this->spreadMethod = other.spreadMethod;
//        this->gradientTransform = (other.gradientTransform) ? other.gradientTransform->Clone() : nullptr;
//    }
//    return *this;
//}
SVGGradient& SVGGradient::operator=(const SVGGradient& other) {
    if (this != &other) {
        this->gradientID = other.gradientID;
        this->stopArray = other.stopArray;
        this->gradientUnits = other.gradientUnits;
        this->spreadMethod = other.spreadMethod;
        this->gradientTransform = other.gradientTransform;
    }
    return *this;
}


const std::string& SVGGradient::getGradientID() const {
    return this->gradientID;
}

const std::vector<SVGStop>& SVGGradient::getStopArray() const {
    return this->stopArray;
}

const std::string& SVGGradient::getGradientUnits() const {
    return this->gradientUnits;
}

const std::string& SVGGradient::getSpreadMethod() const {
    return this->spreadMethod;
}

void SVGGradient::setGradientID(const std::string& id) {
    this->gradientID = id;
}

void SVGGradient::setStopArray(const std::vector<SVGStop>& stops) {
    this->stopArray = stops;
}

void SVGGradient::setGradientUnits(const std::string& units) {
    this->gradientUnits = units;
}

void SVGGradient::setSpreadMethod(const std::string& method) {
    this->spreadMethod = method;
}

void SVGGradient::addStop(const SVGStop& stop) {
    this->stopArray.push_back(stop);
}

void SVGGradient::clearStops() {
    this->stopArray.clear();
}

void SVGGradient::parse(SVGParser& p, tinyxml2::XMLElement* node) {
    p.parseGradient(this, node);
}

const std::string& SVGGradient::getHrefID() const {
    return hrefID;
}

void SVGGradient::setHrefID(const std::string& id) {
    hrefID = id;
}

//const Gdiplus::Matrix* SVGGradient::getGradientTransform() const {
//    return gradientTransform;
//}
//
//void SVGGradient::setGradientTransform(Gdiplus::Matrix* matrix) {
//    if (this->gradientTransform) delete this->gradientTransform;
//    this->gradientTransform = matrix;
//}

bool SVGGradient::isReferencing() const {
    return !hrefID.empty();
}

//const SVGGradient* SVGGradient::resolveReference(const SVGDocumentContext& context) const {
//    const SVGGradient* current = this;
//
//    // Lặp lại cho đến khi không còn tham chiếu
//    while (current && current->isReferencing()) {
//        std::string refID = current->getHrefID();
//        if (refID.size() > 0 && refID[0] == '#') {
//            refID = refID.substr(1);
//        }
//
//        const SVGGradient* next = context.getGradientById(refID);
//
//        if (next) {
//            current = next; // Tiếp tục chuỗi tham chiếu
//        }
//        else {
//            // Tham chiếu bị đứt gãy
//            std::cerr << "Warning: Gradient reference ID #" << refID << " not found.\n";
//            break;
//        }
//    }
//
//    // Trả về Gradient cuối cùng không tham chiếu
//    return current;
//}

void SVGGradient::resolveReference(const SVGDocumentContext& context) const {
    // Nếu đã có màu sắc thì không cần lấy từ cha nữa
    if (!this->stopArray.empty()) return;

    const SVGGradient* current = this;
    std::unordered_set<std::string> visited;

    while (current && current->isReferencing()) {
        std::string refID = current->getHrefID();
        if (!refID.empty() && refID[0] == '#') refID = refID.substr(1);

        // Chống vòng lặp tham chiếu vô tận
        if (visited.count(refID)) break;
        visited.insert(refID);

        const SVGGradient* next = context.getGradientById(refID);
        if (next) {
            // Nếu cha có stops, copy vào bản thân mình
            if (!next->getStopArray().empty()) {
                this->stopArray = next->getStopArray(); // stopArray là mutable nên gán được
                return;
            }
            current = next; // Tiếp tục tìm lên cấp cao hơn
        }
        else {
            break;
        }
    }
}