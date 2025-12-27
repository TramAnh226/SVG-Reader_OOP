#include <sstream>
#include <algorithm>
#include <iostream>
#include "Stroke.h";
#include "SVGParser.h"

// using namespace tinyxml2;
// using namespace std;

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

//Gdiplus::Matrix* SVGParser::parseTransform(const std::string& str) {
//    Gdiplus::Matrix* t = new Gdiplus::Matrix();
//    std::string s = str;
//
//    size_t pos = 0;
//    while (pos < s.size()) {
//        // XỬ LÝ MATRIX(a, b, c, d, e, f)
//        if (s.compare(pos, 6, "matrix") == 0) {
//            pos += 6;
//            size_t l = s.find('(', pos);
//            size_t r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//
//            std::string inside = s.substr(l + 1, r - l - 1);
//            auto nums = parseNumbers(inside); // Đảm bảo hàm này tách được dấu phẩy hoặc khoảng trắng
//            if (nums.size() >= 6) {
//                // SVG matrix: a, b, c, d, e, f tương ứng với ma trận biến đổi 3x3
//                Gdiplus::Matrix m(nums[0], nums[1], nums[2], nums[3], nums[4], nums[5]);
//                t->Multiply(&m);
//            }
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 9, "translate") == 0) {
//            pos += 9;
//            size_t l = s.find('(', pos), r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
//            t->Translate(nums.size() > 0 ? nums[0] : 0.0f, nums.size() > 1 ? nums[1] : 0.0f);
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 5, "scale") == 0) {
//            pos += 5;
//            size_t l = s.find('(', pos), r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
//            float sx = nums.size() > 0 ? nums[0] : 1.0f;
//            float sy = nums.size() > 1 ? nums[1] : sx;
//            t->Scale(sx, sy);
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 6, "rotate") == 0) {
//            pos += 6;
//            size_t l = s.find('(', pos), r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
//            t->Rotate(nums.size() > 0 ? nums[0] : 0.0f);
//            pos = r + 1;
//        }
//        else {
//            pos++;
//        }
//    }
//    return t;
//}
//
//void SVGParser::parseTransform(SVGTransform& transform, const std::string& str)
//{
//    string s = str;
//
//    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
//        return !isspace(ch);
//        }));
//
//    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
//        return !std::isspace(ch);
//        }).base(), s.end());
//
//    size_t pos = 0;
//    while (pos < s.size()) {
//        if (s.compare(pos, 9, "translate") == 0) {
//            pos += 9;
//            size_t l = s.find('(', pos);
//            size_t r = s.find(')', l + 1);
//            if (l == string::npos || r == string::npos) break;
//
//            string inside = s.substr(l + 1, r - l - 1);
//            auto nums = parseNumbers(inside);
//            float tx = nums.size() > 0 ? nums[0] : 0.0f;
//            float ty = nums.size() > 1 ? nums[1] : 0.0f;
//            transform.addTranslate(tx, ty);
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 6, "rotate") == 0) {
//            pos += 6;
//            size_t l = s.find('(', pos);
//            size_t r = s.find(')', l + 1);
//            if (l == string::npos || r == string::npos) break;
//
//            string inside = s.substr(l + 1, r - l - 1);
//            auto nums = parseNumbers(inside);
//            float deg = nums.size() > 0 ? nums[0] : 0.0f;
//            transform.addRotate(deg);
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 5, "scale") == 0) {
//            pos += 5;
//            size_t l = s.find('(', pos);
//            size_t r = s.find(')', l + 1);
//            if (l == string::npos || r == string::npos) break;
//
//            string inside = s.substr(l + 1, r - l - 1);
//            auto nums = parseNumbers(inside);
//            float sx = nums.size() > 0 ? nums[0] : 1.0f;
//            float sy = nums.size() > 1 ? nums[1] : sx;
//            transform.addScale(sx, sy);
//            pos = r + 1;
//        }
//        else {
//            ++pos;
//        }
//    }
//}

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

//static std::vector<float>parseNumbers(const std::string& s)
//{
//    std::vector<float> nums;
//    std::stringstream ss(s);
//    std::string token;
//
//    char c;
//    std::string cur;
//    for (char ch : s) {
//        if (ch == ',' || isspace((unsigned char)ch)) {
//            if (!cur.empty()) {
//                nums.push_back(stof(cur));
//                cur.clear();
//            }
//        }
//        else {
//            cur.push_back(ch);
//        }
//    }
//
//    if (!cur.empty()) {
//        nums.push_back(stof(cur));
//    }
//    return nums;
//}

//Gdiplus::Matrix* SVGParser::parseTransform(const std::string& str) {
//    Gdiplus::Matrix* t = new Gdiplus::Matrix();
//    std::string s = str;
//
//    size_t pos = 0;
//    while (pos < s.size()) {
//        // XỬ LÝ MATRIX(a, b, c, d, e, f)
//        if (s.compare(pos, 6, "matrix") == 0) {
//            pos += 6;
//            size_t l = s.find('(', pos);
//            size_t r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//
//            std::string inside = s.substr(l + 1, r - l - 1);
//            auto nums = parseNumbers(inside); // Đảm bảo hàm này tách được dấu phẩy hoặc khoảng trắng
//            if (nums.size() >= 6) {
//                // SVG matrix: a, b, c, d, e, f tương ứng với ma trận biến đổi 3x3
//                Gdiplus::Matrix m(nums[0], nums[1], nums[2], nums[3], nums[4], nums[5]);
//                t->Multiply(&m);
//            }
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 9, "translate") == 0) {
//            pos += 9;
//            size_t l = s.find('(', pos), r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
//            t->Translate(nums.size() > 0 ? nums[0] : 0.0f, nums.size() > 1 ? nums[1] : 0.0f);
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 5, "scale") == 0) {
//            pos += 5;
//            size_t l = s.find('(', pos), r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
//            float sx = nums.size() > 0 ? nums[0] : 1.0f;
//            float sy = nums.size() > 1 ? nums[1] : sx;
//            t->Scale(sx, sy);
//            pos = r + 1;
//        }
//        else if (s.compare(pos, 6, "rotate") == 0) {
//            pos += 6;
//            size_t l = s.find('(', pos), r = s.find(')', l + 1);
//            if (l == std::string::npos || r == std::string::npos) break;
//            auto nums = parseNumbers(s.substr(l + 1, r - l - 1));
//            t->Rotate(nums.size() > 0 ? nums[0] : 0.0f);
//            pos = r + 1;
//        }
//        else {
//            pos++;
//        }
//    }
//    return t;
//}

// Hàm hỗ trợ chuyển đổi chuỗi sang enum cho TextAnchor
TextAnchor SVGParser::parseTextAnchor(const std::string& value) {
    if (value == "middle") return TextAnchor::Middle;
    if (value == "end") return TextAnchor::End;
    return TextAnchor::Start; // Mặc định là 'start'
}

// Hàm hỗ trợ chuyển đổi chuỗi sang enum cho FontStyle
FontStyles SVGParser::parseFontStyle(const std::string& value) {
    if (value == "italic" || value == "oblique") return FontStyles::Italic;
    return FontStyles::Normal; // Mặc định là 'normal'
}

// Hàm hỗ trợ chuyển đổi chuỗi sang enum cho FontWeight
FontWeight SVGParser::parseFontWeight(const std::string& value) {
    // Chấp nhận "bold" hoặc giá trị số > 400
    if (value == "bold") return FontWeight::Bold;

    try {
        int weight = std::stoi(value);
        if (weight >= 700) return FontWeight::Bold;
    }
    catch (...) {
        // Bỏ qua lỗi nếu không phải là số
    }
    return FontWeight::Normal; // Mặc định là 'normal' (400)
}

SVGGroup* SVGParser::readXML(const std::string& filename, SVGFactoryPattern& factory, SVGDocumentContext& context) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Cannot read SVG file: " << filename << '\n';
        return nullptr;
    }

    tinyxml2::XMLElement* rootXML = doc.RootElement();
    if (!rootXML) {
        std::cerr << " SVG file is invalid!" << '\n';
        return nullptr;
    }

    SVGGroup* rootGroup = new SVGGroup();
    rootGroup->setParent(nullptr);
    rootGroup->setTagName(rootXML->Name());

    parseAttributes(rootXML, rootGroup);
    rootGroup->parse(*this, rootXML);

    parseNode(rootXML, rootGroup, factory, context);
    // them parse thuoc tinh group
    return rootGroup;
}



