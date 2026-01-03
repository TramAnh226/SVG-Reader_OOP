#include "Stroke.h"

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