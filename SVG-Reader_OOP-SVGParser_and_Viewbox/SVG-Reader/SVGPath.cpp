/*
////#pragma comment (lib, "Gdiplus.lib")
//#include <iostream>
//#include <cmath>
//#include "SVGPath.h"
//#include "SVGParser.h"
//#include "SVGRenderer.h"
////#include "SVGElement.h"
//
////using namespace Gdiplus;
////using namespace std;
//
//SVGPath::SVGPath()
//    : SVGElement("path", "", SVGStyle()),
//    pathData(""),
//    commands(vector<PathCommand>())
//{
//    transformMatrix = new Gdiplus::Matrix();
//    transformMatrix->Reset();
//}
//
//SVGPath::SVGPath(const std::string& d) 
//    : SVGElement("path", "", SVGStyle()),
//    pathData(d),
//    commands(vector<PathCommand>())
//{
//    transformMatrix = new Gdiplus::Matrix();
//    transformMatrix->Reset();
//    parsePathData();
//}
//
//SVGPath::SVGPath(const SVGPath& other) {
//    this->pathData = other.pathData;
//    this->commands = other.commands;
//    //this->transform = other.transform->Clone();
//}
//
//SVGPath& SVGPath::operator=(const SVGPath& other) {
//    if (this != &other) {
//        pathData = other.pathData;
//        commands = other.commands;
//        //delete this->transform;
//        //this->transform = other.transform->Clone();
//    }
//    return *this;
//}
//
//SVGPath::~SVGPath() {
//    //delete transform;
//}
//
//SVGElement* SVGPath::clone() const {
//    return new SVGPath(*this);
//}
//
//std::string SVGPath::getPathData() const {
//    return pathData;
//}
//
//void SVGPath::setPathData(const std::string& data) {
//    pathData = data;
//    parsePathData();
//}
//
//
////void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* node) {
////    const char* dAttr = node->Attribute("d");
////    if (dAttr) {
////        setPathData(dAttr);
////    }
////
////    // Parse style
////    SVGElement::parse(parser, node);
////
////    // Parse transform
////    const char* tAttr = node->Attribute("transform");
////    if (tAttr) {
////        setTransform(tAttr);
////    }
////}
//
////void SVGPath::setTransform(const std::string& transformStr) {
////    if (!this->transformMatrix)
////        this->transformMatrix = new Gdiplus::Matrix();
////    else
////        this->transformMatrix->Reset();
////
////    Gdiplus::Matrix combined;
////    combined.Reset();
////
////    std::string s = transformStr;
////    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
////
////    size_t pos = 0;
////    while (pos < s.size()) {
////
////        size_t nameEnd = s.find('(', pos);
////        if (nameEnd == std::string::npos) break;
////
////        std::string name = s.substr(pos, nameEnd - pos);
////
////        size_t close = s.find(')', nameEnd);
////        if (close == std::string::npos) break;
////
////        std::string params = s.substr(nameEnd + 1, close - nameEnd - 1);
////
////        std::vector<float> nums;
////        {
////            std::string tmp;
////            for (char c : params) {
////                if (c == ',' || c == ' ') {
////                    if (!tmp.empty()) {
////                        nums.push_back(std::stof(tmp));
////                        tmp.clear();
////                    }
////                }
////                else tmp += c;
////            }
////            if (!tmp.empty()) nums.push_back(std::stof(tmp));
////        }
////
////        Gdiplus::Matrix m;
////        m.Reset();
////
////        if (name == "translate") {
////            float tx = nums.size() > 0 ? nums[0] : 0;
////            float ty = nums.size() > 1 ? nums[1] : 0;
////            m.Translate(tx, ty);
////        }
////        else if (name == "scale") {
////            float sx = nums.size() > 0 ? nums[0] : 1;
////            float sy = nums.size() > 1 ? nums[1] : sx;
////            m.Scale(sx, sy);
////        }
////        else if (name == "rotate") {
////            float angle = nums.size() > 0 ? nums[0] : 0;
////            m.Rotate(angle);
////        }
////
////        combined.Multiply(&m, Gdiplus::MatrixOrderAppend);
////        pos = close + 1;
////    }
////
////    Gdiplus::REAL elems[6];
////    combined.GetElements(elems);
////
////    this->transformMatrix->SetElements(
////        elems[0], elems[1],
////        elems[2], elems[3],
////        elems[4], elems[5]
////    );
////}
//
////void SVGPath::setTransform(const std::string& transformStr) {
////    using namespace Gdiplus;
////
////    if (transformStr.empty()) {
////        this->transformMatrix->Reset();
////        return;
////    }
////
////    // 1. Phân tích cú pháp chuỗi bằng lớp SVGTransform chuyên biệt
////    SVGTransform t = SVGTransform::parse(transformStr);
////
////    // 2. Reset ma trận của đối tượng
////    // Dùng thuộc tính riêng của SVGPath là 'transform'
////    if (!this->transformMatrix) {
////        this->transformMatrix = new Gdiplus::Matrix();
////    }
////    this->transformMatrix->Reset();
////
////    // 3. Áp dụng các phép biến đổi đã phân tích vào ma trận GDI+
////    t.applyToMatrix(*this->transformMatrix); // Áp dụng vào this->transform
////}
//// Trong SVGPath.cpp
//
//void SVGPath::setTransform(const std::string& transformStr) {
//    using namespace Gdiplus;
//
//    if (transformStr.empty()) {
//        this->transformMatrix->Reset();
//        return;
//    }
//
//    // 1. Phân tích cú pháp chuỗi bằng lớp SVGTransform chuyên biệt
//    // Giả sử lớp SVGTransform có hàm static parse:
//    SVGTransform t = SVGTransform::parse(transformStr);
//
//    // 2. Ma trận đã được khởi tạo trong constructor của SVGElement
//    this->transformMatrix->Reset();
//
//    // 3. Áp dụng các phép biến đổi đã phân tích vào ma trận GDI+
//    // Gọi hàm applyToMatrix của lớp SVGTransform
//    t.applyToMatrix(*this->transformMatrix);
//}
//void SVGPath::parsePathData() {
//    commands.clear();
//
//    char cmd = 0;
//    std::string token = "";
//    std::vector <float> params;
//
//    auto flushParams = [&]() {
//        if (cmd != 0) {
//            commands.push_back({ cmd, params });
//            params.clear();
//        }
//        };
//
//    for (size_t i = 0; i < pathData.size(); i++) {
//        char c = pathData[i];
//        if (isalpha(c)) {
//            flushParams();
//            cmd = c;
//        }
//        else if (isdigit(c) || c == '-' || c == '+' || c == '.') {
//            token += c;
//        }
//        else if (c == ' ' || c == ',' || c == '\n' || c == '\t') {
//            if (!token.empty()) {
//                params.push_back(stof(token));
//                token.clear();
//            }
//        }
//    }
//
//    if (!token.empty()) {
//        params.push_back(stof(token));
//    }
//    flushParams();
//}
//
//void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
//    SVGElement::parse(parser, xmlNode);
//    parser.parsePath(this, xmlNode);
//}
//void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
//    r.renderPath(g, const_cast<SVGPath*>(this), commands, context);
//}





#include "tinyxml2.h"
#include <cctype>
#include <sstream>
#include <vector>
#include "SVGPath.h"
#include "SVGParser.h"
#include "SVGRenderer.h"

SVGPath::SVGPath() : SVGElement() {
    this->setTagName("path");
}

SVGElement* SVGPath::clone() const {
    return new SVGPath(*this);
}

//// Helper: Bỏ qua khoảng trắng và dấu phẩy
//static void skipSpacesAndCommas(const std::string& s, size_t& pos) {
//    while (pos < s.size()) {
//        char ch = s[pos];
//        if (std::isspace((unsigned char)ch) || ch == ',')
//            ++pos;
//        else
//            break;
//    }
//}
//
//// Helper: Đọc 1 số float
//static bool parseFloatToken(const std::string& s, size_t& pos, float& out) {
//    skipSpacesAndCommas(s, pos);
//    if (pos >= s.size()) return false;
//
//    size_t start = pos;
//    // Dấu +/-
//    if (s[pos] == '+' || s[pos] == '-') ++pos;
//
//    bool hasDigit = false;
//    while (pos < s.size() && (std::isdigit((unsigned char)s[pos]) || s[pos] == '.')) {
//        hasDigit = true;
//        ++pos;
//    }
//    // Số mũ e/E
//    if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E')) {
//        ++pos;
//        if (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) ++pos;
//        while (pos < s.size() && std::isdigit((unsigned char)s[pos])) {
//            hasDigit = true;
//            ++pos;
//        }
//    }
//
//    if (!hasDigit) return false;
//
//    try {
//        out = std::stof(s.substr(start, pos - start));
//        return true;
//    }
//    catch (...) { return false; }
//}
////parse(SVGParser&, tinyxml2::XMLElement*)
//void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* node) {
//    SVGElement::parse(parser, node);
//
//    const char* dAttr = node->Attribute("d");
//    if (!dAttr) return;
//
//    std::string d = dAttr;
//    size_t pos = 0;
//    char cmd = 0;
//
//    float currentX = 0.0f, currentY = 0.0f;
//    float startX = 0.0f, startY = 0.0f;
//    float lastCtrlX = 0.0f, lastCtrlY = 0.0f;
//
//    while (pos < d.size()) {
//        skipSpacesAndCommas(d, pos);
//        if (pos >= d.size()) break;
//
//        char c = d[pos];
//        // Nếu là ký tự chữ cái -> là lệnh mới
//        if (std::isalpha((unsigned char)c)) {
//            cmd = c;
//            ++pos;
//        }
//        // Nếu không phải chữ cái -> dùng lại lệnh cũ (implicit command)
//
//        bool isRel = (cmd >= 'a' && cmd <= 'z');
//        char absCmd = (char)std::toupper((unsigned char)cmd);
//
//        switch (absCmd) {
//        case 'M': {
//            float x, y;
//            if (!parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
//            if (isRel) { x += currentX; y += currentY; }
//
//            PathCommand pc;
//            pc.type = PathCommandType::MoveTo;
//            pc.params = { x, y };
//            commands.push_back(pc);
//
//            currentX = x; currentY = y;
//            startX = x; startY = y;
//            // Sau M/m, nếu còn số thì hiểu là L/l
//            cmd = (isRel) ? 'l' : 'L';
//            break;
//        }
//        case 'L': {
//            float x, y;
//            if (!parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
//            if (isRel) { x += currentX; y += currentY; }
//
//            PathCommand pc;
//            pc.type = PathCommandType::LineTo;
//            pc.params = { x, y };
//            commands.push_back(pc);
//            currentX = x; currentY = y;
//            break;
//        }
//        case 'H': {
//            float x;
//            if (!parseFloatToken(d, pos, x)) break;
//            if (isRel) x += currentX;
//
//            PathCommand pc;
//            pc.type = PathCommandType::HLineTo;
//            pc.params = { x };
//            commands.push_back(pc);
//            currentX = x;
//            break;
//        }
//        case 'V': {
//            float y;
//            if (!parseFloatToken(d, pos, y)) break;
//            if (isRel) y += currentY;
//
//            PathCommand pc;
//            pc.type = PathCommandType::VLineTo;
//            pc.params = { y };
//            commands.push_back(pc);
//            currentY = y;
//            break;
//        }
//        case 'C': {
//            float x1, y1, x2, y2, x, y;
//            if (!parseFloatToken(d, pos, x1) || !parseFloatToken(d, pos, y1) ||
//                !parseFloatToken(d, pos, x2) || !parseFloatToken(d, pos, y2) ||
//                !parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
//
//            if (isRel) {
//                x1 += currentX; y1 += currentY;
//                x2 += currentX; y2 += currentY;
//                x += currentX; y += currentY;
//            }
//
//            PathCommand pc;
//            pc.type = PathCommandType::CubicBezier;
//            pc.params = { x1, y1, x2, y2, x, y };
//            commands.push_back(pc);
//
//            lastCtrlX = x2; lastCtrlY = y2;
//            currentX = x; currentY = y;
//            break;
//        }
//        case 'S': {
//            float x2, y2, x, y;
//            if (!parseFloatToken(d, pos, x2) || !parseFloatToken(d, pos, y2) ||
//                !parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
//
//            float x1 = currentX, y1 = currentY;
//            // Tính điểm điều khiển đối xứng
//            if (!commands.empty()) {
//                PathCommandType prevType = commands.back().type;
//                if (prevType == PathCommandType::CubicBezier || prevType == PathCommandType::SmoothCubicBezier) {
//                    x1 = 2.0f * currentX - lastCtrlX;
//                    y1 = 2.0f * currentY - lastCtrlY;
//                }
//            }
//
//            if (isRel) {
//                x2 += currentX; y2 += currentY;
//                x += currentX; y += currentY;
//            }
//
//            PathCommand pc;
//            pc.type = PathCommandType::SmoothCubicBezier;
//            // Lưu luôn 3 điểm để render cho dễ
//            pc.params = { x1, y1, x2, y2, x, y };
//            commands.push_back(pc);
//
//            lastCtrlX = x2; lastCtrlY = y2;
//            currentX = x; currentY = y;
//            break;
//        }
//        case 'Z': {
//            PathCommand pc;
//            pc.type = PathCommandType::ClosePath;
//            commands.push_back(pc);
//            currentX = startX; currentY = startY;
//            break;
//        }
//        default:
//            ++pos; // Bỏ qua lệnh chưa hỗ trợ (Q, A...)
//            break;
//        }
//    }
//}


void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    SVGElement::parse(parser, xmlNode);
    parser.parsePath(this, xmlNode);
}

void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderPath(g, this, context);
}
*/
////#pragma comment (lib, "Gdiplus.lib")
//#include <iostream>
//#include <cmath>
//#include "SVGPath.h"
//#include "SVGParser.h"
//#include "SVGRenderer.h"
////#include "SVGElement.h"
//
////using namespace Gdiplus;
////using namespace std;
//
//SVGPath::SVGPath()
//    : SVGElement("path", "", SVGStyle()),
//    pathData(""),
//    commands(vector<PathCommand>())
//{
//    transformMatrix = new Gdiplus::Matrix();
//    transformMatrix->Reset();
//}
//
//SVGPath::SVGPath(const std::string& d) 
//    : SVGElement("path", "", SVGStyle()),
//    pathData(d),
//    commands(vector<PathCommand>())
//{
//    transformMatrix = new Gdiplus::Matrix();
//    transformMatrix->Reset();
//    parsePathData();
//}
//
//SVGPath::SVGPath(const SVGPath& other) {
//    this->pathData = other.pathData;
//    this->commands = other.commands;
//    //this->transform = other.transform->Clone();
//}
//
//SVGPath& SVGPath::operator=(const SVGPath& other) {
//    if (this != &other) {
//        pathData = other.pathData;
//        commands = other.commands;
//        //delete this->transform;
//        //this->transform = other.transform->Clone();
//    }
//    return *this;
//}
//
//SVGPath::~SVGPath() {
//    //delete transform;
//}
//
//SVGElement* SVGPath::clone() const {
//    return new SVGPath(*this);
//}
//
//std::string SVGPath::getPathData() const {
//    return pathData;
//}
//
//void SVGPath::setPathData(const std::string& data) {
//    pathData = data;
//    parsePathData();
//}
//
//
////void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* node) {
////    const char* dAttr = node->Attribute("d");
////    if (dAttr) {
////        setPathData(dAttr);
////    }
////
////    // Parse style
////    SVGElement::parse(parser, node);
////
////    // Parse transform
////    const char* tAttr = node->Attribute("transform");
////    if (tAttr) {
////        setTransform(tAttr);
////    }
////}
//
////void SVGPath::setTransform(const std::string& transformStr) {
////    if (!this->transformMatrix)
////        this->transformMatrix = new Gdiplus::Matrix();
////    else
////        this->transformMatrix->Reset();
////
////    Gdiplus::Matrix combined;
////    combined.Reset();
////
////    std::string s = transformStr;
////    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
////
////    size_t pos = 0;
////    while (pos < s.size()) {
////
////        size_t nameEnd = s.find('(', pos);
////        if (nameEnd == std::string::npos) break;
////
////        std::string name = s.substr(pos, nameEnd - pos);
////
////        size_t close = s.find(')', nameEnd);
////        if (close == std::string::npos) break;
////
////        std::string params = s.substr(nameEnd + 1, close - nameEnd - 1);
////
////        std::vector<float> nums;
////        {
////            std::string tmp;
////            for (char c : params) {
////                if (c == ',' || c == ' ') {
////                    if (!tmp.empty()) {
////                        nums.push_back(std::stof(tmp));
////                        tmp.clear();
////                    }
////                }
////                else tmp += c;
////            }
////            if (!tmp.empty()) nums.push_back(std::stof(tmp));
////        }
////
////        Gdiplus::Matrix m;
////        m.Reset();
////
////        if (name == "translate") {
////            float tx = nums.size() > 0 ? nums[0] : 0;
////            float ty = nums.size() > 1 ? nums[1] : 0;
////            m.Translate(tx, ty);
////        }
////        else if (name == "scale") {
////            float sx = nums.size() > 0 ? nums[0] : 1;
////            float sy = nums.size() > 1 ? nums[1] : sx;
////            m.Scale(sx, sy);
////        }
////        else if (name == "rotate") {
////            float angle = nums.size() > 0 ? nums[0] : 0;
////            m.Rotate(angle);
////        }
////
////        combined.Multiply(&m, Gdiplus::MatrixOrderAppend);
////        pos = close + 1;
////    }
////
////    Gdiplus::REAL elems[6];
////    combined.GetElements(elems);
////
////    this->transformMatrix->SetElements(
////        elems[0], elems[1],
////        elems[2], elems[3],
////        elems[4], elems[5]
////    );
////}
//
////void SVGPath::setTransform(const std::string& transformStr) {
////    using namespace Gdiplus;
////
////    if (transformStr.empty()) {
////        this->transformMatrix->Reset();
////        return;
////    }
////
////    // 1. Phân tích cú pháp chuỗi bằng lớp SVGTransform chuyên biệt
////    SVGTransform t = SVGTransform::parse(transformStr);
////
////    // 2. Reset ma trận của đối tượng
////    // Dùng thuộc tính riêng của SVGPath là 'transform'
////    if (!this->transformMatrix) {
////        this->transformMatrix = new Gdiplus::Matrix();
////    }
////    this->transformMatrix->Reset();
////
////    // 3. Áp dụng các phép biến đổi đã phân tích vào ma trận GDI+
////    t.applyToMatrix(*this->transformMatrix); // Áp dụng vào this->transform
////}
//// Trong SVGPath.cpp
//
//void SVGPath::setTransform(const std::string& transformStr) {
//    using namespace Gdiplus;
//
//    if (transformStr.empty()) {
//        this->transformMatrix->Reset();
//        return;
//    }
//
//    // 1. Phân tích cú pháp chuỗi bằng lớp SVGTransform chuyên biệt
//    // Giả sử lớp SVGTransform có hàm static parse:
//    SVGTransform t = SVGTransform::parse(transformStr);
//
//    // 2. Ma trận đã được khởi tạo trong constructor của SVGElement
//    this->transformMatrix->Reset();
//
//    // 3. Áp dụng các phép biến đổi đã phân tích vào ma trận GDI+
//    // Gọi hàm applyToMatrix của lớp SVGTransform
//    t.applyToMatrix(*this->transformMatrix);
//}
//void SVGPath::parsePathData() {
//    commands.clear();
//
//    char cmd = 0;
//    std::string token = "";
//    std::vector <float> params;
//
//    auto flushParams = [&]() {
//        if (cmd != 0) {
//            commands.push_back({ cmd, params });
//            params.clear();
//        }
//        };
//
//    for (size_t i = 0; i < pathData.size(); i++) {
//        char c = pathData[i];
//        if (isalpha(c)) {
//            flushParams();
//            cmd = c;
//        }
//        else if (isdigit(c) || c == '-' || c == '+' || c == '.') {
//            token += c;
//        }
//        else if (c == ' ' || c == ',' || c == '\n' || c == '\t') {
//            if (!token.empty()) {
//                params.push_back(stof(token));
//                token.clear();
//            }
//        }
//    }
//
//    if (!token.empty()) {
//        params.push_back(stof(token));
//    }
//    flushParams();
//}
//
//void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
//    SVGElement::parse(parser, xmlNode);
//    parser.parsePath(this, xmlNode);
//}
//void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
//    r.renderPath(g, const_cast<SVGPath*>(this), commands, context);
//}