// sua thanh da hinh (bo if-else)
void SVGParser::parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory, SVGDocumentContext& context) {

    std::unordered_map<std::string, std::string> inheritedMapForChildren;
    if (parentGroup) {
        // Lấy Map từ Group cha
        inheritedMapForChildren = parentGroup->getInheritedAttributes();
    }

    for (tinyxml2::XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {

        std::string tagName = child->Name();

        if (tagName == "defs") {
            parseDEFS(child, factory, context);
            continue;
        }

        SVGElement* element = factory.getElement(tagName);

        if (!element) {
            continue;
        }

        if (parentGroup) {
            element->setSVGStyle(parentGroup->getSVGStyle());
        }

        parseAttributes(child, element);
        if (SVGText* textElement = dynamic_cast<SVGText*>(element)) {
            // Trường hợp đặc biệt: Element là Text, gọi hàm parse mới với Group cha
            // (Chúng ta không cần gọi element->parse, vì logic đó đã chuyển vào parseText)

            // LƯU Ý: Nếu bạn muốn giữ cấu trúc đa hình, bạn cần sửa đổi SVGText::parse để nó
            // lấy Group cha từ một cơ chế khác. Nhưng cách đơn giản nhất là gọi trực tiếp:
            this->parseText(textElement, child, parentGroup);

        }
        else {
            // Đối với các Element khác (Rectangle, Path, Circle, v.v.)
            // Logic parse không cần Group cha (chỉ cần thuộc tính trực tiếp)
            element->parse(*this, child);
        }
        parentGroup->addElement(element);

        if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
            group->mergeInheritedAttributes(inheritedMapForChildren);
            parseNode(child, group, factory, context);
        }
    }
}

void SVGParser::parseDEFS(tinyxml2::XMLElement* defsNode, SVGFactoryPattern& factory, SVGDocumentContext& context) {
    for (tinyxml2::XMLElement* child = defsNode->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string tagName = child->Name();
        SVGGradient* grad = factory.getGradientType(tagName);

        if (grad) {
            // 2. PARSE PHẦN CHUNG (ID, STOPS, UNITS,...)
            // Gọi hàm chung chỉ xử lý thuộc tính chung
            parseGradient(grad, child);

            // 3. PARSE PHẦN RIÊNG (Tọa độ)
            // Phân loại và gọi hàm parsing riêng
            if (auto linearGrad = dynamic_cast<SVGLinearGradient*>(grad)) {
                // Gọi hàm riêng để xử lý x1, y1, x2, y2
                parseLinearGradient(linearGrad, child);
            }
            else if (auto radialGrad = dynamic_cast<SVGRadialGradient*>(grad)) {
                // Gọi hàm riêng để xử lý cx, cy, r, fx, fy
                parseRadialGradient(radialGrad, child);
            }

            // 4. LƯU VÀO CONTEXT (Sau khi parsing hoàn tất)
            if (!grad->getGradientID().empty()) {
                context.addGradient(grad);
            }
            else {
                std::cerr << "Warning: Skipping gradient without ID.\n";
                delete grad;
            }
        }
        // Có thể thêm logic cho <pattern> hoặc <mask> sau này
    }
}

//float SVGParser::parseFloatValue(const char* attr) {
//    if (!attr) return 0.0f;
//    std::string s(attr);
//    try {
//        // Xóa khoảng trắng thừa
//        size_t first = s.find_first_not_of(" \t\r\n");
//        if (std::string::npos == first) return 0.0f;
//        size_t last = s.find_last_not_of(" \t\r\n");
//        std::string trimmed = s.substr(first, (last - first + 1));
//
//        float val = std::stof(trimmed);
//        // Nếu có dấu %, chia cho 100 để đưa về hệ số (0.0 - 1.0)
//        if (trimmed.find('%') != std::string::npos) {
//            return val / 100.0f;
//        }
//        return val;
//    }
//    catch (...) { return 0.0f; }
//}

float SVGParser::parseFloatValue(const char* attr) {
    if (!attr) return 0.0f;
    std::string s(attr);
    try {
        size_t first = s.find_first_not_of(" \t\r\n");
        if (std::string::npos == first) return 0.0f;
        size_t last = s.find_last_not_of(" \t\r\n");
        std::string trimmed = s.substr(first, (last - first + 1));

        float val = std::stof(trimmed);
        float finalVal = val;

        if (trimmed.find('%') != std::string::npos) {
            finalVal = val / 100.0f;
        }

        // DEBUG LUÔN Ở ĐÂY ĐỂ BIẾT TẠI SAO HÌNH SAI
        if (val < 0 || finalVal > 1.0f) {
            std::cout << "[PARSER DEBUG] String: '" << trimmed << "' -> Float: " << finalVal << std::endl;
        }

        return finalVal;
    }
    catch (...) {
        std::cout << "[PARSER ERROR] Failed to parse: " << (attr ? attr : "NULL") << std::endl;
        return 0.0f;
    }
}

void SVGParser::parseGradientStops(SVGGradient* grad, tinyxml2::XMLElement* node) {
    for (tinyxml2::XMLElement* child = node->FirstChildElement("stop"); child; child = child->NextSiblingElement("stop")) {
        float offset = 0.0f;
        const char* offsetAttr = child->Attribute("offset");
        if (offsetAttr) {
            std::string s = offsetAttr;
            size_t pos = 0;
            // Dùng hàm parseFloatToken đã sửa để đọc số nén hoặc %
            if (parseFloatToken(s, pos, offset)) {
                // Nếu parseFloatToken chưa xử lý chia 100 cho %, hãy check ở đây
                if (s.find('%') != std::string::npos && offset > 1.0f) offset /= 100.0f;
            }
        }

        CustomColor color(0, 0, 0);
        float opacity = 1.0f;
        const char* stopColorAttr = child->Attribute("stop-color");
        if (stopColorAttr) color = CustomColor::fromStringToCustomColor(stopColorAttr);

        const char* stopOpacityAttr = child->Attribute("stop-opacity");
        if (stopOpacityAttr) opacity = std::stof(stopOpacityAttr);

        grad->addStop(SVGStop(color, offset, opacity));
    }
}

void SVGParser::parseGradient(SVGGradient* grad, tinyxml2::XMLElement* node) {
    if (!grad || !node) return;

    // Xử lý ID
    const char* idAttr = node->Attribute("id");
    if (idAttr) {
        grad->setGradientID(idAttr);
    }

    // Xử lý GradientUnits
    const char* unitsAttr = node->Attribute("gradientUnits");
    if (unitsAttr) {
        grad->setGradientUnits(unitsAttr);
    }

    // Xử lý SpreadMethod
    const char* spreadAttr = node->Attribute("spreadMethod");
    if (spreadAttr) {
        grad->setSpreadMethod(spreadAttr);
    }

    // add
    const char* transformAttr = node->Attribute("gradientTransform");
    if (transformAttr) {
        // Gọi hàm đã được định nghĩa trong lớp
        
        //grad->setGradientTransform(SVGParser::parseTransform(transformAttr));

        //SVGTransform gradientTransform; // Tạo mới hoàn toàn
        //// Hoặc nếu lấy cũ thì phải: gradientTransform.clear(); 
        //parseTransform(gradientTransform, transformAttr);
        //grad->setGradientTransform(gradientTransform);

        //SVGTransform gradientTransform = grad->getGradientTransform();
        //parseTransform(gradientTransform, transformAttr);
        //grad->setGradientTransform(gradientTransform);

        SVGTransform gradientTransform = parseTransform(transformAttr);
        grad->setGradientTransform(gradientTransform);
    }

    grad->clearStops();

    parseGradientStops(grad, node);
}


