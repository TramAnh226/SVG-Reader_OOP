#include "SVGDocumentContext.h"
#include "SVGGradient.h"
#include <iostream>

SVGDocumentContext::SVGDocumentContext() {
}

void SVGDocumentContext::clear() {
    for (auto const& [id, gradientPtr] : gradientDefinitions) {
        delete gradientPtr;
    }
    gradientDefinitions.clear();
}

SVGDocumentContext::~SVGDocumentContext() {
    clear();
}

SVGDocumentContext::SVGDocumentContext(const SVGDocumentContext& other) {
    for (auto const& [id, gradientPtr] : other.gradientDefinitions) {
        this->gradientDefinitions[id] = gradientPtr->clone();
    }
}

SVGDocumentContext& SVGDocumentContext::operator=(const SVGDocumentContext& other) {
    if (this != &other) {
        SVGDocumentContext temp(other);

        std::swap(gradientDefinitions, temp.gradientDefinitions);
    }
    return *this;
}

void SVGDocumentContext::addGradient(SVGGradient* gradient) {
    if (!gradient) return;

    std::string id = gradient->getGradientID();
    if (gradientDefinitions.count(id)) {
        delete gradientDefinitions[id];
    }
    gradientDefinitions[id] = gradient;
}

const SVGGradient* SVGDocumentContext::getGradientById(const std::string& id) const {
    auto it = gradientDefinitions.find(id);
    if (it != gradientDefinitions.end()) {
        return it->second;
    }
    return nullptr;
}