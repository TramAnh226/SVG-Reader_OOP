#include "SVGStyle.h"

SVGStyle::SVGStyle() {
    this->fillColor = CustomColor(0, 0, 0);
    this->fillOpacity = 0.0f;
    this->stroke = Stroke();
}
SVGStyle::SVGStyle(CustomColor color, float opacity, Stroke stroke) {
    this->fillColor = color;
    this->fillOpacity = opacity;
    this->stroke = stroke;
} 
SVGStyle::SVGStyle(const SVGStyle& other) {
    this->fillColor = other.fillColor;
    this->fillOpacity = other.fillOpacity;
    this->stroke = other.stroke;
}
SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        fillColor = other.fillColor;
        fillOpacity = other.fillOpacity;
        stroke = other.stroke;
    }       
    return (*this);
}
SVGStyle::~SVGStyle() {}

CustomColor SVGStyle::getFillColor() const {
    return fillColor;       
}
void SVGStyle::setFillColor(const CustomColor& color) {
    fillColor = color;              
}
float SVGStyle::getFillOpacity() const {
    return fillOpacity;

}
void SVGStyle::setFillOpacity(const float& opacity) {
    fillOpacity = opacity;
}
Stroke SVGStyle::getStroke() const {
    return stroke;      
}
void SVGStyle::setStroke(const Stroke& s) {
    stroke = s;         
}

void SVGStyle::parse(const std::string& styleStr) {
    // p.parseStyle(*this, styleString);
    std::stringstream ss(styleStr);
	std::string item;
	while (getline(ss, item, ';')) {
		item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
		if (item.empty()) continue;

		size_t pos = item.find(':');
		if (pos == std::string::npos) continue;

		std::string key = item.substr(0, pos);
		std::string value = item.substr(pos + 1);

		if (key == "fill") this->setFillColor(CustomColor::fromStringToCustomColor(value));
		else if (key == "fill-opacity") this->setFillOpacity(stof(value));
		else if (key.find("stroke") != std::string::npos) {
            Stroke stroke = this->getStroke();
			// parseStroke(stroke, item);
            std::string s = styleStr;
            s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
            size_t pos = s.find(':');
            if (pos == std::string::npos) return;
            std::string key = s.substr(0, pos);
            std::string value = s.substr(pos + 1);
            if (key == "stroke") stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
            else if (key == "stroke-width") stroke.strokeWidth = stof(value);
            else if (key == "stroke-opacity") stroke.strokeOpacity = stof(value);

			this->setStroke(stroke);
		}
	}
}
// void SVGStyle::render(SVGRenderer& r) const {
//     r.renderStyle(*this);
// }  
// void transform(Matrix*);         