void SVGParser::parseLinearGradient(SVGLinearGradient* grad, tinyxml2::XMLElement* node) {
    if (!grad || !node) return;

    // Sử dụng parseFloatValue để xử lý x1, y1, x2, y2 (hỗ trợ %)
    grad->setX1(parseFloatValue(node->Attribute("x1")));
    grad->setY1(parseFloatValue(node->Attribute("y1")));

    // x2 mặc định thường là 1 (100%) nếu không khai báo
    const char* x2Attr = node->Attribute("x2");
    grad->setX2(x2Attr ? parseFloatValue(x2Attr) : 1.0f);

    grad->setY2(parseFloatValue(node->Attribute("y2")));

    // Xử lý href/xlink:href
    const char* href = node->Attribute("href");
    if (!href) href = node->Attribute("xlink:href");
    if (href) grad->setHrefID(href);
}

void SVGParser::parseRadialGradient(SVGRadialGradient* grad, tinyxml2::XMLElement* node) {
    if (!grad || !node) return;

    // SỬA: Đọc đúng "cx" và "cy" thay vì "x1", "y1"
    grad->setCX(node->Attribute("cx") ? parseFloatValue(node->Attribute("cx")) : 0.5f);
    grad->setCY(node->Attribute("cy") ? parseFloatValue(node->Attribute("cy")) : 0.5f);
    grad->setR(node->Attribute("r") ? parseFloatValue(node->Attribute("r")) : 0.5f);

    const char* fxAttr = node->Attribute("fx");
    const char* fyAttr = node->Attribute("fy");
    grad->setFX(fxAttr ? parseFloatValue(fxAttr) : grad->getCX());
    grad->setFY(fyAttr ? parseFloatValue(fyAttr) : grad->getCY());

    const char* href = node->Attribute("href") ? node->Attribute("href") : node->Attribute("xlink:href");
    if (href) grad->setHrefID(href);
}
Gdiplus::Matrix* parse(const std::string& str)
{
    //SVGTransform t;
    Gdiplus::Matrix* t = new Gdiplus::Matrix();
    std::string s = str;

    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !isspace(ch);
        }));

    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());

    size_t pos = 0;
    while (pos < s.size()) {
        if (s.compare(pos, 9, "translate") == 0) {
            pos += 9;
            size_t l = s.find('(', pos);
            size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;

            std::string inside = s.substr(l + 1, r - l - 1);
            auto nums = parseNumbers(inside);
            float tx = nums.size() > 0 ? nums[0] : 0.0f;
            float ty = nums.size() > 1 ? nums[1] : 0.0f;
            t->Translate(tx, ty);
            pos = r + 1;
        }
        else if (s.compare(pos, 6, "rotate") == 0) {
            pos += 6;
            size_t l = s.find('(', pos);
            size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;

            std::string inside = s.substr(l + 1, r - l - 1);
            auto nums = parseNumbers(inside);
            float deg = nums.size() > 0 ? nums[0] : 0.0f;
            t->Rotate(deg);
            pos = r + 1;
        }
        else if (s.compare(pos, 5, "scale") == 0) {
            pos += 5;
            size_t l = s.find('(', pos);
            size_t r = s.find(')', l + 1);
            if (l == std::string::npos || r == std::string::npos) break;

            std::string inside = s.substr(l + 1, r - l - 1);
            auto nums = parseNumbers(inside);
            float sx = nums.size() > 0 ? nums[0] : 1.0f;
            float sy = nums.size() > 1 ? nums[1] : sx;
            t->Scale(sx, sy);
            pos = r + 1;
        }
        else {
            ++pos;
        }
    }

    return t;
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

//static std::vector<float>parseNumbers(const std::string& s)
//{
//	std::vector<float> nums;
//	std::stringstream ss(s);
//	std::string token;
//
//	char c;
//	std::string cur;
//	for (char ch : s) {
//		if (ch == ',' || isspace((unsigned char)ch)) {
//			if (!cur.empty()) {
//				nums.push_back(stof(cur));
//				cur.clear();
//			}
//		}
//		else {
//			cur.push_back(ch);
//		}
//	}
//
//	if (!cur.empty()) {
//		nums.push_back(stof(cur));
//	}
//	return nums;
//}
//

