#include "Color.h"

Color::Color() {
    r = 0;
    g = 0;      
    b = 0;
}
Color::Color(int red, int green, int blue) {
    r = red;
    g = green;      
    b = blue;
}
int Color::clampColorValue(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}
// eg: "rgb(255,0,128)"
Color Color::fromStringToColor(const std::string& rgbString) {
    // find the position of the parentheses
    size_t start = rgbString.find('(');
    size_t end = rgbString.find(')');
    // return black for invalid format
    if (start == std::string::npos || end == std::string::npos || end <= start) {
        return Color(0, 0, 0); 
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
    return Color(
        clampColorValue(components[0]),
        clampColorValue(components[1]),
        clampColorValue(components[2])
    );
}
std::string Color::fromColorToString() const {
    std::stringstream ss;
    ss << "rgb(" << r << "," << g << "," << b << ")";
    return ss.str();
}

