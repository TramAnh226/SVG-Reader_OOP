#include <sstream>
#include <algorithm>
#include <iostream>
#include "Stroke.h";
#include "SVGParser.h"
#include "Utils.hpp"

SVGTransform SVGParser::parseTransform(const string& str) {
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
            auto nums = Utils::parseNumbers(inside);
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
            auto nums = Utils::parseNumbers(inside);
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
            auto nums = Utils::parseNumbers(inside);
            float sx = nums.size() > 0 ? nums[0] : 1.0f;
            float sy = nums.size() > 1 ? nums[1] : sx;
            t.addScale(sx, sy);
            pos = r + 1;
        }
        else if (s.compare(pos, 6, "matrix") == 0) {
            pos += 6;
            size_t l = s.find('(', pos);
            size_t r = s.find(')', l + 1);
            if (l != string::npos && r != string::npos) {
                string inside = s.substr(l + 1, r - l - 1);
                auto nums = Utils::parseNumbers(inside);
                if (nums.size() == 6) {
                    // Thêm lệnh addMatrix vào SVGTransform của bạn
                     t.addMatrix(nums[0], nums[1], nums[2], nums[3], nums[4], nums[5]);
                }
                pos = r + 1;
            }
        }
        else {
            ++pos;
        }
    }

    return t;
}

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

            // LƯU Ý: Nếu muốn giữ cấu trúc đa hình, cần sửa đổi SVGText::parse để nó
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

        return finalVal;
    }
    catch (...) {
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
            if (Utils::parseFloatToken(s, pos, offset)) {
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

    const char* transformAttr = node->Attribute("gradientTransform");
    if (transformAttr) {
        // Gọi hàm đã được định nghĩa trong lớp
        grad->setGradientTransform(SVGParser::parseTransform(transformAttr));
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

    // Đọc đúng "cx" và "cy" thay vì "x1", "y1"
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



void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
    // 1. Lấy đối tượng style hiện tại (sao chép giá trị từ element)
    // Lưu ý: currentStyle là một bản sao giá trị, sửa đổi nó không làm thay đổi element cho đến khi setSVGStyle được gọi.
    SVGStyle& currentStyle = element->getSVGStyle();

    const tinyxml2::XMLAttribute* strokeAttr = xmlNode->FindAttribute("stroke");
    const tinyxml2::XMLAttribute* widthAttr = xmlNode->FindAttribute("stroke-width");
    const tinyxml2::XMLAttribute* opacityAttr = xmlNode->FindAttribute("stroke-opacity");

    if ((strokeAttr || widthAttr || opacityAttr) && currentStyle.getStroke() == nullptr) {
        Stroke tempStroke;
        currentStyle.setStroke(&tempStroke);
        // Sau lệnh này, currentStyle.getStroke() KHÔNG BAO GIỜ LÀ nullptr.
    }

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
                Utils::parseColorString(value, c, alpha, hasAlpha);
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
                    // Phân loại URL (Gradient) và Color (Đơn)
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

    // Xóa tất cả các khoảng trắng (whitespace) trong chuỗi style
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    // 2. Tìm vị trí của dấu hai chấm (:)
    size_t pos = s.find(':');

    // Nếu không tìm thấy dấu hai chấm, kết thúc hàm
    if (pos == std::string::npos) return;

    // 3. Phân tách Key và Value
    // Key: Từ đầu chuỗi đến vị trí dấu hai chấm
    std::string key = s.substr(0, pos);
    // Value: Từ vị trí sau dấu hai chấm đến hết chuỗi
    std::string value = s.substr(pos + 1);

    // 4. Cập nhật thuộc tính Stroke (với xử lý ngoại lệ an toàn)
    if (key == "stroke") {
        // Phân loại URL (Gradient) và Color (Đơn)
        if (value.size() >= 4 && value.substr(0, 4) == "url(") {
            // TRƯỜNG HỢP 1: Gradient
            size_t start = value.find_first_of('#');
            size_t end = value.find_last_of(')');

            if (start != std::string::npos && end != std::string::npos && end > start) {
                std::string idWithHash = value.substr(start, end - start);
                // Gán ID Gradient cho Stroke
                stroke.strokeGradId = idWithHash;
                stroke.strokeGrad = nullptr; // Đảm bảo reset con trỏ

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
 
void SVGParser::parseInheritableAttributes(tinyxml2::XMLElement* xmlNode, std::unordered_map<std::string, std::string>& inheritableMap) {
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
    float subPathStartX = 0.0f, subPathStartY = 0.0f;
    float lastCtrlX = 0.0f, lastCtrlY = 0.0f;

    while (pos < d.size()) {
        Utils::skipSpacesAndCommas(d, pos);
        if (pos >= d.size()) break;

        // Nhận diện lệnh mới
        if (std::isalpha((unsigned char)d[pos])) {
            cmd = d[pos];
            pos++;
        }

        bool isRel = (cmd >= 'a' && cmd <= 'z');
        char absCmd = (char)std::toupper((unsigned char)cmd);

        switch (absCmd) {
        case 'M': {
            float x, y;
            bool firstPair = true; // Cờ đánh dấu cặp số đầu tiên
            while (Utils::parseFloatToken(d, pos, x) && Utils::parseFloatToken(d, pos, y)) {
                float absX = isRel ? (x + currentX) : x;
                float absY = isRel ? (y + currentY) : y;

                if (firstPair) {
                    // Cặp đầu tiên luôn là MoveTo
                    path->setCommands({ PathCommandType::MoveTo, {absX, absY} });
                    subPathStartX = absX; subPathStartY = absY; // Lưu gốc để lệnh Z dùng
                    firstPair = false;
                }
                else {
                    // Các cặp số tiếp theo của lệnh M phải là LineTo
                    path->setCommands({ PathCommandType::LineTo, {absX, absY} });
                }

                currentX = absX; currentY = absY;
                Utils::skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'S': { // Smooth Cubic Bezier
            float x2, y2, x, y;
            // Vòng lặp while giúp đọc được chuỗi nén như S 1,2 3,4 5,6 7,8
            while (Utils::parseFloatToken(d, pos, x2) && Utils::parseFloatToken(d, pos, y2) &&
                Utils::parseFloatToken(d, pos, x) && Utils::parseFloatToken(d, pos, y)) {

                float absX2 = isRel ? (x2 + currentX) : x2;
                float absY2 = isRel ? (y2 + currentY) : y2;
                float absX = isRel ? (x + currentX) : x;
                float absY = isRel ? (y + currentY) : y;

                // 1. Tính điểm điều khiển đối xứng (x1, y1)
                float absX1 = currentX;
                float absY1 = currentY;

                if (!path->getCommands().empty()) {
                    PathCommandType prevType = path->getCommands().back().type;
                    // Nếu lệnh trước là Cubic hoặc Smooth Cubic, ta lấy đối xứng
                    if (prevType == PathCommandType::CubicBezier || prevType == PathCommandType::SmoothCubicBezier) {
                        absX1 = 2.0f * currentX - lastCtrlX;
                        absY1 = 2.0f * currentY - lastCtrlY;
                    }
                }

                // 2. Lưu lệnh với đầy đủ 6 tham số (giả lập như lệnh 'C')
                PathCommand pc;
                pc.type = PathCommandType::SmoothCubicBezier;
                pc.params = { absX1, absY1, absX2, absY2, absX, absY };
                path->setCommands(pc);

                // 3. Cập nhật trạng thái cho vòng lặp tiếp theo
                lastCtrlX = absX2; // Điểm điều khiển thứ 2 của lệnh này trở thành 'lastCtrl' cho lệnh sau
                lastCtrlY = absY2;
                currentX = absX;
                currentY = absY;

                Utils::skipSpacesAndCommas(d, pos);
                // Nếu ký tự tiếp theo là chữ cái (lệnh mới), thoát vòng lặp while
                if (pos < d.size() && std::isalpha((unsigned char)d[pos]) && d[pos] != 'e' && d[pos] != 'E')
                    break;
                lastCtrlX = absX2;
                lastCtrlY = absY2;
                currentX = absX;
                currentY = absY;
            }
            break;
        }
        case 'L': {
            float x, y;
            while (Utils::parseFloatToken(d, pos, x) && Utils::parseFloatToken(d, pos, y)) {
                float absX = isRel ? (x + currentX) : x;
                float absY = isRel ? (y + currentY) : y;

                path->setCommands({ PathCommandType::LineTo, {absX, absY} });
                currentX = absX; currentY = absY;
                Utils::skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'H': {
            float x;
            while (Utils::parseFloatToken(d, pos, x)) {
                float absX = isRel ? (x + currentX) : x;
                path->setCommands({ PathCommandType::HLineTo, {absX} });
                currentX = absX;
                Utils::skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
            }
            break;
        }
        case 'V': {
            float y;
            while (Utils::parseFloatToken(d, pos, y)) {
                float absY = isRel ? (y + currentY) : y;
                path->setCommands({ PathCommandType::VLineTo, {absY} });
                currentY = absY;
                Utils::skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
                lastCtrlX = currentX;
                lastCtrlY = currentY;
            }
            break;
        }
        case 'C': {
            float x1, y1, x2, y2, x, y;
            while (Utils::parseFloatToken(d, pos, x1) && Utils::parseFloatToken(d, pos, y1) &&
                Utils::parseFloatToken(d, pos, x2) && Utils::parseFloatToken(d, pos, y2) &&
                Utils::parseFloatToken(d, pos, x) && Utils::parseFloatToken(d, pos, y)) {

                float aX1 = isRel ? (x1 + currentX) : x1; float aY1 = isRel ? (y1 + currentY) : y1;
                float aX2 = isRel ? (x2 + currentX) : x2; float aY2 = isRel ? (y2 + currentY) : y2;
                float aX = isRel ? (x + currentX) : x;   float aY = isRel ? (y + currentY) : y;

                path->setCommands({ PathCommandType::CubicBezier, {aX1, aY1, aX2, aY2, aX, aY} });
                lastCtrlX = aX2; lastCtrlY = aY2;
                currentX = aX; currentY = aY;
                Utils::skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos])) break;
                lastCtrlX = aX2;
                lastCtrlY = aY2;
                currentX = aX;
                currentY = aY;
            }
            break;
        }
        case 'A': {
            float rx, ry, rot, large, sweep, x, y;
            while (Utils::parseFloatToken(d, pos, rx) && Utils::parseFloatToken(d, pos, ry) &&
                Utils::parseFloatToken(d, pos, rot) && Utils::parseArcFlag(d, pos, large) &&
                Utils::parseArcFlag(d, pos, sweep) && Utils::parseFloatToken(d, pos, x) &&
                Utils::parseFloatToken(d, pos, y)) {

                float aX = isRel ? (x + currentX) : x;
                float aY = isRel ? (y + currentY) : y;

                path->setCommands({ PathCommandType::Arc, {rx, ry, rot, large, sweep, aX, aY} });
                currentX = aX; currentY = aY;
                Utils::skipSpacesAndCommas(d, pos);
                if (pos < d.size() && std::isalpha(d[pos]) && d[pos] != 'e') break;
            }
            break;
        }
        case 'Z': {
            path->setCommands({ PathCommandType::ClosePath, {} });
            currentX = subPathStartX; currentY = subPathStartY;
            break;
        }
        default:
            pos++;
            break;
        }
    }
}