SVGTransform SVGParser::parseTransform(const string& str){
	SVGTransform t;
	string s = str;

	s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !isspace(ch);
		}));

	s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());

	size_t pos = 0;
	while (pos < s.size()) {
		if (s.compare(pos, 9, "translate") == 0) {
			pos += 9;
			size_t l = s.find('(', pos);
			size_t r = s.find(')', l + 1);
			if (l == string::npos || r == string::npos) break;

			string inside = s.substr(l + 1, r - l - 1);
			auto nums = parseNumbers(inside);
			float tx = nums.size() > 0 ? nums[0] : 0.0f;
			float ty = nums.size() > 1 ? nums[1] : 0.0f;
			t.addTranslate(tx, ty);
			pos = r + 1;
		}
		else if (s.compare(pos, 6, "rotate") == 0) {
			pos += 6;
			size_t l = s.find('(', pos);
			size_t r = s.find(')', l + 1);
			if (l == string::npos || r == string::npos) break;

			string inside = s.substr(l + 1, r - l - 1);
			auto nums = parseNumbers(inside);
			float deg = nums.size() > 0 ? nums[0] : 0.0f;
			t.addRotate(deg);
			pos = r + 1;
		}
		else if (s.compare(pos, 5, "scale") == 0) {
			pos += 5;
			size_t l = s.find('(', pos);
			size_t r = s.find(')', l + 1);
			if (l == string::npos || r == string::npos) break;

			string inside = s.substr(l + 1, r - l - 1);
			auto nums = parseNumbers(inside);
			float sx = nums.size() > 0 ? nums[0] : 1.0f;
			float sy = nums.size() > 1 ? nums[1] : sx;
			t.addScale(sx, sy);
			pos = r + 1;
		}
		else {
			++pos;
		}
	}

	return t;
}
void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
    // 1. Lấy đối tượng style hiện tại (sao chép giá trị từ element)
    // Lưu ý: currentStyle là một bản sao giá trị, sửa đổi nó không làm thay đổi element cho đến khi setSVGStyle được gọi.
    //SVGStyle currentStyle = element->getSVGStyle();
    SVGStyle& currentStyle = element->getSVGStyle();

    // --- KHẮC PHỤC LỖI NULL STROKE: KHỞI TẠO NẾU CẦN ---
    // Kiểm tra xem có bất kỳ thuộc tính Stroke nào tồn tại không
    const tinyxml2::XMLAttribute* strokeAttr = xmlNode->FindAttribute("stroke");
    const tinyxml2::XMLAttribute* widthAttr = xmlNode->FindAttribute("stroke-width");
    const tinyxml2::XMLAttribute* opacityAttr = xmlNode->FindAttribute("stroke-opacity");

    // Nếu có bất kỳ thuộc tính stroke nào VÀ stroke chưa được khởi tạo
    if ((strokeAttr || widthAttr || opacityAttr) && currentStyle.getStroke() == nullptr) {
        // TẠO VÀ GÁN ĐỐI TƯỢNG STROKE MỚI HỢP LỆ
        Stroke tempStroke;
        currentStyle.setStroke(&tempStroke);
        // Sau lệnh này, currentStyle.getStroke() KHÔNG BAO GIỜ LÀ nullptr.
    }
    // ----------------------------------------------------

    // 2. Vòng lặp phân tích cú pháp tất cả các thuộc tính
    const tinyxml2::XMLAttribute* attr = xmlNode->FirstAttribute();
    while (attr) {
        std::string name = attr->Name();
        std::string value = attr->Value();

        if (name == "id") {
            element->setId(value);
        }
        if (name == "style") {
            // Xử lý style tổng hợp: Gọi parseStyle để áp dụng nhiều thuộc tính cùng lúc
            // Không cần tạo đối tượng style mới, chỉ cần sửa đổi currentStyle
            this->parseStyle(currentStyle, value);
        }
        // Xử lý Fill riêng lẻ
        if (name == "fill") {
            if (value == "none" || value == "transparent") {
                // None
                currentStyle.setGradId("");
                currentStyle.setFillOpacity(0.0f);
            }
            else {
                CustomColor c; float alpha = 1.0f; bool hasAlpha = false;
                parseColorString(value, c, alpha, hasAlpha);
                currentStyle.setFillColor(c);
                if (hasAlpha) currentStyle.setFillOpacity(alpha);
                else if (currentStyle.getFillOpacity() <= 0.0f) currentStyle.setFillOpacity(1.0f);
                if (value.size() >= 4 && value.substr(0, 4) == "url(") {
                    // Gradient
                    size_t start = value.find_first_of('#');
                    size_t end = value.find_last_of(')');
                    if (start != std::string::npos && end != std::string::npos && end > start) {
                        std::string idWithHash = value.substr(start, end - start);
                        currentStyle.setGradId(idWithHash);
                        currentStyle.setGrad(nullptr);
                    }
                    else {
                        currentStyle.setGradId("");
                        currentStyle.setFillColor(CustomColor(0, 0, 0));
                    }
                }
                else {
                    // Màu đơn
                    currentStyle.setGradId("");
                    currentStyle.setFillColor(CustomColor::fromStringToCustomColor(value));
                }
            }
        }
        if (name == "fill-opacity") {
            try { currentStyle.setFillOpacity(std::stof(value)); }
            catch (...) {}
        }

        if (name == "fill-rule") {
            if (value == "nonzero")
                currentStyle.setFillRule(FillRule::NonZero);
            else if (value == "evenodd")
                currentStyle.setFillRule(FillRule::EvenOdd);
        }
        // Xử lý Stroke riêng lẻ (sau khi đã đảm bảo Stroke tồn tại)
        if (name == "stroke" || name == "stroke-width" || name == "stroke-opacity" || name == "stroke-miterlimit") {
            Stroke* stroke = currentStyle.getStroke();
            if (!stroke) { Stroke temp; currentStyle.setStroke(&temp); stroke = currentStyle.getStroke(); }
            if (stroke) {
                if (name == "stroke") {
                    stroke->strokeColor = CustomColor::fromStringToCustomColor(value);
                    // >>> LOGIC MỚI: Phân loại URL (Gradient) và Color (Đơn) <<<
                    if (value.size() >= 4 && value.substr(0, 4) == "url(") {
                        // TRƯỜNG HỢP 1: Gradient
                        size_t start = value.find_first_of('#');
                        size_t end = value.find_last_of(')');
                        if (start != std::string::npos && end != std::string::npos && end > start) {
                            std::string idWithHash = value.substr(start, end - start);
                            // Gán ID Gradient cho Stroke
                            stroke->strokeGradId = idWithHash;
                            stroke->strokeGrad = nullptr; // Đảm bảo reset con trỏ
                            // Cần thêm logic để xử lý nếu stroke="none" trong hàm này.
                        }
                        else {
                            // Lỗi cú pháp URL, reset GradId
                            stroke->strokeGradId = "";
                        }
                    }
                    else if (value == "none" || value == "transparent") {
                        // TRƯỜNG HỢP 2: None
                        stroke->strokeGradId = "";
                        stroke->strokeColor = CustomColor(0, 0, 0); // Màu đen
                        stroke->strokeOpacity = 0.0f; // Mất viền
                    }
                    else {
                        // TRƯỜNG HỢP 3: Màu đơn
                        stroke->strokeGradId = ""; // Đảm bảo reset GradId
                        stroke->strokeColor = CustomColor::fromStringToCustomColor(value);
                    }
                }
                else if (name == "stroke-width") {
                    try {
                        stroke->strokeWidth = std::stof(value);
                    }
                    catch (const std::exception& e) {
                        stroke->strokeWidth = 1.0f; // Giá trị mặc định an toàn
                    }
                }
                else if (name == "stroke-opacity") {
                    try {
                        stroke->strokeOpacity = std::stof(value);
                    }
                    catch (const std::exception& e) {
                        stroke->strokeOpacity = 1.0f; // Giá trị mặc định an toàn
                    }
                }
                else if (name == "stroke-miterlimit") {
                    try {
                        stroke->setStrokeMiterLimit(std::stof(value));
                    }
                    catch (...) {
                        stroke->setStrokeMiterLimit(10.0f); // Mặc định SVG
                    }
                }
            }
        }
        // 4. Cập nhật thuộc tính Stroke (với xử lý ngoại lệ an toàn)
        Stroke* stroke = currentStyle.getStroke();


        attr = attr->Next();
    }
    const char* transformStr = xmlNode->Attribute("transform");
    if (transformStr) {
        //Gdiplus::Matrix* t = parse(transformStr);
        //element->setTransformMatrix(t);
        //element->transformMatrix = new Gdiplus::Matrix(t)

        //SVGTransform transform = element->getTransform();
        //parseTransform(transform, transformStr);

        //SVGTransform t = SVGTransform::parse(transformStr);
        //element->setTransform(t);

        SVGTransform t = parseTransform(transformStr);
        element->getSVGStyle().setTransform(t);
    }
    // 3. Gán lại style sau khi tất cả các thuộc tính đã được phân tích cú pháp
    element->setSVGStyle(currentStyle);
}

void SVGParser::parseStyle(SVGStyle& style, const std::string& styleStr) {
    // 1. Phân tách chuỗi style bằng dấu chấm phẩy (;)
    std::stringstream ss(styleStr);
    std::string item;

    // Sử dụng std::remove_if từ <algorithm>
    while (std::getline(ss, item, ';')) {
        // Loại bỏ tất cả khoảng trắng (whitespace) trong item
        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.empty()) continue; // Bỏ qua nếu chuỗi rỗng

        // Tìm dấu hai chấm (:) để phân tách key và value
        size_t pos = item.find(':');
        if (pos == std::string::npos) continue;

        std::string key = item.substr(0, pos);
        std::string value = item.substr(pos + 1);

        // 2. Xử lý các thuộc tính Fill
        if (key == "fill") {
            // Trường hợp 1: Gradient (url(...))
            if (value.size() >= 4 && value.substr(0, 4) == "url(") {

                size_t start = value.find_first_of('#');
                size_t end = value.find_last_of(')');

                if (start != std::string::npos && end != std::string::npos && end > start) {
                    std::string idWithHash = value.substr(start, end - start);
                    style.setGradId(idWithHash);
                    style.setGrad(nullptr); // Đảm bảo con trỏ thực tế chưa được gán
                }
            }
            // Trường hợp 2: None
            else if (value == "none" || value == "transparent") {
                style.setGradId("");
                style.setFillOpacity(0.0f);
                style.setFillColor(CustomColor(0, 0, 0));
            }
            // Trường hợp 3: Màu đơn (RGB/Hex/Name)
            else {
                style.setGradId("");
                style.setFillColor(CustomColor::fromStringToCustomColor(value));
            }
        }
        else if (key == "fill-opacity") {
            try {
                style.setFillOpacity(std::stof(value));
            }
            catch (const std::exception& e) {
                // Xử lý ngoại lệ nếu giá trị không hợp lệ
                std::cerr << "Style parsing error (fill-opacity): " << e.what() << std::endl;
            }
        }
        else if (key == "fill-rule") {
            if (value == "nonzero")
                style.setFillRule(FillRule::NonZero);
            else if (value == "evenodd")
                style.setFillRule(FillRule::EvenOdd);
        }
        // 3. Xử lý các thuộc tính Stroke
        else if (key.find("stroke") != std::string::npos) {

            // Xử lý Lỗi Con trỏ Null: Đảm bảo Stroke được cấp phát trước khi sửa đổi
            Stroke* currentStroke = style.getStroke();

            if (!currentStroke) {
                // Nếu chưa có, tạo đối tượng Stroke mới và gán nó cho SVGStyle
                Stroke tempStroke;

                // setStroke sẽ tạo bản sao (deep copy) của tempStroke
                style.setStroke(&tempStroke);

                // Lấy lại con trỏ nội bộ mới (đã được cấp phát hợp lệ)
                currentStroke = style.getStroke();
            }

            // Sửa đổi TRỰC TIẾP đối tượng Stroke hiện tại
            if (currentStroke) {
                // Dùng hàm parseStroke để xử lý thuộc tính (key và item/value)
                // Ví dụ: parseStroke cần được thiết kế để xử lý cặp key:value
                // Nếu parseStroke được thiết kế để xử lý item:
                parseStroke(*currentStroke, item);
            }
        }
    }
}

