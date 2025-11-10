#include "CustomColor.h"

CustomColor::CustomColor() {
    r = 0;
    g = 0;      
    b = 0;
}
CustomColor::CustomColor(int red, int green, int blue) {
    r = red;
    g = green;      
    b = blue;
}
int CustomColor::clampCustomColorValue(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}
// eg: "rgb(255,0,128)"
CustomColor CustomColor::fromStringToCustomColor(const std::string& rgbString) {
    // find the position of the parentheses
    size_t start = rgbString.find('(');
    size_t end = rgbString.find(')');
    // return black for invalid format
    if (start == std::string::npos || end == std::string::npos || end <= start) {
        return CustomColor(0, 0, 0); 
    }
    
    std::string values = rgbString.substr(start + 1, end - start - 1);
    std::stringstream ss(values);
    std::string segment;
    int components[3] = {0, 0, 0};
    int i = 0;
    while (std::getline(ss, segment, ',') && i < 3) {
        try {
            components[i] = std::stoi(segment);
            i++;
        } catch (...) {
            components[i] = 0;
            i++;
        }
    }
    return CustomColor(
        clampCustomColorValue(components[0]),
        clampCustomColorValue(components[1]),
        clampCustomColorValue(components[2])
    );
}
std::string CustomColor::fromCustomColorToString() const {
    std::stringstream ss;
    ss << "rgb(" << r << "," << g << "," << b << ")";
    return ss.str();
}

