/*#include "Color.h"

Color::Color() {
    r = 0;
    g = 0;      
    b = 0;
}
Color::Color(int red, int green, int blue) {
    r = red;
    g = green;      
    b = blue;
};
*/
#include "Color.h"
#include <sstream>
#include <algorithm>
#include <unordered_map>

Color Color::fromString(const std::string& str) {
    std::string s = str;
    // loại bỏ khoảng trắng
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    // map các màu tên cơ bản
    static std::unordered_map<std::string, Color> colorMap = {
        {"black", Color(0,0,0)},
        {"white", Color(255,255,255)},
        {"red", Color(255,0,0)},
        {"green", Color(0,128,0)},
        {"blue", Color(0,0,255)},
        {"yellow", Color(255,255,0)},
        {"gray", Color(128,128,128)}
        // bạn có thể thêm nhiều màu khác
    };

    // nếu là màu tên
    auto it = colorMap.find(s);
    if (it != colorMap.end()) return it->second;

    // nếu là màu hex: #RRGGBB
    if (!s.empty() && s[0] == '#' && s.length() == 7) {
        int r = std::stoi(s.substr(1, 2), nullptr, 16);
        int g = std::stoi(s.substr(3, 2), nullptr, 16);
        int b = std::stoi(s.substr(5, 2), nullptr, 16);
        return Color(r, g, b);
    }

    // mặc định: đen nếu không nhận diện được
    return Color(0, 0, 0);
}