void SVGParser::parseStroke(Stroke& stroke, const std::string& styleStr) {
    // 1. Tạo bản sao của chuỗi style để xử lý
    std::string s = styleStr;

    // Yêu cầu #include <algorithm> và std::remove_if
    // Xóa tất cả các khoảng trắng (whitespace) trong chuỗi style
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    // 2. Tìm vị trí của dấu hai chấm (:)
    size_t pos = s.find(':'); // <<< ĐÂY LÀ VỊ TRÍ CỦA DẤU HAI CHẤM

    // Nếu không tìm thấy dấu hai chấm, kết thúc hàm
    if (pos == std::string::npos) return;

    // 3. Phân tách Key và Value
    // Key: Từ đầu chuỗi đến vị trí dấu hai chấm
    std::string key = s.substr(0, pos);
    // Value: Từ vị trí sau dấu hai chấm đến hết chuỗi
    std::string value = s.substr(pos + 1);

    // 4. Cập nhật thuộc tính Stroke (với xử lý ngoại lệ an toàn)
    if (key == "stroke") {
        // >>> LOGIC MỚI: Phân loại URL (Gradient) và Color (Đơn) <<<
        if (value.size() >= 4 && value.substr(0, 4) == "url(") {
            // TRƯỜNG HỢP 1: Gradient
            size_t start = value.find_first_of('#');
            size_t end = value.find_last_of(')');

            if (start != std::string::npos && end != std::string::npos && end > start) {
                std::string idWithHash = value.substr(start, end - start);
                // Gán ID Gradient cho Stroke
                stroke.strokeGradId = idWithHash;
                stroke.strokeGrad = nullptr; // Đảm bảo reset con trỏ

                // Cần thêm logic để xử lý nếu stroke="none" trong hàm này.
            }
            else {
                // Lỗi cú pháp URL, reset GradId
                stroke.strokeGradId = "";
            }
        }
        else if (value == "none" || value == "transparent") {
            // TRƯỜNG HỢP 2: None
            stroke.strokeGradId = "";
            stroke.strokeColor = CustomColor(0, 0, 0); // Màu đen
            stroke.strokeOpacity = 0.0f; // Mất viền
        }
        else {
            // TRƯỜNG HỢP 3: Màu đơn
            stroke.strokeGradId = ""; // Đảm bảo reset GradId
            stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
        }
    }
    else if (key == "stroke-width") {
        try {
            stroke.strokeWidth = std::stof(value);
        }
        catch (const std::exception& e) {
            stroke.strokeWidth = 1.0f; // Giá trị mặc định an toàn
        }
    }
    else if (key == "stroke-opacity") {
        try {
            stroke.strokeOpacity = std::stof(value);
        }
        catch (const std::exception& e) {
            stroke.strokeOpacity = 1.0f; // Giá trị mặc định an toàn
        }
    }
    else if (key == "stroke-miterlimit") {
        try {
            stroke.setStrokeMiterLimit(std::stof(value));
        }
        catch (...) {
            stroke.setStrokeMiterLimit(10.0f); // Mặc định SVG
        }
    }
}

void SVGParser::parseRectangle(SVGRectangle* rect, tinyxml2::XMLElement* Node) {
    float x = Node->FloatAttribute("x");
    float y = Node->FloatAttribute("y");
    float w = Node->FloatAttribute("width");
    float h = Node->FloatAttribute("height");
    rect->setTopLeftCorner(CustomPoint(x, y));
    rect->setWidth(w);
    rect->setHeight(h);
}

void SVGParser::parseSquare(SVGSquare* sq, tinyxml2::XMLElement* Node) {
    float x = Node->FloatAttribute("x");
    float y = Node->FloatAttribute("y");
    float size = Node->FloatAttribute("width"); // SVG square dùng width = height
    sq->setTopLeftCorner(CustomPoint(x, y));
    sq->setWidth(size);
    sq->setHeight(size);
}

void SVGParser::parseEllipse(SVGEllipse* el, tinyxml2::XMLElement* Node) {
    float cx = Node->FloatAttribute("cx");
    float cy = Node->FloatAttribute("cy");
    float rx = Node->FloatAttribute("rx");
    float ry = Node->FloatAttribute("ry");
    el->setCenter(CustomPoint(cx, cy));
    el->setRadiusX(rx);
    el->setRadiusY(ry);
}

void SVGParser::parseCircle(SVGCircle* circle, tinyxml2::XMLElement* Node) {
    float cx = Node->FloatAttribute("cx");
    float cy = Node->FloatAttribute("cy");
    float r = Node->FloatAttribute("r");
    circle->setCenter(CustomPoint(cx, cy));
    circle->setRadius(r);
}

void SVGParser::parseLine(SVGLine* line, tinyxml2::XMLElement* Node) {
    float x1 = Node->FloatAttribute("x1");
    float y1 = Node->FloatAttribute("y1");
    float x2 = Node->FloatAttribute("x2");
    float y2 = Node->FloatAttribute("y2");
    line->setStartPoint(CustomPoint(x1, y1));
    line->setEndPoint(CustomPoint(x2, y2));
}

//void SVGParser::parsePolyshape(SVGPolyshapeBase* poly, tinyxml2::XMLElement* Node) {
//    const char* pointsStr = Node->Attribute("points");
//    if (!pointsStr) return;
//    std::vector<CustomPoint> pts;
//    std::stringstream ss(pointsStr);
//    std::string token;
//    while (getline(ss, token, ' ')) {
//        if (token.empty()) continue;
//        size_t comma = token.find(',');
//        if (comma == std::string::npos) continue;
//        float x = stof(token.substr(0, comma));
//        float y = stof(token.substr(comma + 1));
//        pts.emplace_back(x, y);
//    }
//    poly->setPoints(pts);
//}

void SVGParser::parsePolyshape(SVGPolyshapeBase* poly, tinyxml2::XMLElement* Node) {
    const char* pointsStr = Node->Attribute("points");
    if (!pointsStr) return;

    std::vector<CustomPoint> pts;
    std::string pointsString(pointsStr);

    // 1. CHUẨN HÓA: Thay thế dấu phẩy bằng khoảng trắng
    // Điều này cho phép chúng ta xử lý cả định dạng "x,y x,y" và "x y x y"
    std::replace(pointsString.begin(), pointsString.end(), ',', ' ');

    // 2. TÁCH TOKEN VÀ GHÉP CẶP
    std::stringstream ss(pointsString);
    std::string token;

    float x = 0.0f;
    bool readingX = true;

    // ss >> token tự động bỏ qua khoảng trắng thừa
    while (ss >> token) {
        try {
            float value = std::stof(token);

            if (readingX) {
                x = value;
                // Sẵn sàng đọc tọa độ Y
                readingX = false;
            }
            else {
                // Đã đọc xong Y, tạo điểm và sẵn sàng đọc X tiếp theo
                pts.emplace_back(x, value);
                readingX = true;
            }
        }
        catch (const std::exception& e) {
            // Xử lý lỗi: Nếu không thể chuyển token thành float, bỏ qua
            // Nếu đang chờ Y, chúng ta reset để không làm hỏng cặp tiếp theo
            if (!readingX) {
                readingX = true;
            }
        }
    }

    // Gán tập hợp điểm đã phân tích cho đối tượng
    poly->setPoints(pts);
}