#include "SVGPath.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "tinyxml2.h"
#include <cctype>
#include <sstream>
#include <vector>

SVGPath::SVGPath() : SVGElement() {
    this->setTagName("path");
}

SVGElement* SVGPath::clone() const {
    return new SVGPath(*this);
}
//void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
//    SVGElement::parse(parser, xmlNode);
//    parser.parsePath(this, xmlNode);
//}
void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderPath(g, this, context);
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

// Helper: Đọc 1 số float
static bool parseFloatToken(const std::string& s, size_t& pos, float& out) {
    skipSpacesAndCommas(s, pos);
    if (pos >= s.size()) return false;

    size_t start = pos;
    // Dấu +/-
    if (s[pos] == '+' || s[pos] == '-') ++pos;

    bool hasDigit = false;
    while (pos < s.size() && (std::isdigit((unsigned char)s[pos]) || s[pos] == '.')) {
        hasDigit = true;
        ++pos;
    }
    // Số mũ e/E
    if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E')) {
        ++pos;
        if (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) ++pos;
        while (pos < s.size() && std::isdigit((unsigned char)s[pos])) {
            hasDigit = true;
            ++pos;
        }
    }

    if (!hasDigit) return false;

    try {
        out = std::stof(s.substr(start, pos - start));
        return true;
    }
    catch (...) { return false; }
}
//parse(SVGParser&, tinyxml2::XMLElement*)
void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* node) {
    SVGElement::parse(parser, node);

    const char* dAttr = node->Attribute("d");
    if (!dAttr) return;

    std::string d = dAttr;
    size_t pos = 0;
    char cmd = 0;

    float currentX = 0.0f, currentY = 0.0f;
    float startX = 0.0f, startY = 0.0f;
    float lastCtrlX = 0.0f, lastCtrlY = 0.0f;

    while (pos < d.size()) {
        skipSpacesAndCommas(d, pos);
        if (pos >= d.size()) break;

        char c = d[pos];
        // Nếu là ký tự chữ cái -> là lệnh mới
        if (std::isalpha((unsigned char)c)) {
            cmd = c;
            ++pos;
        }
        // Nếu không phải chữ cái -> dùng lại lệnh cũ (implicit command)

        bool isRel = (cmd >= 'a' && cmd <= 'z');
        char absCmd = (char)std::toupper((unsigned char)cmd);

        switch (absCmd) {
        case 'M': {
            float x, y;
            if (!parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
            if (isRel) { x += currentX; y += currentY; }

            PathCommand pc;
            pc.type = PathCommandType::MoveTo;
            pc.params = { x, y };
            commands.push_back(pc);

            currentX = x; currentY = y;
            startX = x; startY = y;
            // Sau M/m, nếu còn số thì hiểu là L/l
            cmd = (isRel) ? 'l' : 'L';
            break;
        }
        case 'L': {
            float x, y;
            if (!parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;
            if (isRel) { x += currentX; y += currentY; }

            PathCommand pc;
            pc.type = PathCommandType::LineTo;
            pc.params = { x, y };
            commands.push_back(pc);
            currentX = x; currentY = y;
            break;
        }
        case 'H': {
            float x;
            if (!parseFloatToken(d, pos, x)) break;
            if (isRel) x += currentX;

            PathCommand pc;
            pc.type = PathCommandType::HLineTo;
            pc.params = { x };
            commands.push_back(pc);
            currentX = x;
            break;
        }
        case 'V': {
            float y;
            if (!parseFloatToken(d, pos, y)) break;
            if (isRel) y += currentY;

            PathCommand pc;
            pc.type = PathCommandType::VLineTo;
            pc.params = { y };
            commands.push_back(pc);
            currentY = y;
            break;
        }
        case 'C': {
            float x1, y1, x2, y2, x, y;
            if (!parseFloatToken(d, pos, x1) || !parseFloatToken(d, pos, y1) ||
                !parseFloatToken(d, pos, x2) || !parseFloatToken(d, pos, y2) ||
                !parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;

            if (isRel) {
                x1 += currentX; y1 += currentY;
                x2 += currentX; y2 += currentY;
                x += currentX; y += currentY;
            }

            PathCommand pc;
            pc.type = PathCommandType::CubicBezier;
            pc.params = { x1, y1, x2, y2, x, y };
            commands.push_back(pc);

            lastCtrlX = x2; lastCtrlY = y2;
            currentX = x; currentY = y;
            break;
        }
        case 'S': {
            float x2, y2, x, y;
            if (!parseFloatToken(d, pos, x2) || !parseFloatToken(d, pos, y2) ||
                !parseFloatToken(d, pos, x) || !parseFloatToken(d, pos, y)) break;

            float x1 = currentX, y1 = currentY;
            // Tính điểm điều khiển đối xứng
            if (!commands.empty()) {
                PathCommandType prevType = commands.back().type;
                if (prevType == PathCommandType::CubicBezier || prevType == PathCommandType::SmoothCubicBezier) {
                    x1 = 2.0f * currentX - lastCtrlX;
                    y1 = 2.0f * currentY - lastCtrlY;
                }
            }

            if (isRel) {
                x2 += currentX; y2 += currentY;
                x += currentX; y += currentY;
            }

            PathCommand pc;
            pc.type = PathCommandType::SmoothCubicBezier;
            // Lưu luôn 3 điểm để render cho dễ
            pc.params = { x1, y1, x2, y2, x, y };
            commands.push_back(pc);

            lastCtrlX = x2; lastCtrlY = y2;
            currentX = x; currentY = y;
            break;
        }
        case 'Z': {
            PathCommand pc;
            pc.type = PathCommandType::ClosePath;
            commands.push_back(pc);
            currentX = startX; currentY = startY;
            break;
        }
        default:
            ++pos; // Bỏ qua lệnh chưa hỗ trợ (Q, A...)
            break;
        }
    }
}

//void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
//    r.renderPath(g, this);
//}



