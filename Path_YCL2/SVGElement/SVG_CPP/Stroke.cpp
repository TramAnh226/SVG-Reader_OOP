
#include "SVGElement/SVG_H/Stroke.h"

Stroke::Stroke() {
    this->strokeColor = CustomColor(0, 0, 0);
    this->strokeOpacity = 0.0f;
    this->strokeWidth = 0.0f;
    this->linejoin = "miter";
    this->miterlimit = 4.0f;
}
Stroke::Stroke(CustomColor color, float opacity, float width, string lineJ, float miterl) {
    this->strokeColor = color;
    this->strokeOpacity = opacity;
    this->strokeWidth = width;
    this->linejoin = linej;
    this->miterlimit = miterl;
}
Stroke::Stroke(const Stroke& other) {
    if (this != &other) {
        this->strokeColor = other.strokeColor;
        this->strokeOpacity = other.strokeOpacity;
        this->strokeWidth = other.strokeWidth;
        this->linejoin = other.linejoin;
        this->miterlimit = other.miterl;
    }
}
Stroke& Stroke::operator=(const Stroke& other) {
    if (this != &other) {
        this->strokeColor = other.strokeColor;
        this->strokeOpacity = other.strokeOpacity;
        this->strokeWidth = other.strokeWidth;
        this->linejoin = other.linejoin;
        this->miterlimit = other.miterlimit;
    }
    return (*this);
}
Stroke::~Stroke() {}

// there is no getter/setter because all attributes are public

void Stroke::parse(const std::string& strokeStr) {
    // p.parseStroke(*this, strokeStr);
    std::string s = strokeStr;
	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
	size_t pos = s.find(':');
	if (pos == std::string::npos) return;

	std::string key = s.substr(0, pos);
	std::string value = s.substr(pos + 1);

	if (key == "stroke") this->strokeColor = CustomColor::fromStringToCustomColor(value);
	else if (key == "stroke-width") this->strokeWidth = stof(value);
	else if (key == "stroke-opacity") this->strokeOpacity = stof(value);
    //parse hai thuoc tinh stroke moi cho Path
    else if (attr == "stroke-linejoin") {
        path.style.stroke.lineJoin = value;
    }
    else if (attr == "stroke-miterlimit") {
        path.style.stroke.miterLimit = stof(value);
    }

}