//void SVGParser::parseText(SVGText* text, tinyxml2::XMLElement* Node) {
//    float x = Node->FloatAttribute("x");
//    float y = Node->FloatAttribute("y");
//    const char* content = Node->GetText();
//    text->setStart(CustomPoint(x, y));
//    //const char* content_char = Node->GetText();
//    // Chuyển đổi char* sang wstring để lưu trữ
//    float fontSize = Node->FloatAttribute("font-size");
//    if (fontSize <= 0.0f) fontSize = 12.0f;
//    text->setFontSize(fontSize);
//
//    const char* content_char = Node->GetText();
//    /*if (content_char) {
//        std::string content_str(content_char);
//
//        text->setContent(std::wstring(content_str.begin(), content_str.end()));
//
//    }*/
//    if (!content_char && Node->FirstChild()) {
//        auto child = Node->FirstChild();
//        if (child->ToText()) {
//            content_char = child->Value();
//        }
//    }
//
//    if (content_char) {
//        // 1. Convert sang WideString
//        int size_needed = MultiByteToWideChar(CP_UTF8, 0, content_char, -1, NULL, 0);
//        std::wstring ws(size_needed, 0);
//        MultiByteToWideChar(CP_UTF8, 0, content_char, -1, &ws[0], size_needed);
//        if (!ws.empty() && ws.back() == L'\0') ws.pop_back();
//
//        // 2. QUAN TRỌNG: Thay thế xuống dòng/tab thành dấu cách
//        // Để "sleep all \n day" thành "sleep all day"
//        std::replace(ws.begin(), ws.end(), L'\n', L' ');
//        std::replace(ws.begin(), ws.end(), L'\r', L' ');
//        std::replace(ws.begin(), ws.end(), L'\t', L' ');
//
//        // (Tùy chọn) Xóa khoảng trắng thừa kép nếu cần, nhưng replace đơn giản là đủ
//        text->setContent(ws);
//    }
//    else {
//
//        text->setContent(L"");
//
//    }
//}
void SVGParser::parseText(SVGText* text, tinyxml2::XMLElement* Node, const SVGGroup* parentGroup) {
    if (!text || !Node) return;

    // --- 1. XỬ LÝ CÁC THUỘC TÍNH CHUNG (ỦY QUYỀN CHO parseAttributes) ---
    // Điều này sẽ xử lý: fill, stroke, transform, id, style
    if (parentGroup) {
        const auto& inheritedMap = parentGroup->getInheritedAttributes();

        // Kế thừa Font Size
        if (inheritedMap.count("font-size")) {
            try {
                float inheritedSize = std::stof(inheritedMap.at("font-size"));
                if (inheritedSize > 0.0f) {
                    text->setFontSize(inheritedSize);
                }
            }
            catch (...) {}
        }

        // Kế thừa Font Family
        if (inheritedMap.count("font-family")) {
            text->setFontFamily(inheritedMap.at("font-family"));
        }

        // Kế thừa Text Anchor
        if (inheritedMap.count("text-anchor")) {
            text->setTextAnchor(this->parseTextAnchor(inheritedMap.at("text-anchor")));
        }

        // Kế thừa Font Style
        if (inheritedMap.count("font-style")) {
            text->setFontStyle(this->parseFontStyle(inheritedMap.at("font-style")));
        }

        // Kế thừa Font Weight
        if (inheritedMap.count("font-weight")) {
            text->setFontWeight(this->parseFontWeight(inheritedMap.at("font-weight")));
        }
    }

    // --- 2. XỬ LÝ CÁC THUỘC TÍNH RIÊNG CỦA TEXT ---

    // A. Lấy và gán Vị trí (x, y)
    float x = Node->FloatAttribute("x", 0.0f);
    float y = Node->FloatAttribute("y", 0.0f);
    text->setStart(CustomPoint(x, y));

    // B. Lấy và gán các thuộc tính Font/Text khác
    const tinyxml2::XMLAttribute* attr = Node->FirstAttribute();
    while (attr) {
        std::string name = attr->Name();
        std::string value = attr->Value();

        if (name == "font-size") {
            try {
                float fontSize = std::stof(value);
                if (fontSize > 0.0f) {
                    text->setFontSize(fontSize);
                }
            }
            catch (...) {}
        }
        else if (name == "font-family") {
            std::string fullValue = value; // value = "Times New Roman,Times New Roman_MSFontService,sans-serif"

            // Tìm dấu phẩy đầu tiên
            size_t commaPos = fullValue.find(',');
            if (commaPos != std::string::npos) {
                // Cắt chuỗi chỉ lấy tên font đầu tiên
                fullValue = fullValue.substr(0, commaPos);
            }

            // Loại bỏ khoảng trắng thừa (nếu có, ví dụ: "Times New Roman ,Times...")
            // (Optional nhưng nên có)
            fullValue.erase(std::remove_if(fullValue.begin(), fullValue.end(), ::isspace), fullValue.end());

            text->setFontFamily(fullValue); // Kết quả là "Times New Roman"
        }
        else if (name == "text-anchor") {
            // Giả định hàm parseTextAnchor đã được định nghĩa trong SVGParser
            text->setTextAnchor(this->parseTextAnchor(value));
        }
        else if (name == "font-style") {
            text->setFontStyle(this->parseFontStyle(value));
        }
        else if (name == "font-weight") {
            text->setFontWeight(this->parseFontWeight(value));
        }
        else if (name == "dx") {
            float dx_val = std::stof(value);
            text->setDx(dx_val);
        }
        else if (name == "dy") {
            float dy_val = std::stof(value);
            text->setDy(dy_val);
        }

        attr = attr->Next();
    }

    // Nếu font-size chưa được thiết lập (hoặc giá trị không hợp lệ), đặt mặc định 12.0f
    if (text->getFontSize() <= 0.0f) {
        text->setFontSize(12.0f);
    }

    // C. XỬ LÝ NỘI DUNG VĂN BẢN (CONTENT)

    const char* content_char = Node->GetText(); // Cố gắng lấy nội dung Text node con

    // Nếu Node->GetText() không tìm thấy (do có thẻ con hoặc comment), kiểm tra Node con
    if (!content_char) {
        auto child = Node->FirstChild();
        if (child && child->ToText()) {
            content_char = child->Value();
        }
    }

    if (content_char) {
        // 1. Chuyển đổi từ UTF-8 (char*) sang WideString (wchar_t / std::wstring)
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, content_char, -1, NULL, 0);
        std::wstring ws(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, content_char, -1, &ws[0], size_needed);

        // Loại bỏ ký tự null thừa nếu có
        if (!ws.empty() && ws.back() == L'\0') ws.pop_back();

        // 2. Chuẩn hóa khoảng trắng theo quy tắc SVG/XML: thay thế newline/tab bằng dấu cách
        std::replace(ws.begin(), ws.end(), L'\n', L' ');
        std::replace(ws.begin(), ws.end(), L'\r', L' ');
        std::replace(ws.begin(), ws.end(), L'\t', L' ');

        // (Tùy chọn) Loại bỏ các khoảng trắng kép nếu cần (ví dụ: "a  b" thành "a b")
        // Tuy nhiên, đối với SVG text, việc này không bắt buộc trừ khi dùng text-spacing=normal.

        text->setContent(ws);
    }
    else {
        text->setContent(L"");
    }
}
//void SVGParser::parsePath(SVGPath* path, tinyxml2::XMLElement* Node) {
//    if (!path || !Node) return;
//    //const char* dAttr = Node->Attribute("d");
//    //if (dAttr) {
//    //    path->setPathData(dAttr);
//    //}
//    // 1. Đọc thuộc tính Path Data (d)
//    const char* dAttr = Node->Attribute("d");
//    if (dAttr) {
//        path->setPathData(dAttr);
//        // setPathData sẽ tự động gọi path->parsePathData()
//    }

//    // 2. Các thuộc tính khác của Path (ví dụ: Path Length, nếu cần)
//    // Hiện tại chỉ cần d.
//}

//void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* node) {
//    const char* dAttr = node->Attribute("d");
//    if (dAttr) {
//        setPathData(dAttr);
//    }

