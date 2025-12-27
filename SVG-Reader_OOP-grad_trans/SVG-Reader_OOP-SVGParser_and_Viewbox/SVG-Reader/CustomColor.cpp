/*
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

//#include <windows.h>
//#include <gdiplus.h>
//#include <sstream>
//#include <algorithm> // Cần cho std::remove_if
//#include <stdexcept> // Cần cho stof/stoi exceptions
//#include <cctype>    // Cần cho ::isspace, ::tolower
//#include <unordered_map>
//#include <cmath>
//#include <climits>
#include "CustomColor.h"

//namespace Gdiplus {
//    // Định nghĩa lại các kiểu GDI+ cơ bản để tránh lỗi C2039/C2027/E0135
//    // Kiểu BYTE bị lỗi:
//    typedef unsigned char BYTE;
//    // Kiểu ARGB bị lỗi: (thường là 32-bit unsigned integer)
//    typedef unsigned char ARGB;
//
//    // Nếu bạn đang dùng Gdiplus::Color, bạn không thể dễ dàng định nghĩa lại.
//    // Nếu Gdiplus::Color cũng bị lỗi, việc sử dụng nó có thể là gốc rễ.
//}

//using namespace Gdiplus;
//using namespace std;

// --- Hàm tiện ích ---

static const std::unordered_map<std::string, CustomColor> SVG_NAMED_COLORS = {
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
CustomColor parseHexColor(const std::string& hexStr) {
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
    std::string tempStr = rgbString;

    // 1. Loại bỏ khoảng trắng và tạo chuỗi chữ thường
    tempStr.erase(remove_if(tempStr.begin(), tempStr.end(), ::isspace), tempStr.end());
    //std::string lowerStr = tempStr;
    //std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    std::string lowerStr;
    lowerStr.resize(tempStr.size());
    std::transform(tempStr.begin(), tempStr.end(), lowerStr.begin(), [](unsigned char c) {
        return ::tolower(c);
        });

    // 2. Xử lý HEX (#RRGGBB)
    if (tempStr.size() >= 7 && tempStr[0] == '#') {
        return parseHexColor(tempStr);
    }

    // 3. Xử lý RGB (rgb(r,g,b))
    if (lowerStr.size() >= 4 && lowerStr.substr(0, 4) == "rgb(") {
        size_t start = tempStr.find('(');
        size_t end = tempStr.find(')');

        if (start == std::string::npos || end == std::string::npos || end <= start) {
            return CustomColor(0, 0, 0);
        }

        std::string values = tempStr.substr(start + 1, end - start - 1);
        std::stringstream ss(values);
        std::string segment;
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

//CustomColor::operator Gdiplus::Color() const {
//    // Kênh Alpha luôn là 255 (hoàn toàn mờ đục)
//    // r, g, b là thành viên của đối tượng hiện tại (this)
//    return Gdiplus::Color(255,
//        static_cast<BYTE>(r),
//        static_cast<BYTE>(g),
//        static_cast<BYTE>(b));
//} 

// old code
CustomColor::operator Gdiplus::Color() const {
    if (this == nullptr) { 
        return Gdiplus::Color(255, 0, 0, 0); 
    }
    return Gdiplus::Color(255, static_cast<BYTE>(r), static_cast<BYTE>(g), static_cast<BYTE>(b));
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
*/
#include "CustomColor.h"

