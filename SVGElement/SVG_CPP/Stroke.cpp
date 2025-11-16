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
}
Stroke& Stroke::operator=(const Stroke& other) {
    if (this != &other) {
        this->strokeColor = other.strokeColor;
        this->strokeOpacity = other.strokeOpacity;
        this->strokeWidth = other.strokeWidth;
    }
    return (*this);
}
Stroke::~Stroke() {}

// there is no getter/setter because all attributes are public

<<<<<<< HEAD
void Stroke::parse(SVGParser& p, const std::string& strokeStr) {
    p.parseStroke(*this, strokeStr);
=======
void Stroke::parse(SVGParser& p) {
    p.parseStroke();
}
void STroke::render(SVGRenderer& r) const {
    r.renderStroke();
>>>>>>> 0e98a1fd97ea090b926142e9f550a5fe7b856f9e
}
// void STroke::render(SVGRenderer& r) {
//     r.renderStroke();
// }
// void transform(Matrix*);