//    // Parse transform
//    const char* tAttr = node->Attribute("transform");
//    if (tAttr) {
//        setTransform(tAttr);
//    }
//}
//void SVGParser::parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node) {
//    if (!group || !Node) return;
//
//    // --- 1. Xử lý thuộc tính Transform (Áp dụng cho cả <svg> và <g>) ---
//
//    //const char* transformStr = Node->Attribute("transform");
//    //if (transformStr) {
//    //    // Cần một hàm hỗ trợ trong SVGParser để phân tích chuỗi transform
//    //    // và trả về Gdiplus::Matrix*
//    //    Gdiplus::Matrix* matrix = parseTransform(transformStr);
//
//    //    if (matrix) {
//    //        // Giả định SVGGroup (hoặc SVGElement) có setTransform(Gdiplus::Matrix*)
//    //        group->setTransform(matrix);
//    //    }
//    //}
//
//    // --- 2. Xử lý thuộc tính đặc biệt của <svg> (width, height, viewBox) ---
//
//    if (std::string(Node->Name()) == "svg") {
//
//        // Đọc Width
//        float w = Node->FloatAttribute("width");
//        if (w <= 0.0f) { w = 300.0f; } // Mặc định
//        group->setViewBoxW(w);
//
//        // Đọc Height
//        float h = Node->FloatAttribute("height");
//        if (h <= 0.0f) { h = 150.0f; } // Mặc định
//        group->setViewBoxH(h);
//        if (group->getViewBoxW() <= 0.0f || group->getViewBoxH() <= 0.0f) {
//            // Đảm bảo W/H ít nhất là 1 để tránh chia cho 0
//            group->setViewBoxW(1000.0f);
//            group->setViewBoxH(500.0f);
//        }
//        // Đọc ViewBox
//        const char* viewBoxStr = Node->Attribute("viewBox");
//        if (viewBoxStr) {
//            group->setViewBox(std::string(viewBoxStr));
//        }
//        else {
//            // Gán ViewBox mặc định bằng Kích thước (nếu không có viewBox)
//            group->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
//        }
//    }
//}
//void SVGParser::parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node) {
//    if (!group || !Node) return;
//
//    // --- 1. XỬ LÝ CÁC THUỘC TÍNH CHUNG (fill, stroke, transform, id, style) ---
//
//    // Gọi hàm parseAttributes để xử lý thuộc tính chung cho <g> và <svg>
//    this->parseAttributes(Node, group);
//
//    // ----------------------------------------------------------------------
//    // --- 2. XỬ LÝ CÁC THUỘC TÍNH ĐẶC BIỆT CỦA <svg> (Root Group) ---
//    // ----------------------------------------------------------------------
//
//    if (std::string(Node->Name()) == "svg") {
//
//        // A. Đọc thuộc tính Width và Height (Kích thước Viewport/Vật lý)
//
//        // Tên thuộc tính: width
//        const char* widthAttr = Node->Attribute("width");
//        float w = 0.0f;
//        if (widthAttr) {
//            try { w = std::stof(widthAttr); }
//            catch (...) {}
//        }
//
//        // Tên thuộc tính: height
//        const char* heightAttr = Node->Attribute("height");
//        float h = 0.0f;
//        if (heightAttr) {
//            try { h = std::stof(heightAttr); }
//            catch (...) {}
//        }
//
//        // LƯU Ý: setWidth/setHeight LÀ KÍCH THƯỚC VẬT LÝ
//        // Bạn cần đảm bảo đã sử dụng đúng setter cho W/H vật lý.
//        // Giả sử các hàm setWidth/setHeight của bạn lưu kích thước vật lý (không phải ViewBox).
//        group->setWidth(w > 0.0f ? w : 300.0f); // Mặc định 300
//        group->setHeight(h > 0.0f ? h : 150.0f); // Mặc định 150
//
//
//        // B. XỬ LÝ VIEWBOX (Tọa độ Logic)
//
//        const char* viewBoxStr = Node->Attribute("viewBox");
//
//        if (viewBoxStr) {
//            // Nếu có viewBox, nó sẽ gán ViewBoxX/Y/W/H từ chuỗi
//            group->setViewBox(std::string(viewBoxStr));
//        }
//        else {
//            // Nếu không có viewBox, gán ViewBox bằng kích thước Vật lý/Viewport
//            // Điều này đảm bảo Viewport và Hệ tọa độ Logic khớp nhau.
//            float defaultW = group->getWidth();
//            float defaultH = group->getHeight();
//
//            std::string defaultViewBox = "0 0 " + std::to_string(defaultW) + " " + std::to_string(defaultH);
//            group->setViewBox(defaultViewBox); // Gọi setViewBox để phân tích cú pháp "0 0 W H"
//        }
//
//        // Đảm bảo W/H ViewBox lớn hơn 0 để tránh lỗi chia 0 khi render
//        if (group->getViewBoxW() <= 0.0f || group->getViewBoxH() <= 0.0f) {
//            group->setViewBoxW(1.0f);
//            group->setViewBoxH(1.0f);
//        }
//    }

void SVGParser::parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node) {
    if (!group || !Node) return;
    // Xử lí thuộc tính chung (đã gọi trong SVGElement)
    std::unordered_map<std::string, std::string>& inheritableMap = const_cast<std::unordered_map<std::string, std::string>&>(group->getInheritedAttributes());

    // Gọi hàm parse
    this->parseInheritableAttributes(Node, inheritableMap);
    // Xử lý các thuộc tính đặc biệt của <svg> (width, height, viewBox)
    if (std::string(Node->Name()) == "svg") {

        // --- 1. Đọc Width ---
        const char* wAttr = Node->Attribute("width");
        float w = 0.0f;
        if (wAttr) {
            try { w = std::stof(wAttr); }
            catch (...) {}
        }
        if (w <= 0.0f) { w = 300.0f; } // Mặc định SVG
        group->setWidth(w);

        // --- 2. Đọc Height ---
        const char* hAttr = Node->Attribute("height");
        float h = 0.0f;
        if (hAttr) {
            try { h = std::stof(hAttr); }
            catch (...) {}
        }
        if (h <= 0.0f) { h = 150.0f; } // Mặc định SVG
        group->setHeight(h);

        // --- 3. Đọc và Phân tích ViewBox ---
        const char* viewBoxStr = Node->Attribute("viewBox");
        if (viewBoxStr) {
            // Gọi hàm setter đã viết ở trên
            group->setViewBox(std::string(viewBoxStr));
        }
        else {
            // Nếu không có viewBox, mặc định ViewBox = (0, 0, width, height)
            std::string defaultViewBox = "0 0 " + std::to_string(w) + " " + std::to_string(h);
            group->setViewBox(defaultViewBox);
        }
    }
}
    // --- 3. ĐỆ QUY VÀO CÁC PHẦN TỬ CON ---

    //tinyxml2::XMLElement* child = Node->FirstChildElement();
    //while (child) {
    //    // Gọi Factory Pattern để tạo và parse phần tử con
    //    SVGElement* element = g_factory.createElement(child->Name());
    //    if (element) {
    //        // Gọi hàm parse cụ thể của element (ví dụ: SVGText::parse, SVGRectangle::parse)
    //        element->parse(*this, child);
    //        group->addElement(element);
    //    }

    //    child = child->NextSiblingElement();
    //}
// --- THAY ĐỔI 1: Hàm mới để parse các thuộc tính kế thừa vào Map ---
// Hàm này phải được khai báo trong SVGParser.h
void SVGParser::parseInheritableAttributes(tinyxml2::XMLElement* xmlNode,
    std::unordered_map<std::string, std::string>& inheritableMap) {
    // Định nghĩa các thuộc tính kiểu dáng/text có thể kế thừa (bổ sung ngoài fill/stroke)
    static const std::vector<std::string> INHERITABLE_ATTRIBUTES = {
        "font-size", "font-family", "text-anchor", "alignment-baseline"
    };

    const tinyxml2::XMLAttribute* attr = xmlNode->FirstAttribute();
    while (attr) {
        std::string name = attr->Name();

        // Chỉ lưu các thuộc tính thuộc danh sách kế thừa tùy chỉnh
        if (std::find(INHERITABLE_ATTRIBUTES.begin(), INHERITABLE_ATTRIBUTES.end(), name) != INHERITABLE_ATTRIBUTES.end()) {
            inheritableMap[name] = attr->Value();
        }
        attr = attr->Next();
    }
}

