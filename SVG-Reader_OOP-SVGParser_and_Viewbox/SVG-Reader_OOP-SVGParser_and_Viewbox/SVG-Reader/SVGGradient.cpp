#include "SVGGradient.h"
#include "SVGStop.h"
#include "tinyxml2.h"
#include "SVGParser.h"
#include <algorithm>
#include <sstream>
#include <iostream>

SVGGradient::SVGGradient()
    : gradientUnits("objectBoundingBox"), spreadMethod("pad"){
}

SVGGradient::SVGGradient(const SVGGradient& other)
    : gradientID(other.gradientID),
    stopArray(other.stopArray), 
    gradientUnits(other.gradientUnits),
    spreadMethod(other.spreadMethod),
    transform(other.transform){
}

SVGGradient& SVGGradient::operator=(const SVGGradient& other) {
    if (this != &other) {
        this->gradientID = other.gradientID;
        this->stopArray = other.stopArray;
        this->gradientUnits = other.gradientUnits;
        this->spreadMethod = other.spreadMethod;
        this->transform = other.transform;
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

const std::string& SVGGradient::getTransform() const {
    return this->transform;
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

void SVGGradient::setTransform(const std::string& t) {
    this->transform = t;
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

bool SVGGradient::isReferencing() const {
    return !hrefID.empty();
}

// Trong SVGGradient.cpp
const SVGGradient* SVGGradient::resolveReference(const SVGDocumentContext& context) const {
    const SVGGradient* current = this;

    // Lặp lại cho đến khi không còn tham chiếu
    while (current && current->isReferencing()) {
        std::string refID = current->getHrefID();
        if (refID.size() > 0 && refID[0] == '#') {
            refID = refID.substr(1);
        }

        const SVGGradient* next = context.getGradientById(refID);

        if (next) {
            current = next; // Tiếp tục chuỗi tham chiếu
        }
        else {
            // Tham chiếu bị đứt gãy
            std::cerr << "Warning: Gradient reference ID #" << refID << " not found.\n";
            break;
        }
    }

    // Trả về Gradient cuối cùng không tham chiếu
    return current;
}