//static const std::unordered_map<std::string, CustomColor> SVG_NAMED_COLORS = {
//    // Màu cơ bản
//    {"red", CustomColor(255, 0, 0)},
//    {"green", CustomColor(0, 128, 0)},
//    {"blue", CustomColor(0, 0, 255)},
//    {"yellow", CustomColor(255, 255, 0)},
//    {"black", CustomColor(0, 0, 0)},
//    {"white", CustomColor(255, 255, 255)},
//    {"gray", CustomColor(128, 128, 128)},
//    {"transparent", CustomColor(0, 0, 0)},
//    {"none", CustomColor(0, 0, 0)},
//
//    {"darkred", CustomColor(139, 0, 0)},
//    {"darkgreen", CustomColor(0, 100, 0)},
//    {"darkblue", CustomColor(0, 0, 139)},
//    {"navy", CustomColor(0, 0, 128)},
//    {"teal", CustomColor(0, 128, 128)},
//    {"fuchsia", CustomColor(255, 0, 255)},
//    {"silver", CustomColor(192, 192, 192)},
//    {"maroon", CustomColor(128, 0, 0)},
//    {"purple", CustomColor(128, 0, 128)},
//    {"lime", CustomColor(0, 255, 0)},
//    {"olive", CustomColor(128, 128, 0)},
//    {"aliceblue", CustomColor(240, 248, 255)},
//    {"antiquewhite", CustomColor(250, 235, 215)},
//    {"aqua", CustomColor(0, 255, 255)},
//    {"midnightblue", CustomColor(25, 25, 112)}, // Màu chuẩn
//    {"bluemidnight", CustomColor(25, 25, 112)}  // Alias
//};
static const std::unordered_map<std::string, CustomColor> SVG_NAMED_COLORS = {
    // --- Các màu cơ bản và chuẩn hóa (Giữ lại của bạn) ---
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
    {"midnightblue", CustomColor(25, 25, 112)},
    {"bluemidnight", CustomColor(25, 25, 112)},

    // --- BỔ SUNG CÁC TÊN GỌI KHÁC (ALIAS) ---
    {"cyan", CustomColor(0, 255, 255)},       // Alias cho Aqua
    {"magenta", CustomColor(255, 0, 255)},    // Alias cho Fuchsia (từ magenta1)
    {"grey", CustomColor(128, 128, 128)},     // Alias cho gray
    {"darkgrey", CustomColor(169, 169, 169)}, // Alias cho darkgray
    {"lightgrey", CustomColor(211, 211, 211)},// Alias cho lightgray

    // --- Các màu còn lại (424 màu đã gửi) ---
    {"ghostwhite", CustomColor(248, 248, 255)},
    {"whitesmoke", CustomColor(245, 245, 245)},
    {"gainsboro", CustomColor(220, 220, 220)},
    {"dimgray", CustomColor(105, 105, 105)},
    {"lightgray", CustomColor(211, 211, 211)},
    {"gray11", CustomColor(28, 28, 28)},
    {"gray21", CustomColor(54, 54, 54)},
    {"gray31", CustomColor(79, 79, 79)},
    {"gray41", CustomColor(105, 105, 105)},
    {"gray51", CustomColor(130, 130, 130)},
    {"gray61", CustomColor(156, 156, 156)},
    {"gray71", CustomColor(181, 181, 181)},
    {"gray81", CustomColor(207, 207, 207)},
    {"gray91", CustomColor(232, 232, 232)},
    {"darkgray", CustomColor(169, 169, 169)},
    {"lightslategray", CustomColor(119, 136, 153)},
    {"slategray", CustomColor(112, 128, 144)},
    {"slategray1", CustomColor(198, 226, 255)},
    {"slategray2", CustomColor(185, 211, 238)},
    {"slategray3", CustomColor(159, 182, 205)},
    {"slategray4", CustomColor(108, 123, 139)},
    {"darkslategray", CustomColor(47, 79, 79)},
    {"darkslategray1", CustomColor(151, 255, 255)},
    {"darkslategray2", CustomColor(141, 238, 238)},
    {"darkslategray3", CustomColor(121, 205, 205)},
    {"darkslategray4", CustomColor(82, 139, 139)},
    {"palegoldenrod", CustomColor(238, 232, 170)},
    {"floralwhite", CustomColor(255, 250, 240)},
    {"lightgoldenrodyellow", CustomColor(250, 250, 210)},
    {"saddlebrown", CustomColor(139, 69, 19)},
    {"sienna", CustomColor(160, 82, 45)},
    {"oldlace", CustomColor(253, 245, 230)},
    {"linen", CustomColor(250, 240, 230)},
    {"papayawhip", CustomColor(255, 239, 213)},
    {"blanchedalmond", CustomColor(255, 235, 205)},
    {"moccasin", CustomColor(255, 228, 181)},
    {"peru", CustomColor(205, 133, 63)},
    {"beige", CustomColor(245, 245, 220)},
    {"sandybrown", CustomColor(244, 164, 96)},
    {"snow1", CustomColor(255, 250, 250)},
    {"snow2", CustomColor(238, 233, 233)},
    {"snow3", CustomColor(205, 201, 201)},
    {"snow4", CustomColor(139, 137, 137)},
    {"seashell1", CustomColor(255, 245, 238)},
    {"seashell2", CustomColor(238, 229, 222)},
    {"seashell3", CustomColor(205, 197, 191)},
    {"seashell4", CustomColor(139, 134, 130)},
    {"antiquewhite1", CustomColor(255, 239, 219)},
    {"antiquewhite2", CustomColor(238, 223, 204)},
    {"antiquewhite3", CustomColor(205, 192, 176)},
    {"antiquewhite4", CustomColor(139, 131, 120)},
    {"bisque1", CustomColor(255, 228, 196)},
    {"bisque2", CustomColor(238, 213, 183)},
    {"bisque3", CustomColor(205, 183, 158)},
    {"bisque4", CustomColor(139, 125, 107)},
    {"peachpuff1", CustomColor(255, 218, 185)},
    {"peachpuff2", CustomColor(238, 203, 173)},
    {"peachpuff3", CustomColor(205, 175, 149)},
    {"peachpuff4", CustomColor(139, 119, 101)},
    {"navajowhite1", CustomColor(255, 222, 173)},
    {"navajowhite2", CustomColor(238, 207, 161)},
    {"navajowhite3", CustomColor(205, 179, 139)},
    {"navajowhite4", CustomColor(139, 121, 94)},
    {"lemonchiffon1", CustomColor(255, 250, 205)},
    {"lemonchiffon2", CustomColor(238, 233, 191)},
    {"lemonchiffon3", CustomColor(205, 201, 165)},
    {"lemonchiffon4", CustomColor(139, 137, 112)},
    {"cornsilk1", CustomColor(255, 248, 220)},
    {"cornsilk2", CustomColor(238, 232, 205)},
    {"cornsilk3", CustomColor(205, 200, 177)},
    {"cornsilk4", CustomColor(139, 136, 120)},
    {"ivory1", CustomColor(255, 255, 240)},
    {"ivory2", CustomColor(238, 238, 224)},
    {"ivory3", CustomColor(205, 205, 193)},
    {"ivory4", CustomColor(139, 139, 131)},
    {"mintcream", CustomColor(245, 255, 250)},
    {"honeydew1", CustomColor(240, 255, 240)},
    {"honeydew2", CustomColor(224, 238, 224)},
    {"honeydew3", CustomColor(193, 205, 193)},
    {"honeydew4", CustomColor(131, 139, 131)},
    {"lavenderblush1", CustomColor(255, 240, 245)},
    {"lavenderblush2", CustomColor(238, 224, 229)},
    {"lavenderblush3", CustomColor(205, 193, 197)},
    {"lavenderblush4", CustomColor(139, 131, 134)},
    {"lavender", CustomColor(230, 230, 250)},
    {"mistyrose1", CustomColor(255, 228, 225)},
    {"mistyrose2", CustomColor(238, 213, 210)},
    {"mistyrose3", CustomColor(205, 183, 181)},
    {"mistyrose4", CustomColor(139, 125, 123)},
    {"azure1", CustomColor(240, 255, 255)},
    {"azure2", CustomColor(224, 238, 238)},
    {"azure3", CustomColor(193, 205, 205)},
    {"azure4", CustomColor(131, 139, 139)},
    {"lightslateblue", CustomColor(132, 112, 255)},
    {"mediumslateblue", CustomColor(123, 104, 238)},
    {"slateblue", CustomColor(106, 90, 205)},
    {"slateblue1", CustomColor(131, 111, 255)},
    {"slateblue2", CustomColor(122, 103, 238)},
    {"slateblue3", CustomColor(105, 89, 205)},
    {"slateblue4", CustomColor(71, 60, 139)},
    {"darkslateblue", CustomColor(72, 61, 139)},
    {"royalblue", CustomColor(65, 105, 225)},
    {"royalblue1", CustomColor(72, 118, 255)},
    {"royalblue2", CustomColor(67, 110, 238)},
    {"royalblue3", CustomColor(58, 95, 205)},
    {"royalblue4", CustomColor(39, 64, 139)},
    {"blue1", CustomColor(0, 0, 255)},
    {"blue2", CustomColor(0, 0, 238)},
    {"darkblue", CustomColor(0, 0, 139)},
    {"navyblue", CustomColor(0, 0, 128)},
    {"midnightblue", CustomColor(25, 25, 112)},
    {"cornflowerblue", CustomColor(100, 149, 237)},
    {"mediumblue", CustomColor(0, 0, 205)},
    {"powderblue", CustomColor(176, 224, 230)},
    {"dodgerblue1", CustomColor(30, 144, 255)},
    {"dodgerblue2", CustomColor(28, 134, 238)},
    {"dodgerblue3", CustomColor(24, 116, 205)},
    {"dodgerblue4", CustomColor(16, 78, 139)},
    {"steelblue", CustomColor(70, 130, 180)},
    {"steelblue1", CustomColor(99, 184, 255)},
    {"steelblue2", CustomColor(92, 172, 238)},
    {"steelblue3", CustomColor(79, 148, 205)},
    {"steelblue4", CustomColor(54, 100, 139)},
    {"deepskyblue", CustomColor(0, 191, 255)},
    {"deepskyblue2", CustomColor(0, 178, 238)},
    {"deepskyblue3", CustomColor(0, 154, 205)},
    {"deepskyblue4", CustomColor(0, 104, 139)},
    {"skyblue", CustomColor(135, 206, 235)},
    {"skyblue1", CustomColor(135, 206, 255)},
    {"skyblue2", CustomColor(126, 192, 238)},
    {"skyblue3", CustomColor(108, 166, 205)},
    {"skyblue4", CustomColor(74, 112, 139)},
    {"lightskyblue", CustomColor(135, 206, 250)},
    {"lightskyblue1", CustomColor(176, 226, 255)},
    {"lightskyblue2", CustomColor(164, 211, 238)},
    {"lightskyblue3", CustomColor(141, 182, 205)},
    {"lightskyblue4", CustomColor(96, 123, 139)},
    {"lightsteelblue", CustomColor(176, 196, 222)},
    {"lightsteelblue1", CustomColor(202, 225, 255)},
    {"lightsteelblue2", CustomColor(188, 210, 238)},
    {"lightsteelblue3", CustomColor(162, 181, 205)},
    {"lightsteelblue4", CustomColor(110, 123, 139)},
    {"lightblue", CustomColor(173, 216, 230)},
    {"lightblue1", CustomColor(191, 239, 255)},
    {"lightblue2", CustomColor(178, 223, 238)},
    {"lightblue3", CustomColor(154, 192, 205)},
    {"lightblue4", CustomColor(104, 131, 139)},
    {"lightcyan1", CustomColor(224, 255, 255)},
    {"lightcyan2", CustomColor(209, 238, 238)},
    {"lightcyan3", CustomColor(180, 205, 205)},
    {"lightcyan4", CustomColor(122, 139, 139)},
    {"cadetblue", CustomColor(95, 158, 160)},
    {"cadetblue1", CustomColor(152, 245, 255)},
    {"cadetblue2", CustomColor(142, 229, 238)},
    {"cadetblue3", CustomColor(122, 197, 205)},
    {"cadetblue4", CustomColor(83, 134, 139)},
    {"paleturquoise", CustomColor(175, 238, 238)},
    {"paleturquoise1", CustomColor(187, 255, 255)},
    {"paleturquoise2", CustomColor(174, 238, 238)},
    {"paleturquoise3", CustomColor(150, 205, 205)},
    {"paleturquoise4", CustomColor(102, 139, 139)},
    {"mediumturquoise", CustomColor(72, 209, 204)},
    {"darkturquoise", CustomColor(0, 206, 209)},
    {"turquoise", CustomColor(64, 224, 208)},
    {"turquoise1", CustomColor(0, 245, 255)},
    {"turquoise2", CustomColor(0, 229, 238)},
    {"turquoise3", CustomColor(0, 197, 205)},
    {"turquoise4", CustomColor(0, 134, 139)},
    {"cyan1", CustomColor(0, 255, 255)},
    {"cyan2", CustomColor(0, 238, 238)},
    {"cyan3", CustomColor(0, 205, 205)},
    {"darkcyan", CustomColor(0, 139, 139)},
    {"aquamarine1", CustomColor(127, 255, 212)},
    {"aquamarine2", CustomColor(118, 238, 198)},
    {"aquamarine3", CustomColor(102, 205, 170)},
    {"aquamarine4", CustomColor(69, 139, 116)},
    {"darkseagreen", CustomColor(143, 188, 143)},
    {"darkseagreen1", CustomColor(193, 255, 193)},
    {"darkseagreen2", CustomColor(180, 238, 180)},
    {"darkseagreen3", CustomColor(155, 205, 155)},
    {"darkseagreen4", CustomColor(105, 139, 105)},
    {"seagreen", CustomColor(46, 139, 87)},
    {"seagreen1", CustomColor(84, 255, 159)},
    {"seagreen2", CustomColor(78, 238, 148)},
    {"seagreen3", CustomColor(67, 205, 128)},
    {"palegreen", CustomColor(152, 251, 152)},
    {"palegreen1", CustomColor(154, 255, 154)},
    {"palegreen2", CustomColor(144, 238, 144)},
    {"palegreen3", CustomColor(124, 205, 124)},
    {"palegreen4", CustomColor(84, 139, 84)},
    {"springgreen1", CustomColor(0, 255, 127)},
    {"springgreen2", CustomColor(0, 238, 118)},
    {"springgreen3", CustomColor(0, 205, 102)},
    {"springgreen4", CustomColor(0, 139, 69)},
    {"green1", CustomColor(0, 255, 0)},
    {"green2", CustomColor(0, 238, 0)},
    {"green3", CustomColor(0, 205, 0)},
    {"green4", CustomColor(0, 139, 0)},
    {"mediumseagreen", CustomColor(60, 179, 113)},
    {"lightseagreen", CustomColor(32, 178, 170)},
    {"lightgreen", CustomColor(144, 238, 144)},
    {"lawngreen", CustomColor(124, 252, 0)},
    {"mediumspringgreen", CustomColor(0, 250, 154)},
    {"greenyellow", CustomColor(173, 255, 47)},
    {"limegreen", CustomColor(50, 205, 50)},
    {"yellowgreen", CustomColor(154, 205, 50)},
    {"forestgreen", CustomColor(34, 139, 34)},
    {"chartreuse1", CustomColor(127, 255, 0)},
    {"chartreuse2", CustomColor(118, 238, 0)},
    {"chartreuse3", CustomColor(102, 205, 0)},
    {"chartreuse4", CustomColor(69, 139, 0)},
    {"olivedrab", CustomColor(107, 142, 35)},
    {"olivedrab1", CustomColor(192, 255, 62)},
    {"olivedrab2", CustomColor(179, 238, 58)},
    {"olivedrab3", CustomColor(154, 205, 50)},
    {"olivedrab4", CustomColor(105, 139, 34)},
    {"darkolivegreen", CustomColor(85, 107, 47)},
    {"darkolivegreen1", CustomColor(202, 255, 112)},
    {"darkolivegreen2", CustomColor(188, 238, 104)},
    {"darkolivegreen3", CustomColor(162, 205, 90)},
    {"darkolivegreen4", CustomColor(110, 139, 61)},
    {"khaki1", CustomColor(255, 246, 143)},
    {"khaki2", CustomColor(238, 230, 133)},
    {"khaki3", CustomColor(205, 198, 115)},
    {"khaki4", CustomColor(139, 134, 78)},
    {"darkkhaki", CustomColor(189, 183, 107)},
    {"lightgoldenrod", CustomColor(238, 221, 130)},
    {"lightgoldenrod1", CustomColor(255, 236, 139)},
    {"lightgoldenrod2", CustomColor(238, 220, 130)},
    {"lightgoldenrod3", CustomColor(205, 190, 112)},
    {"lightgoldenrod4", CustomColor(139, 129, 76)},
    {"lightyellow1", CustomColor(255, 255, 224)},
    {"lightyellow2", CustomColor(238, 238, 209)},
    {"lightyellow3", CustomColor(205, 205, 180)},
    {"lightyellow4", CustomColor(139, 139, 122)},
    {"yellow1", CustomColor(255, 255, 0)},
    {"yellow2", CustomColor(238, 238, 0)},
    {"yellow3", CustomColor(205, 205, 0)},
    {"yellow4", CustomColor(139, 139, 0)},
    {"gold1", CustomColor(255, 215, 0)},
    {"gold2", CustomColor(238, 201, 0)},
    {"gold3", CustomColor(205, 173, 0)},
    {"gold4", CustomColor(139, 117, 0)},
    {"goldenrod", CustomColor(218, 165, 32)},
    {"goldenrod1", CustomColor(255, 193, 37)},
    {"goldenrod2", CustomColor(238, 180, 34)},
    {"goldenrod3", CustomColor(205, 155, 29)},
    {"goldenrod4", CustomColor(139, 105, 20)},
    {"darkgoldenrod", CustomColor(184, 134, 11)},
    {"darkgoldenrod1", CustomColor(255, 185, 15)},
    {"darkgoldenrod2", CustomColor(238, 173, 14)},
    {"darkgoldenrod3", CustomColor(205, 149, 12)},
    {"darkgoldenrod4", CustomColor(139, 101, 8)},
    {"rosybrown", CustomColor(188, 143, 143)},
    {"rosybrown1", CustomColor(255, 193, 193)},
    {"rosybrown2", CustomColor(238, 180, 180)},
    {"rosybrown3", CustomColor(205, 155, 155)},
    {"rosybrown4", CustomColor(139, 105, 105)},
    {"indianred", CustomColor(205, 92, 92)},
    {"indianred1", CustomColor(255, 106, 106)},
    {"indianred2", CustomColor(238, 99, 99)},
    {"indianred3", CustomColor(205, 85, 85)},
    {"indianred4", CustomColor(139, 58, 58)},
    {"sienna1", CustomColor(255, 130, 71)},
    {"sienna2", CustomColor(238, 121, 66)},
    {"sienna3", CustomColor(205, 104, 57)},
    {"sienna4", CustomColor(139, 71, 38)},
    {"burlywood", CustomColor(222, 184, 135)},
    {"burlywood1", CustomColor(255, 211, 155)},
    {"burlywood2", CustomColor(238, 197, 145)},
    {"burlywood3", CustomColor(205, 170, 125)},
    {"burlywood4", CustomColor(139, 115, 85)},
    {"wheat", CustomColor(245, 222, 179)},
    {"wheat1", CustomColor(255, 231, 186)},
    {"wheat2", CustomColor(238, 216, 174)},
    {"wheat3", CustomColor(205, 186, 150)},
    {"wheat4", CustomColor(139, 126, 102)},
    {"tan", CustomColor(210, 180, 140)},
    {"tan1", CustomColor(255, 165, 79)},
    {"tan2", CustomColor(238, 154, 73)},
    {"tan3", CustomColor(205, 133, 63)},
    {"tan4", CustomColor(139, 90, 43)},
    {"chocolate", CustomColor(210, 105, 30)},
    {"chocolate1", CustomColor(255, 127, 36)},
    {"chocolate2", CustomColor(238, 118, 33)},
    {"chocolate3", CustomColor(205, 102, 29)},
    {"chocolate4", CustomColor(139, 69, 19)},
    {"firebrick", CustomColor(178, 34, 34)},
    {"firebrick1", CustomColor(255, 48, 48)},
    {"firebrick2", CustomColor(238, 44, 44)},
    {"firebrick3", CustomColor(205, 38, 38)},
    {"firebrick4", CustomColor(139, 26, 26)},
    {"brown", CustomColor(165, 42, 42)},
    {"brown1", CustomColor(255, 64, 64)},
    {"brown2", CustomColor(238, 59, 59)},
    {"brown3", CustomColor(205, 51, 51)},
    {"brown4", CustomColor(139, 35, 35)},
    {"salmon", CustomColor(250, 128, 114)},
    {"salmon1", CustomColor(255, 140, 105)},
    {"salmon2", CustomColor(238, 130, 98)},
    {"salmon3", CustomColor(205, 112, 84)},
    {"salmon4", CustomColor(139, 76, 57)},
    {"lightsalmon1", CustomColor(255, 160, 122)},
    {"lightsalmon2", CustomColor(238, 149, 114)},
    {"lightsalmon3", CustomColor(205, 129, 98)},
    {"lightsalmon4", CustomColor(139, 87, 66)},
    {"orange1", CustomColor(255, 165, 0)},
    {"orange2", CustomColor(238, 154, 0)},
    {"orange3", CustomColor(205, 133, 0)},
    {"orange4", CustomColor(139, 90, 0)},
    {"darkorange", CustomColor(255, 140, 0)},
    {"darkorange1", CustomColor(255, 127, 0)},
    {"darkorange2", CustomColor(238, 118, 0)},
    {"darkorange3", CustomColor(205, 102, 0)},
    {"darkorange4", CustomColor(139, 69, 0)},
    {"lightcoral", CustomColor(240, 128, 128)},
    {"coral", CustomColor(255, 127, 80)},
    {"coral1", CustomColor(255, 114, 86)},
    {"coral2", CustomColor(238, 106, 80)},
    {"coral3", CustomColor(205, 91, 69)},
    {"coral4", CustomColor(139, 62, 47)},
    {"tomato1", CustomColor(255, 99, 71)},
    {"tomato2", CustomColor(238, 92, 66)},
    {"tomato3", CustomColor(205, 79, 57)},
    {"tomato4", CustomColor(139, 54, 38)},
    {"orangered1", CustomColor(255, 69, 0)},
    {"orangered2", CustomColor(238, 64, 0)},
    {"orangered3", CustomColor(205, 55, 0)},
    {"orangered4", CustomColor(139, 37, 0)},
    {"red1", CustomColor(255, 0, 0)},
    {"red2", CustomColor(238, 0, 0)},
    {"red3", CustomColor(205, 0, 0)},
    {"darkred", CustomColor(139, 0, 0)},
    {"rouge", CustomColor(198, 0, 0)},
    {"crimson", CustomColor(220, 20, 60)},
    {"deeppink1", CustomColor(255, 20, 147)},
    {"deeppink2", CustomColor(238, 18, 137)},
    {"deeppink3", CustomColor(205, 16, 118)},
    {"deeppink4", CustomColor(139, 10, 80)},
    {"hotpink", CustomColor(255, 105, 180)},
    {"hotpink1", CustomColor(255, 110, 180)},
    {"hotpink2", CustomColor(238, 106, 167)},
    {"hotpink3", CustomColor(205, 96, 144)},
    {"hotpink4", CustomColor(139, 58, 98)},
    {"pink", CustomColor(255, 192, 203)},
    {"pink1", CustomColor(255, 181, 197)},
    {"pink2", CustomColor(238, 169, 184)},
    {"pink3", CustomColor(205, 145, 158)},
    {"pink4", CustomColor(139, 99, 108)},
    {"lightpink", CustomColor(255, 182, 193)},
    {"lightpink1", CustomColor(255, 174, 185)},
    {"lightpink2", CustomColor(238, 162, 173)},
    {"lightpink3", CustomColor(205, 140, 149)},
    {"lightpink4", CustomColor(139, 95, 101)},
    {"palevioletred", CustomColor(219, 112, 147)},
    {"palevioletred1", CustomColor(255, 130, 171)},
    {"palevioletred2", CustomColor(238, 121, 159)},
    {"palevioletred3", CustomColor(205, 104, 137)},
    {"palevioletred4", CustomColor(139, 71, 93)},
    {"maroon", CustomColor(176, 48, 96)},
    {"maroon1", CustomColor(255, 52, 179)},
    {"maroon2", CustomColor(238, 48, 167)},
    {"maroon3", CustomColor(205, 41, 144)},
    {"maroon4", CustomColor(139, 28, 98)},
    {"violetred", CustomColor(208, 32, 144)},
    {"violetred1", CustomColor(255, 62, 150)},
    {"violetred2", CustomColor(238, 58, 140)},
    {"violetred3", CustomColor(205, 50, 120)},
    {"violetred4", CustomColor(139, 34, 82)},
    {"magenta1", CustomColor(255, 0, 255)},
    {"magenta2", CustomColor(238, 0, 238)},
    {"magenta3", CustomColor(205, 0, 205)},
    {"darkmagenta", CustomColor(139, 0, 139)},
    {"orchid", CustomColor(218, 112, 214)},
    {"orchid1", CustomColor(255, 131, 250)},
    {"orchid2", CustomColor(238, 122, 233)},
    {"orchid3", CustomColor(205, 105, 201)},
    {"orchid4", CustomColor(139, 71, 137)},
    {"plum", CustomColor(221, 160, 221)},
    {"plum1", CustomColor(255, 187, 255)},
    {"plum2", CustomColor(238, 174, 238)},
    {"plum3", CustomColor(205, 150, 205)},
    {"plum4", CustomColor(139, 102, 139)},
    {"mediumorchid", CustomColor(186, 85, 211)},
    {"mediumorchid1", CustomColor(224, 102, 255)},
    {"mediumorchid2", CustomColor(209, 95, 238)},
    {"mediumorchid3", CustomColor(180, 82, 205)},
    {"mediumorchid4", CustomColor(122, 55, 139)},
    {"darkorchid", CustomColor(153, 50, 204)},
    {"darkorchid1", CustomColor(191, 62, 255)},
    {"darkorchid2", CustomColor(178, 58, 238)},
    {"darkorchid3", CustomColor(154, 50, 205)},
    {"darkorchid4", CustomColor(104, 34, 139)},
    {"mediumvioletred", CustomColor(199, 21, 133)},
    {"violet", CustomColor(238, 130, 238)},
    {"darkviolet", CustomColor(148, 0, 211)},
    {"blueviolet", CustomColor(138, 43, 226)},
    {"purple1", CustomColor(160, 32, 240)},
    {"purple2", CustomColor(155, 48, 255)},
    {"purple3", CustomColor(145, 44, 238)},
    {"purple4", CustomColor(125, 38, 205)},
    {"purple5", CustomColor(85, 26, 139)},
    {"mediumpurple", CustomColor(147, 112, 219)},
    {"mediumpurple1", CustomColor(171, 130, 255)},
    {"mediumpurple2", CustomColor(159, 121, 238)},
    {"mediumpurple3", CustomColor(137, 104, 205)},
    {"mediumpurple4", CustomColor(93, 71, 139)},
    {"thistle", CustomColor(216, 191, 216)},
    {"thistle1", CustomColor(255, 225, 255)},
    {"thistle2", CustomColor(238, 210, 238)},
    {"thistle3", CustomColor(205, 181, 205)},
    {"thistle4", CustomColor(139, 123, 139)},
    {"antiquegold", CustomColor(221, 196, 136)},
    {"agedpaper", CustomColor(236, 171, 83)},
    {"dark cyan", CustomColor(0, 128, 128)},
    {"peach-orange", CustomColor(255, 204, 153)}
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
CustomColor parseHexColor(const std::string& hexStr) {
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
    std::string tempStr = rgbString;

    // 1. Loại bỏ khoảng trắng và tạo chuỗi chữ thường
    tempStr.erase(remove_if(tempStr.begin(), tempStr.end(), ::isspace), tempStr.end());

    std::string lowerStr;
    lowerStr.resize(tempStr.size());
    std::transform(tempStr.begin(), tempStr.end(), lowerStr.begin(), [](unsigned char c) {
        return ::tolower(c);
        });

    // Nếu chuỗi rỗng sau khi loại bỏ khoảng trắng, trả về mặc định
    if (tempStr.empty()) {
        return CustomColor(0, 0, 0);
    }

    // -------------------------------------------------------------
    // 2. Xử lý MÃ HEX (#RRGGBB, #RGB, RRGGBB, RGB)
    // -------------------------------------------------------------

    std::string hexStr = tempStr;

    // Nếu thiếu dấu #, thêm vào để dễ dàng xử lý sau này
    if (hexStr[0] != '#') {
        hexStr = "#" + tempStr;
    }

    size_t len = hexStr.size();

    // Kiểm tra định dạng HEX: #RGB (4) hoặc #RRGGBB (7)
    if (len == 4 && hexStr[0] == '#') {
        // Xử lý rút gọn: #RGB -> #RRGGBB
        std::string expandedHex = "#";
        // RR
        expandedHex += hexStr[1]; expandedHex += hexStr[1];
        // GG
        expandedHex += hexStr[2]; expandedHex += hexStr[2];
        // BB
        expandedHex += hexStr[3]; expandedHex += hexStr[3];

        // Kiểm tra xem các ký tự có phải là hex hợp lệ không
        if (std::all_of(expandedHex.begin() + 1, expandedHex.end(), [](char c) { return std::isxdigit(c); })) {
            return parseHexColor(expandedHex);
        }
    }
    else if (len == 7 && hexStr[0] == '#') {
        // Định dạng chuẩn: #RRGGBB
        // Kiểm tra xem các ký tự có phải là hex hợp lệ không
        if (std::all_of(hexStr.begin() + 1, hexStr.end(), [](char c) { return std::isxdigit(c); })) {
            return parseHexColor(hexStr);
        }
    }


    // -------------------------------------------------------------
    // 3. Xử lý RGB (rgb(r,g,b))
    // -------------------------------------------------------------
    if (lowerStr.size() >= 4 && lowerStr.substr(0, 4) == "rgb(") {
        size_t start = tempStr.find('(');
        size_t end = tempStr.find(')');

        if (start == std::string::npos || end == std::string::npos || end <= start) {
            return CustomColor(0, 0, 0);
        }

        std::string values = tempStr.substr(start + 1, end - start - 1);
        std::stringstream ss(values);
        std::string segment;
        int components[3] = { 0, 0, 0 };
        int i = 0;

        while (getline(ss, segment, ',') && i < 3) {
            try {
                if (!segment.empty()) {
                    components[i] = std::stoi(segment);
                }
            }
            catch (const std::exception& e) {
                // Nếu có lỗi parse, giữ nguyên giá trị 0
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

    // -------------------------------------------------------------
    // 4. Xử lý Tên Màu (Bao gồm 'none' và 'transparent')
    // -------------------------------------------------------------
    // Giả định SVG_NAMED_COLORS là std::map<std::string, CustomColor>
    if (SVG_NAMED_COLORS.count(lowerStr)) {
        return SVG_NAMED_COLORS.at(lowerStr);
    }

    // -------------------------------------------------------------
    // 5. Trường hợp không xác định/Lỗi
    // -------------------------------------------------------------
    return CustomColor(0, 0, 0);
}


std::string CustomColor::fromCustomColorToString() const {
    std::stringstream ss;
    ss << "rgb(" << r << "," << g << "," << b << ")";
    return ss.str();
}

// --- Toán tử chuyển đổi Gdiplus ---

//CustomColor::operator Gdiplus::Color() const {
//    // Kênh Alpha luôn là 255 (hoàn toàn mờ đục)
//    // r, g, b là thành viên của đối tượng hiện tại (this)
//    return Gdiplus::Color(255,
//        static_cast<BYTE>(r),
//        static_cast<BYTE>(g),
//        static_cast<BYTE>(b));
//} 

// old code
CustomColor::operator Gdiplus::Color() const {
    if (this == nullptr) {
        return Gdiplus::Color(255, 0, 0, 0);
    }
    return Gdiplus::Color(255, static_cast<BYTE>(r), static_cast<BYTE>(g), static_cast<BYTE>(b));
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