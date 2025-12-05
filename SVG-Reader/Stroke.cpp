
#include "Stroke.h"
#include <windows.h>
#include <gdiplus.h>
#include <algorithm> 
#include <cmath>   
#include <stdexcept> 

using namespace Gdiplus;
using namespace std;

// --- Hàm tạo (Constructors) ---

Stroke::Stroke() {
    this->strokeColor = CustomColor(0, 0, 0);
    this->strokeOpacity = 1.0f;
    this->strokeWidth = 1.0f;
}

Stroke::Stroke(CustomColor color, float opacity, float width) {
    this->strokeColor = color;
    this->strokeOpacity = opacity;
    this->strokeWidth = width;
}

// 1. SỬA LỖI HÀM TẠO BẢN SAO: Dùng danh sách khởi tạo và loại bỏ if(this != &other)
Stroke::Stroke(const Stroke& other)
    : strokeColor(other.strokeColor),
    strokeOpacity(other.strokeOpacity),
    strokeWidth(other.strokeWidth)
{
    // Thân hàm có thể để trống
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



Gdiplus::Color Stroke::getGdiColor() const {
    // 1. Áp dụng giới hạn an toàn cho strokeOpacity [0.0f, 1.0f]
    float safeOpacity = min(1.0f, max(0.0f, this->strokeOpacity)); 

    // 2. Tính Alpha: Chuyển đổi giá trị đã giới hạn sang BYTE
    BYTE alpha = static_cast<BYTE>(std::round(safeOpacity * 255.0f));

    Gdiplus::Color baseColor = this->strokeColor;

    // 3. Tạo màu ARGB mới
    return Gdiplus::Color(alpha, baseColor.GetR(), baseColor.GetG(), baseColor.GetB());
}

void Stroke::parse(const std::string& strokeStr) {
    // Lưu ý: strokeStr (chính là item) hiện tại là chuỗi "key:value" VÀ KHÔNG CÓ KHOẢNG TRẮNG.

    std::string key, value;
    size_t pos = strokeStr.find(':'); // Tìm vị trí dấu hai chấm trong chuỗi "key:value"
    if (pos == std::string::npos) return;

    // Phân tách key và value
    key = strokeStr.substr(0, pos);
    value = strokeStr.substr(pos + 1);

    // Bỏ qua việc xóa khoảng trắng lại (vì đã làm trong SVGStyle::parse)
    // Nhưng nên giữ logic kiểm tra và giới hạn an toàn:

    if (key == "stroke") {
        this->strokeColor = CustomColor::fromStringToCustomColor(value);
    }
    else if (key == "stroke-width") {
        if (value.empty()) { this->strokeWidth = 1.0f; return; }
        try {
            float width = std::stof(value);
            // Sử dụng max(0.1f, width) để đảm bảo độ rộng tối thiểu
            this->strokeWidth = max(0.1f, width);
        }
        catch (const std::exception& e) { this->strokeWidth = 1.0f; }
    }
    else if (key == "stroke-opacity") {
        if (value.empty()) { this->strokeOpacity = 1.0f; return; }
        try {
            float opacity = std::stof(value);
            // Giới hạn trong khoảng [0.0f, 1.0f]
            this->strokeOpacity = min(1.0f, max(0.0f, opacity));
        }
        catch (const std::exception& e) { this->strokeOpacity = 1.0f; }
    }
}