//#include "CustomColor.h"
//#include <windows.h>
//#include <gdiplus.h>
//using namespace Gdiplus;
//CustomColor::CustomColor() {
//    r = 0;
//    g = 0;
//    b = 0;
//}
//CustomColor::CustomColor(int red, int green, int blue) {
//    r = red;
//    g = green;
//    b = blue;
//}
//int CustomColor::clampCustomColorValue(int value) {
//    if (value < 0) return 0;
//    if (value > 255) return 255;
//    return value;
//}
//// eg: "rgb(255,0,128)"
//CustomColor CustomColor::fromStringToCustomColor(const std::string& rgbString) {
//    // find the position of the parentheses
//    size_t start = rgbString.find('(');
//    size_t end = rgbString.find(')');
//    // return black for invalid format
//    if (start == std::string::npos || end == std::string::npos || end <= start) {
//        return CustomColor(0, 0, 0);
//    }
//
//    std::string values = rgbString.substr(start + 1, end - start - 1);
//    std::stringstream ss(values);
//    std::string segment;
//    int components[3] = { 0, 0, 0 };
//    int i = 0;
//    while (std::getline(ss, segment, ',') && i < 3) {
//        try {
//            components[i] = std::stoi(segment);
//            i++;
//        }
//        catch (...) {
//            components[i] = 0;
//            i++;
//        }
//    }
//    return CustomColor(
//        clampCustomColorValue(components[0]),
//        clampCustomColorValue(components[1]),
//        clampCustomColorValue(components[2])
//    );
//}
//std::string CustomColor::fromCustomColorToString() const {
//    std::stringstream ss;
//    ss << "rgb(" << r << "," << g << "," << b << ")";
//    return ss.str();
//}
//CustomColor::operator Gdiplus::Color() const {
//    if (this == nullptr) { 
//        return Gdiplus::Color(255, 0, 0, 0); 
//    }
//    return Gdiplus::Color(255, static_cast<BYTE>(r), static_cast<BYTE>(g), static_cast<BYTE>(b));
//}

#include "CustomColor.h"
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <algorithm> // Cần cho std::remove_if
#include <stdexcept> // Cần cho stof/stoi exceptions

using namespace Gdiplus;
using namespace std;

// --- Hàm tạo (Constructors) ---

CustomColor::CustomColor() {
    r = 0; // Mặc định là màu đen (black)
    g = 0;
    b = 0;
}

CustomColor::CustomColor(int red, int green, int blue) {
    r = red;
    g = green;
    b = blue;
}

// --- Hàm tiện ích ---

int CustomColor::clampCustomColorValue(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

// --- Phân tích cú pháp màu (Parsing) ---

// eg: "rgb(255,0,128)"
CustomColor CustomColor::fromStringToCustomColor(const std::string& rgbString) {
    // 1. Kiểm tra và loại bỏ khoảng trắng
    string tempStr = rgbString;
    tempStr.erase(remove_if(tempStr.begin(), tempStr.end(), ::isspace), tempStr.end());

    // 2. Kiểm tra định dạng
    if (tempStr.substr(0, 4) != "rgb(") {
        // Nếu không phải định dạng rgb, trả về màu đen (hoặc màu mặc định)
        return CustomColor(0, 0, 0);
    }

    // 3. Trích xuất các giá trị
    size_t start = tempStr.find('(');
    size_t end = tempStr.find(')');

    if (start == string::npos || end == string::npos || end <= start) {
        return CustomColor(0, 0, 0);
    }

    string values = tempStr.substr(start + 1, end - start - 1);
    stringstream ss(values);
    string segment;
    int components[3] = { 0, 0, 0 };
    int i = 0;

    // 4. Phân tách và chuyển đổi
    while (getline(ss, segment, ',') && i < 3) {
        try {
            if (!segment.empty()) {
                // Chuyển đổi chuỗi (đã loại bỏ khoảng trắng) thành số nguyên
                components[i] = std::stoi(segment);
            }
        }
        catch (const std::exception& e) {
            // Nếu lỗi chuyển đổi (ví dụ: chuỗi rỗng), giữ nguyên giá trị 0
            components[i] = 0;
        }
        i++;
    }

    // 5. Trả về CustomColor đã giới hạn giá trị (clamp)
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

// --- Toán tử chuyển đổi Gdiplus ---

CustomColor::operator Gdiplus::Color() const {
    // Kênh Alpha luôn là 255 (hoàn toàn mờ đục)
    // r, g, b là thành viên của đối tượng hiện tại (this)
    return Gdiplus::Color(255,
        static_cast<BYTE>(r),
        static_cast<BYTE>(g),
        static_cast<BYTE>(b));
}