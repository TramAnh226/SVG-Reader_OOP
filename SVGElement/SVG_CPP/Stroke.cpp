#include "Stroke.h"

Stroke::Stroke() {
    this->strokeColor = CustomColor(0, 0, 0);
    this->strokeOpacity = 0.0f;
    this->strokeWidth = 0.0f;
}
Stroke::Stroke(CustomColor color, float opacity, float width) {
    this->strokeColor = color;
    this->strokeOpacity = opacity;
    this->strokeWidth = width;
}
Stroke::Stroke(const Stroke& other) {
    if (this != &other) {
        this->strokeColor = other.strokeColor;
        this->strokeOpacity = other.strokeOpacity;
        this->strokeWidth = other.strokeWidth;
    }
    return (*this);
}
Stroke* Stroke::operator=(const Stroke& other) {
    if (this != &other) {
        this->strokeColor = other.strokeColor;
        this->strokeOpacity = other.strokeOpacity;
        this->strokeWidth = other.strokeWidth;
    }
    return (*this);
}
Stroke::~Stroke() {}

// there is no getter/setter because all attributes are public

void Stroke::parse(SVGParser& p) {
    p.parseStroke();
}
void STroke::render(SVGRenderer& r) const {
    r.renderStroke();
}
// void transform(Matrix*);