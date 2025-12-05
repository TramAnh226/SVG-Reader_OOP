
#include "Stroke.h"
#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
// Trong Stroke.cpp
Stroke::Stroke() {
    this->strokeColor = CustomColor(0, 0, 0); // Giữ nguyên màu mặc định (đen)
    this->strokeOpacity = 1.0f;              // Mặc định là mờ đục
    this->strokeWidth = 1.0f;                // <<< ĐẶT MẶC ĐỊNH LÀ 1.0f
}
Stroke::Stroke(CustomColor color, float opacity, float width) {
    this->strokeColor = color;
    this->strokeOpacity = opacity;
    this->strokeWidth = width;
}
//Stroke::Stroke(const Stroke& other) {
//    this->strokeColor = other.strokeColor;
//    this->strokeOpacity = other.strokeOpacity;
//    this->strokeWidth = other.strokeWidth;
//}
// Trong Stroke.cpp

// SỬA LỖI HÀM TẠO BẢN SAO: SỬ DỤNG DANH SÁCH KHỞI TẠO
Stroke::Stroke(const Stroke& other)
    : strokeColor(other.strokeColor),  // Khởi tạo CustomColor
    strokeOpacity(other.strokeOpacity), // Khởi tạo float
    strokeWidth(other.strokeWidth)      // Khởi tạo float
{
    // Thân hàm này bây giờ có thể để trống
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

    if (key == "stroke") this->strokeColor = CustomColor::fromStringToCustomColor(value);
    else if (key == "stroke-width") this->strokeWidth = stof(value);
    else if (key == "stroke-opacity") this->strokeOpacity = stof(value);
}
// void STroke::render(SVGRenderer& r) const {
//     r.renderStroke();
// }
// void transform(Matrix*);

/*
#include "Stroke.h"
#include <windows.h>
#include <gdiplus.h>
#include <algorithm> // Cần cho std::remove_if
#include <cmath>     // Cần cho std::round
#include <stdexcept> // Cần cho xử lý exception

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

// --- Định nghĩa Hàm GdiColor ---

#include <cmath> // Cần cho std::round

// ...

Gdiplus::Color Stroke::getGdiColor() const {
    // 1. Chuyển đổi Opacity (0.0 - 1.0) sang Alpha (0 - 255)
    // Sử dụng std::round để đảm bảo độ chính xác khi chuyển đổi sang BYTE
    BYTE alpha = static_cast<BYTE>(std::round(this->strokeOpacity * 255.0f));

    // 2. Lấy màu RGB cơ bản (sử dụng toán tử chuyển đổi của CustomColor)
    // Lưu ý: CustomColor::operator Gdiplus::Color() trả về màu với Alpha=255.
    Gdiplus::Color baseColor = this->strokeColor;

    // 3. Tạo Gdiplus::Color mới với Alpha chính xác và RGB cơ bản
    return Gdiplus::Color(alpha, baseColor.GetR(), baseColor.GetG(), baseColor.GetB());
}
//Gdiplus::Color Stroke::getGdiColor() const {
//    // 1. Tính giá trị Alpha Channel thô (0.0f - 255.0f)
//    float rawAlpha = std::round(this->strokeOpacity * 255.0f);
//
//    // 2. GIỚI HẠN GIÁ TRỊ (CLAMP): Đảm bảo Alpha nằm trong khoảng [0, 255]
//    // Đây là bước quan trọng để tránh lỗi khi chuyển sang BYTE
//    BYTE finalAlpha = static_cast<BYTE>(std::clamp(rawAlpha, 0.0f, 255.0f));
//
//    // 3. Lấy màu RGB cơ bản
//    Gdiplus::Color baseColor = this->strokeColor;
//
//    // 4. Tạo Gdiplus::Color mới với Alpha đã giới hạn và RGB cơ bản
//    return Gdiplus::Color(
//        finalAlpha,
//        baseColor.GetR(),
//        baseColor.GetG(),
//        baseColor.GetB()
//    );
//}

// --- Hàm Parse ---

void Stroke::parse(const std::string& strokeStr) {
    std::string s = strokeStr;
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    size_t pos = s.find(':');
    if (pos == std::string::npos) return;

    std::string key = s.substr(0, pos);
    std::string value = s.substr(pos + 1);

    if (key == "stroke") {
        this->strokeColor = CustomColor::fromStringToCustomColor(value);
    }
    else if (key == "stroke-width") {
        // 2. SỬA LỖI: Thêm try-catch để xử lý lỗi chuyển đổi
        try {
            this->strokeWidth = std::stof(value);
        }
        catch (const std::exception& e) {
            this->strokeWidth = 1.0f; // Đặt giá trị mặc định an toàn
        }
    }
    else if (key == "stroke-opacity") {
        // 2. SỬA LỖI: Thêm try-catch để xử lý lỗi chuyển đổi
        try {
            this->strokeOpacity = std::stof(value);
        }
        catch (const std::exception& e) {
            this->strokeOpacity = 1.0f; // Đặt giá trị mặc định an toàn
        }
    }
}
*/