void SVGParser::parsePath(SVGPath* path, tinyxml2::XMLElement* Node) {
    const char* dAttr = Node->Attribute("d");
    if (!dAttr) return;

    std::string d = dAttr;
    size_t pos = 0;
    char cmd = 0;

    float currentX = 0.0f, currentY = 0.0f;
    float subPathStartX = 0.0f, subPathStartY = 0.0f; // Điểm bắt đầu của Sub-path (lệnh M gần nhất)
    float lastCtrlX = 0.0f, lastCtrlY = 0.0f;        // Lưu điểm điều khiển cho lệnh S

    while (pos < d.size()) {
        skipSpacesAndCommas(d, pos);
        if (pos >= d.size()) break;

        // Kiểm tra lệnh mới
        if (std::isalpha((unsigned char)d[pos])) {
            cmd = d[pos];
            pos++;
        }

        bool isRel = (cmd >= 'a' && cmd <= 'z'); // Lệnh chữ thường = tương đối
        char absCmd = (char)std::toupper((unsigned char)cmd);

        switch (absCmd) {
        case 'M': {
            float x, y;
            while (parseFloatToken(d, pos, x) && parseFloatToken(d, pos, y)) {
                if (isRel) { x += currentX; y += currentY; }
                path->setCommands({ PathCommandType::MoveTo, {x, y} });
                currentX = x; currentY = y;
                subPathStartX = x; subPathStartY = y; // QUAN TRỌNG: Cập nhật gốc Sub-path
                cmd = isRel ? 'l' : 'L'; // Implicit LineTo sau M
                skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'L': {
            float x, y;
            while (parseFloatToken(d, pos, x) && parseFloatToken(d, pos, y)) {
                if (isRel) { x += currentX; y += currentY; }
                path->setCommands({ PathCommandType::LineTo, {x, y} });
                currentX = x; currentY = y;
                lastCtrlX = currentX; lastCtrlY = currentY;
                skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'H': { // Horizontal Line
            float x;
            while (parseFloatToken(d, pos, x)) {
                if (isRel) x += currentX;
                path->setCommands({ PathCommandType::HLineTo, {x} });
                currentX = x;
                lastCtrlX = currentX; lastCtrlY = currentY;
                skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'V': { // Vertical Line
            float y;
            while (parseFloatToken(d, pos, y)) {
                if (isRel) y += currentY;
                path->setCommands({ PathCommandType::VLineTo, {y} });
                currentY = y;
                lastCtrlX = currentX; lastCtrlY = currentY;
                skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'C': { // Cubic Bezier
            float x1, y1, x2, y2, x, y;
            while (parseFloatToken(d, pos, x1) && parseFloatToken(d, pos, y1) &&
                parseFloatToken(d, pos, x2) && parseFloatToken(d, pos, y2) &&
                parseFloatToken(d, pos, x) && parseFloatToken(d, pos, y)) {
                if (isRel) {
                    x1 += currentX; y1 += currentY;
                    x2 += currentX; y2 += currentY;
                    x += currentX;  y += currentY;
                }
                path->setCommands({ PathCommandType::CubicBezier, {x1, y1, x2, y2, x, y} });
                lastCtrlX = x2; lastCtrlY = y2; // Lưu để dùng cho lệnh S
                currentX = x; currentY = y;
                skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'S': { // Smooth Cubic Bezier
            float x2, y2, x, y;
            while (parseFloatToken(d, pos, x2) && parseFloatToken(d, pos, y2) &&
                parseFloatToken(d, pos, x) && parseFloatToken(d, pos, y)) {
                if (isRel) {
                    x2 += currentX; y2 += currentY;
                    x += currentX;  y += currentY;
                }
                // Tính toán điểm x1, y1 đối xứng (để renderer dùng luôn cho AddBezier)
                float x1 = currentX, y1 = currentY;
                if (!path->getCommands().empty()) {
                    PathCommandType prevType = path->getCommands().back().type;
                    if (prevType == PathCommandType::CubicBezier || prevType == PathCommandType::SmoothCubicBezier) {
                        x1 = 2.0f * currentX - lastCtrlX;
                        y1 = 2.0f * currentY - lastCtrlY;
                    }
                }
                path->setCommands({ PathCommandType::SmoothCubicBezier, {x1, y1, x2, y2, x, y} });
                lastCtrlX = x2; lastCtrlY = y2;
                currentX = x; currentY = y;
                skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
                //case 'A': { // Elliptical Arc
                //    float rx, ry, rot, large, sweep, x, y;
                //    while (parseFloatToken(d, pos, rx) && parseFloatToken(d, pos, ry) &&
                //        parseFloatToken(d, pos, rot) && parseFloatToken(d, pos, large) &&
                //        parseFloatToken(d, pos, sweep) && parseFloatToken(d, pos, x) &&
                //        parseFloatToken(d, pos, y)) {
                //        if (isRel) { x += currentX; y += currentY; }
                //        path->setCommands({ PathCommandType::Arc, {rx, ry, rot, large, sweep, x, y} });
                //        currentX = x; currentY = y;
                //        skipSpacesAndCommas(d, pos);
                //        if (pos < d.size() && std::isalpha(d[pos])) break;
                //    }
                //    break;
                //}
                //case 'A': {
                //    float rx, ry, rot, large, sweep, x, y;
                //    while (parseFloatToken(d, pos, rx) && parseFloatToken(d, pos, ry) &&
                //        parseFloatToken(d, pos, rot) && parseFloatToken(d, pos, large) &&
                //        parseFloatToken(d, pos, sweep) && parseFloatToken(d, pos, x) &&
                //        parseFloatToken(d, pos, y)) {

                //        float targetX = x;
                //        float targetY = y;

                //        if (isRel) { targetX += currentX; targetY += currentY; }

                //        // --- LỆNH IN ĐỂ DEBUG ---
                //        std::cout << "--- Command Arc (A/a) detected ---" << std::endl;
                //        std::cout << "Original Type: " << cmd << " (isRel: " << (isRel ? "true" : "false") << ")" << std::endl;
                //        std::cout << "Params read: "
                //            << "rx=" << rx << ", ry=" << ry << ", rot=" << rot
                //            << ", large=" << large << ", sweep=" << sweep
                //            << ", x=" << x << ", y=" << y << std::endl;
                //        std::cout << "Calculated Absolute EndPoint: (" << targetX << ", " << targetY << ")" << std::endl;
                //        std::cout << "Current StartPoint: (" << currentX << ", " << currentY << ")" << std::endl;
                //        // ------------------------

                //        path->setCommands({ PathCommandType::Arc, {rx, ry, rot, large, sweep, targetX, targetY} });

                //        currentX = x; currentY = y;
                //        lastCtrlX = currentX; lastCtrlY = currentY;
                //        skipSpacesAndCommas(d, pos);
                //        if (pos < d.size() && std::isalpha(d[pos]) && d[pos] != 'e') break;
                //    }
                //    break;
                //}
        case 'A': {
            float rx, ry, rot, large, sweep, x, y;
            while (parseFloatToken(d, pos, rx) &&
                parseFloatToken(d, pos, ry) &&
                parseFloatToken(d, pos, rot) &&
                parseArcFlag(d, pos, large) && // Dùng hàm đọc flag riêng
                parseArcFlag(d, pos, sweep) && // Dùng hàm đọc flag riêng
                parseFloatToken(d, pos, x) &&
                parseFloatToken(d, pos, y)) {

                float targetX = x;
                float targetY = y;
                if (isRel) {
                    targetX += currentX;
                    targetY += currentY;
                }

                // --- LỆNH IN ĐỂ DEBUG ---
                std::cout << "--- Command Arc (A/a) detected ---" << std::endl;
                std::cout << "Original Type: " << cmd << " (isRel: " << (isRel ? "true" : "false") << ")" << std::endl;
                std::cout << "Params read: "
                    << "rx=" << rx << ", ry=" << ry << ", rot=" << rot
                    << ", large=" << large << ", sweep=" << sweep
                    << ", x=" << x << ", y=" << y << std::endl;
                std::cout << "Calculated Absolute EndPoint: (" << targetX << ", " << targetY << ")" << std::endl;
                std::cout << "Current StartPoint: (" << currentX << ", " << currentY << ")" << std::endl;
                // ------------------------

                path->setCommands({ PathCommandType::Arc, {rx, ry, rot, large, sweep, targetX, targetY} });

                // QUAN TRỌNG: Cập nhật bằng tọa độ tuyệt đối để lệnh tiếp theo dùng đúng gốc
                currentX = targetX;
                currentY = targetY;

                lastCtrlX = currentX; lastCtrlY = currentY;
                skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos]) && d[pos] != 'e') break;
            }
            break;
        }
        case 'Z': {
            path->setCommands({ PathCommandType::ClosePath, {} });
            currentX = subPathStartX; // Quay về điểm bắt đầu Sub-path
            currentY = subPathStartY;
            break;
        }
        default: pos++; break;
        }
    }
}
