#include "tinyxml2.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include "SVGGradient.h"
#include "SVGStop.h"
#include "SVGDocumentContext.h"
#include "SVGParser.h"

SVGGradient::SVGGradient()
    : gradientUnits("objectBoundingBox"), spreadMethod("pad") {
}

SVGGradient::SVGGradient(const SVGGradient& other)
    : gradientID(other.gradientID),
    stopArray(other.stopArray),
    gradientUnits(other.gradientUnits),
    spreadMethod(other.spreadMethod),
    gradientTransform(other.gradientTransform) {
}

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

const SVGTransform SVGGradient::getGradientTransform() const {
    return gradientTransform;
}
SVGTransform& SVGGradient::getGradientTransform() {
    return gradientTransform;
}
void SVGGradient::setGradientTransform(SVGTransform transform) {
    this->gradientTransform = transform;
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

bool SVGGradient::isReferencing() const {
    return !hrefID.empty();
}

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