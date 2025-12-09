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
#include <cctype>    // Cần cho ::isspace, ::tolower
#include <unordered_map>
#include <cmath>
#include <climits>


using namespace Gdiplus;
using namespace std;

// --- Hàm tiện ích ---

static const unordered_map<string, CustomColor> SVG_NAMED_COLORS = {
    // Màu cơ bản
    {"red", CustomColor(255, 0, 0)},
    {"green", CustomColor(0, 128, 0)},
    {"blue", CustomColor(0, 0, 255)},
    {"yellow", CustomColor(255, 255, 0)},
    {"black", CustomColor(0, 0, 0)},
    {"white", CustomColor(255, 255, 255)},
    {"gray", CustomColor(128, 128, 128)},
    {"transparent", CustomColor(0, 0, 0)},
    {"none", CustomColor(0, 0, 0)},

    {"darkred", CustomColor(139, 0, 0)},
    {"darkgreen", CustomColor(0, 100, 0)},
    {"darkblue", CustomColor(0, 0, 139)},
    {"navy", CustomColor(0, 0, 128)},
    {"teal", CustomColor(0, 128, 128)},
    {"fuchsia", CustomColor(255, 0, 255)},
    {"silver", CustomColor(192, 192, 192)},
    {"maroon", CustomColor(128, 0, 0)},
    {"purple", CustomColor(128, 0, 128)},
    {"lime", CustomColor(0, 255, 0)},
    {"olive", CustomColor(128, 128, 0)},
    {"aliceblue", CustomColor(240, 248, 255)},
    {"antiquewhite", CustomColor(250, 235, 215)},
    {"aqua", CustomColor(0, 255, 255)},
    {"midnightblue", CustomColor(25, 25, 112)}, // Màu chuẩn
    {"bluemidnight", CustomColor(25, 25, 112)}  // Alias
};

// --- HELPERS ---

// Hàm chuyển đổi HEX sang số nguyên
int hexToDec(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

// Hàm phân tích cú pháp màu HEX (#RRGGBB)
CustomColor parseHexColor(const string& hexStr) {
    if (hexStr.size() < 7 || hexStr[0] != '#') return CustomColor(0, 0, 0);

    try {
        int r = (hexToDec(hexStr[1]) << 4) + hexToDec(hexStr[2]);
        int g = (hexToDec(hexStr[3]) << 4) + hexToDec(hexStr[4]);
        int b = (hexToDec(hexStr[5]) << 4) + hexToDec(hexStr[6]);

        return CustomColor(r, g, b);
    }
    catch (...) {
        return CustomColor(0, 0, 0);
    }
}


int CustomColor::clampCustomColorValue(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

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

// --- Phân tích cú pháp màu (Parsing) ---

////eg: "rgb(255,0,128)"
//CustomColor CustomColor::fromStringToCustomColor(const std::string& rgbString) {
//    // 1. Kiểm tra và loại bỏ khoảng trắng
//    string tempStr = rgbString;
//    tempStr.erase(remove_if(tempStr.begin(), tempStr.end(), ::isspace), tempStr.end());
//
//    // 2. Kiểm tra định dạng
//    if (tempStr.substr(0, 4) != "rgb(") {
//        // Nếu không phải định dạng rgb, trả về màu đen (hoặc màu mặc định)
//        return CustomColor(0, 0, 0);
//    }
//
//    // 3. Trích xuất các giá trị
//    size_t start = tempStr.find('(');
//    size_t end = tempStr.find(')');
//
//    if (start == string::npos || end == string::npos || end <= start) {
//        return CustomColor(0, 0, 0);
//    }
//
//    string values = tempStr.substr(start + 1, end - start - 1);
//    stringstream ss(values);
//    string segment;
//    int components[3] = { 0, 0, 0 };
//    int i = 0;
//
//    // 4. Phân tách và chuyển đổi
//    while (getline(ss, segment, ',') && i < 3) {
//        try {
//            if (!segment.empty()) {
//                // Chuyển đổi chuỗi (đã loại bỏ khoảng trắng) thành số nguyên
//                components[i] = std::stoi(segment);
//            }
//        }
//        catch (const std::exception& e) {
//            // Nếu lỗi chuyển đổi (ví dụ: chuỗi rỗng), giữ nguyên giá trị 0
//            components[i] = 0;
//        }
//        i++;
//    }
//
//    // 5. Trả về CustomColor đã giới hạn giá trị (clamp)
//    return CustomColor(
//        clampCustomColorValue(components[0]),
//        clampCustomColorValue(components[1]),
//        clampCustomColorValue(components[2])
//    );
//}

// eg: "rgb(255,0,128)" + "#172C51" + "none" + "blue mid night"
CustomColor CustomColor::fromStringToCustomColor(const std::string& rgbString) {
    string tempStr = rgbString;

    // 1. Loại bỏ khoảng trắng và tạo chuỗi chữ thường
    tempStr.erase(remove_if(tempStr.begin(), tempStr.end(), ::isspace), tempStr.end());
    string lowerStr = tempStr;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    // 2. Xử lý HEX (#RRGGBB)
    if (tempStr.size() >= 7 && tempStr[0] == '#') {
        return parseHexColor(tempStr);
    }

    // 3. Xử lý RGB (rgb(r,g,b))
    if (lowerStr.size() >= 4 && lowerStr.substr(0, 4) == "rgb(") {
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

        while (getline(ss, segment, ',') && i < 3) {
            try {
                if (!segment.empty()) {
                    components[i] = std::stoi(segment);
                }
            }
            catch (const std::exception& e) {
                components[i] = 0;
            }
            i++;
        }
        return CustomColor(
            clampCustomColorValue(components[0]),
            clampCustomColorValue(components[1]),
            clampCustomColorValue(components[2])
        );
    }

    // 4. Xử lý Tên Màu (Bao gồm 'none' và 'transparent')
    if (SVG_NAMED_COLORS.count(lowerStr)) {
        return SVG_NAMED_COLORS.at(lowerStr);
    }

    // 5. Trường hợp không xác định
    return CustomColor(0, 0, 0);
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

Gdiplus::ARGB CustomColor::getARGB(float opacity) const {
    float clamped_opacity = (std::min)(1.0f, (std::max)(0.0f, opacity));
    Gdiplus::BYTE alpha = static_cast<Gdiplus::BYTE>(std::round(clamped_opacity * 255.0f));

    // Gộp A, R, G, B thành giá trị 32-bit
    Gdiplus::ARGB argb = ((static_cast<Gdiplus::ARGB>(alpha) << 24)) |
        ((static_cast<Gdiplus::ARGB>(r) << 16)) |
        ((static_cast<Gdiplus::ARGB>(g) << 8)) |
        (static_cast<Gdiplus::ARGB>(b));

    return argb;
}