#include "Stroke.h"

//using namespace Gdiplus;

Stroke::Stroke()
    : strokeColor(CustomColor(0, 0, 0)),
    strokeOpacity(1.0f),
    strokeWidth(1.0f),
    strokeMiterLimit(4.0f),        
    strokeGradId(""),
    strokeGrad(nullptr)
{
}

Stroke::Stroke(CustomColor color, float opacity, float width, float miter)
    : strokeColor(color),
    strokeOpacity(opacity),
    strokeWidth(width),
    strokeMiterLimit(miter),         
    strokeGradId(""),
    strokeGrad(nullptr)
{
}

Stroke::Stroke(const Stroke& other)
    : strokeColor(other.strokeColor),
    strokeOpacity(other.strokeOpacity),
    strokeWidth(other.strokeWidth),
    strokeMiterLimit(other.strokeMiterLimit),     
    strokeGradId(other.strokeGradId),
    strokeGrad(nullptr) 
{
}

Stroke& Stroke::operator=(const Stroke& other) {
    if (this != &other) {
        this->strokeColor = other.strokeColor;
        this->strokeOpacity = other.strokeOpacity;
        this->strokeWidth = other.strokeWidth;
        this->strokeMiterLimit = other.strokeMiterLimit;    

        this->strokeGradId = other.strokeGradId;
        this->strokeGrad = nullptr;
    }
    return (*this);
}

Stroke::~Stroke() {}

float Stroke::getStrokeOpacity() const {
    return this->strokeOpacity;
}

const std::string& Stroke::getStrokeGradId() const { return this->strokeGradId; }
const SVGGradient* Stroke::getStrokeGrad() const { return this->strokeGrad; }

void Stroke::setStrokeGradId(const std::string& id) { this->strokeGradId = id; }
void Stroke::setStrokeGrad(SVGGradient* ptr) { this->strokeGrad = ptr; }

// there is no getter/setter because all attributes are public

Gdiplus::Color Stroke::getGdiColor() const {
    // 1. Chuyển đổi độ mờ (0.0 - 1.0) thành kênh Alpha (0 - 255)
    BYTE alpha = static_cast<BYTE>(strokeOpacity * 255.0f);

    // 2. Lấy CustomColor và chuyển đổi nó thành Gdiplus::Color
    Gdiplus::Color baseColor = strokeColor; // Sử dụng operator chuyển đổi của CustomColor

    // 3. Tạo Gdiplus::Color mới với alpha đã tính toán
    return Gdiplus::Color(alpha, baseColor.GetR(), baseColor.GetG(), baseColor.GetB());
}

void Stroke::parse(const std::string& strokeStr) {
    // p.parseStroke(*this, strokeStr);
    std::string s = strokeStr;
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    size_t pos = s.find(':');
    if (pos == std::string::npos) return;

    std::string key = s.substr(0, pos);
    std::string value = s.substr(pos + 1);

    if (key == "stroke") {
        if (value.size() >= 4 && value.substr(0, 4) == "url(") {
            // Gradient
            size_t start = value.find_first_of('#');
            size_t end = value.find_last_of(')');

            if (start != std::string::npos && end != std::string::npos && end > start) {
                this->strokeGradId = value.substr(start, end - start);
                this->strokeGrad = nullptr;
            }
            else {
                this->strokeGradId = "";
            }
        }
        else if (value == "none" || value == "transparent") {
            // None
            this->strokeGradId = "";
            this->strokeColor = CustomColor(0, 0, 0);
            this->strokeOpacity = 0.0f;
        }
        else {
            // Màu đơn
            this->strokeGradId = "";
            this->strokeColor = CustomColor::fromStringToCustomColor(value);
        }
    }
    else if (key == "stroke-width") this->strokeWidth = stof(value);
    else if (key == "stroke-opacity") this->strokeOpacity = stof(value);
}