#ifndef UTILS_HPP
#define UTILS_HPP

#include <windows.h>
#include <gdiplus.h>
#include <string.h>
#include <string>
#include "CustomColor.h"

// Sử dụng 'final' để ngăn kế thừa
class Utils final {
private: 
	// 1. Private Constructor: Ngăn việc tạo đối tượng từ bên ngoài
	Utils() = default;
public: 
	// 2. Các hàm tĩnh (Static methods)
	
	// Hàm bổ trợ: Điều chỉnh độ loang
	static float applyBias(float x, float bias) {
		if (x <= 0.0f) return 0.0f;
		if (x >= 1.0f) return 1.0f;
		return x / ((1.0f / bias - 2.0f) * (1.0f - x) + 1.0f);
	}

	// Hàm bổ trợ: Tính độ sáng của màu
	static float getLuminance(Gdiplus::Color c) {
		return 0.299f * c.GetR() + 0.587f * c.GetG() + 0.114f * c.GetB();
	}

	// Chuyển đổi SpreadMethod sang WrapMode của GDI+
	inline Gdiplus::WrapMode toGdiWrapMode(const std::string& spread) {
		if (spread == "reflect") return Gdiplus::WrapModeTileFlipXY; // Bí kíp làm mượt màu chéo
		if (spread == "repeat") return Gdiplus::WrapModeTile;
		return Gdiplus::WrapModeClamp; // Mặc định là "pad"
	}

	// Chuyển đổi tọa độ tỷ lệ (%) sang tọa độ tuyệt đối trên màn hình
	inline Gdiplus::PointF mapPoint(float x, float y, const Gdiplus::RectF& b) {
		return Gdiplus::PointF(b.X + x * b.Width, b.Y + y * b.Height);
	} 

	static bool isBlueHeavy(const Gdiplus::Color& c) {
		return c.GetB() > c.GetR() + 25 &&
			c.GetB() > c.GetG() + 20;
	}

	static bool isPurpleZone(const Gdiplus::Color& c) {
		return c.GetR() > 120 && c.GetB() > 120;
	}

    static Gdiplus::Color normalizeRadialColor(const Gdiplus::Color& src, float offset) {
        int r = src.GetR();
        int g = src.GetG();
        int b = src.GetB();
        BYTE a = src.GetA();

        // Chỉ can thiệp vùng ngoài tâm
        if (offset > 0.35f) {

            // Giảm xanh gắt (Instagram, Firefox glow...)
            if (isBlueHeavy(src)) {
                r = (std::min)(255, r + int(32 * offset));
                b = (std::max)(0, b - int(36 * offset));
            }

            // Giữ tím không chuyển xanh
            if (isPurpleZone(src)) {
                r = (std::min)(255, r + int(14 * offset));
                g = (std::min)(255, g + int(6 * offset));
            }
        }

        return Gdiplus::Color(a, r, g, b);
    }

    static Gdiplus::Color stabilizeHue(const Gdiplus::Color& src, float offset) {
        int r = src.GetR();
        int g = src.GetG();
        int b = src.GetB();
        int a = src.GetA();

        // Vùng ngoài (nơi IG bị bẻ xanh)
        if (offset > 0.45f) {
            // Nếu xanh chiếm ưu thế
            if (b > r && b > g) {
                float t = (offset - 0.45f) / 0.55f;
                t = std::clamp(t, 0.0f, 1.0f);

                r += int(45 * t);   // kéo về tím
                b -= int(35 * t);   // giảm xanh
            }
        }

        return Gdiplus::Color(a,
            (BYTE)std::clamp(r, 0, 255),
            (BYTE)std::clamp(g, 0, 255),
            (BYTE)std::clamp(b, 0, 255)
        );
    }

    // Helper: Bỏ qua khoảng trắng và dấu phẩy
    static void skipSpacesAndCommas(const std::string& s, size_t& pos) {
        while (pos < s.size()) {
            char ch = s[pos];
            if (std::isspace((unsigned char)ch) || ch == ',')
                ++pos;
            else
                break;
        }
    }

    static bool parseFloatToken(const std::string& s, size_t& pos, float& out) {
        skipSpacesAndCommas(s, pos);
        if (pos >= s.size()) return false;
        size_t start = pos;
        bool hasDecimal = false;
        for (; pos < s.size(); ++pos) {
            char ch = s[pos];
            if (std::isdigit((unsigned char)ch)) continue;
            if (ch == '-' || ch == '+') { if (pos == start) continue; else break; }
            if (ch == '.') { if (hasDecimal) break; hasDecimal = true; continue; }
            if (ch == 'e' || ch == 'E') {
                if (pos + 1 < s.size() && (std::isdigit(s[pos + 1]) || s[pos + 1] == '-' || s[pos + 1] == '+')) {
                    pos++; continue;
                }
                break;
            }
            break;
        }
        if (start == pos) return false;
        try {
            out = std::stof(s.substr(start, pos - start));
            return true;
        }
        catch (...) { return false; }
    }

    static bool parseArcFlag(const std::string& s, size_t& pos, float& out) {
        skipSpacesAndCommas(s, pos);
        if (pos < s.size() && (s[pos] == '0' || s[pos] == '1')) {
            out = (float)(s[pos] - '0');
            pos++;
            return true;
        }
        return false;
    }

    static std::vector<float>parseNumbers(const std::string& s)
    {
        std::vector<float> nums;
        std::stringstream ss(s);
        std::string token;

        char c;
        std::string cur;
        for (char ch : s) {
            if (ch == ',' || isspace((unsigned char)ch)) {
                if (!cur.empty()) {
                    nums.push_back(stof(cur));
                    cur.clear();
                }
            }
            else {
                cur.push_back(ch);
            }
        }

        if (!cur.empty()) {
            nums.push_back(stof(cur));
        }
        return nums;
    }

    static void parseColorString(const std::string& val, CustomColor& outColor, float& outOpacity, bool& hasAlpha) {
        std::string s = val;
        s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

        if (s.rfind("rgba(", 0) == 0) {
            size_t start = s.find('(');
            size_t end = s.find(')');
            if (start != std::string::npos && end != std::string::npos) {
                std::string content = s.substr(start + 1, end - start - 1);
                std::stringstream ss(content);
                std::string segment;
                int r = 0, g = 0, b = 0;
                float a = 1.0f;
                try {
                    if (std::getline(ss, segment, ',')) r = std::stoi(segment);
                    if (std::getline(ss, segment, ',')) g = std::stoi(segment);
                    if (std::getline(ss, segment, ',')) b = std::stoi(segment);
                    if (std::getline(ss, segment, ',')) a = std::stof(segment);
                    outColor = CustomColor(r, g, b);
                    outOpacity = a;
                    hasAlpha = true;
                    return;
                }
                catch (...) {}
            }
        }
        outColor = CustomColor::fromStringToCustomColor(val);
        outOpacity = 1.0f;
        hasAlpha = false;
    }

	// Xóa các hàm copy/assignment để đảm bảo tính duy nhất
	Utils(const Utils&) = delete;
	Utils& operator=(const Utils&) = delete;
};
#endif