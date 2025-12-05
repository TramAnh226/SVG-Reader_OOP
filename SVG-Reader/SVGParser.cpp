#include "SVGParser.h"

#include <sstream>
#include <algorithm>
#include <iostream>

// using namespace tinyxml2;
// using namespace std;

SVGGroup* SVGParser::readXML(const std::string& filename, SVGFactoryPattern& factory) {
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
    rootGroup->parse(rootXML);
    parseNode(rootXML, rootGroup, factory);
    // them parse thuoc tinh group
    return rootGroup;
}



// sua thanh da hinh (bo if-else)
void SVGParser::parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory) {
    for (tinyxml2::XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {

        std::string tagName = child->Name();
        SVGElement* element = factory.getElement(tagName);

        if (!element) {
            continue;
        }

        parseAttributes(child, element);
        element->parse(child);
        parentGroup->addElement(element);

        if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
            parseNode(child, group, factory);
        }
    }
}


//void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
//    // parse thuoc tinh chung
//    const tinyxml2::XMLAttribute* attr = xmlNode->FirstAttribute();
//    while (attr) {
//        std::string name = attr->Name();
//        std::string value = attr->Value();
//
//        if (name == "id") {
//            element->setId(value);
//        }
//        else if (name == "style") {
//            SVGStyle style;
//            parseStyle(style, value);
//            element->setSVGStyle(style);
//        }
//        /*else if (name == "transform") {
//            element->setTransform(value);
//        }*/
//        attr = attr->Next();
//    }
//}

//void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
//    // 1. Lấy đối tượng style hiện tại
//    SVGStyle currentStyle = element->getSVGStyle();
//
//    // --- KHẮC PHỤC: KIỂM TRA VÀ KHỞI TẠO STROKE NẾU THIẾU ---
//    const tinyxml2::XMLAttribute* strokeAttr = xmlNode->FindAttribute("stroke");
//    const tinyxml2::XMLAttribute* widthAttr = xmlNode->FindAttribute("stroke-width");
//
//    if ((strokeAttr || widthAttr) && currentStyle.getStroke() == nullptr) {
//        Stroke tempStroke;
//        currentStyle.setStroke(&tempStroke);
//        // currentStyle hiện đã có một đối tượng Stroke hợp lệ (không null)
//    }
//    // ----------------------------------------------------
//
//    // 2. Vòng lặp phân tích cú pháp tất cả các thuộc tính
//    const tinyxml2::XMLAttribute* attr = xmlNode->FirstAttribute();
//    while (attr) {
//        std::string name = attr->Name();
//        std::string value = attr->Value();
//
//        if (name == "id") {
//            element->setId(value);
//        }
//        else if (name == "style") {
//            // Xử lý style tổng hợp (giữ nguyên)
//            SVGStyle style;
//            this->parseStyle(style, value);
//            element->setSVGStyle(style);
//        }
//        // Xử lý Fill riêng lẻ
//        else if (name == "fill") {
//            currentStyle.setFillColor(CustomColor::fromStringToCustomColor(value));
//        }
//        else if (name == "fill-opacity") {
//            try { currentStyle.setFillOpacity(std::stof(value)); }
//            catch (...) {}
//        }
//        // Xử lý Stroke riêng lẻ (sau khi đã khởi tạo Stroke ở trên)
//        else if (name == "stroke" || name == "stroke-width" || name == "stroke-opacity") {
//            Stroke* s = currentStyle.getStroke();
//            if (s) {
//                // Sửa đổi thuộc tính Stroke trực tiếp
//                if (name == "stroke") {
//                    s->strokeColor = CustomColor::fromStringToCustomColor(value);
//                }
//                else if (name == "stroke-width") {
//                    try { s->strokeWidth = std::stof(value); }
//                    catch (...) {}
//                }
//                else if (name == "stroke-opacity") {
//                    try { s->strokeOpacity = std::stof(value); }
//                    catch (...) {}
//                }
//            }
//        }
//
//        attr = attr->Next();
//    }
//    // 3. Gán lại style sau khi tất cả các thuộc tính đã được phân tích cú pháp
//    element->setSVGStyle(currentStyle);
//}

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
        else if (name == "style") {
            // Xử lý style tổng hợp: Gọi parseStyle để áp dụng nhiều thuộc tính cùng lúc
            // Không cần tạo đối tượng style mới, chỉ cần sửa đổi currentStyle
            this->parseStyle(currentStyle, value);
        }
        // Xử lý Fill riêng lẻ
        else if (name == "fill") {
            currentStyle.setFillColor(CustomColor::fromStringToCustomColor(value));
        }
        else if (name == "fill-opacity") {
            try { currentStyle.setFillOpacity(std::stof(value)); }
            catch (...) {}
        }
        // Xử lý Stroke riêng lẻ (sau khi đã đảm bảo Stroke tồn tại)
        else if (name == "stroke" || name == "stroke-width" || name == "stroke-opacity") {
            Stroke* s = currentStyle.getStroke();
            if (s) { // Lô-gic này luôn đúng vì đã kiểm tra ở Bước 1
                if (name == "stroke") {
                    s->strokeColor = CustomColor::fromStringToCustomColor(value);
                }
                else if (name == "stroke-width") {
                    try { s->strokeWidth = std::stof(value); }
                    catch (...) {}
                }
                else if (name == "stroke-opacity") {
                    try { s->strokeOpacity = std::stof(value); }
                    catch (...) {}
                }
            }
        }

        attr = attr->Next();
    }
    // 3. Gán lại style sau khi tất cả các thuộc tính đã được phân tích cú pháp
    element->setSVGStyle(currentStyle);
}

 /*void SVGParser::parseStyle(SVGStyle& style, const std::string& styleStr) {
 	std::stringstream ss(styleStr);
 	std::string item;
 	while (getline(ss, item, ';')) {
 		item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
 		if (item.empty()) continue;
 		size_t pos = item.find(':');
 		if (pos == std::string::npos) continue;
 		std::string key = item.substr(0, pos);
 		std::string value = item.substr(pos + 1);
 		if (key == "fill") style.setFillColor(CustomColor::fromStringToCustomColor(value));
 		else if (key == "fill-opacity") style.setFillOpacity(stof(value));
 		else if (key.find("stroke") != std::string::npos) {
             Stroke* stroke = style.getStroke();
 			parseStroke(*stroke, item);
 			style.setStroke(stroke);
 		}
 	}
 }*/
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
            style.setFillColor(CustomColor::fromStringToCustomColor(value));
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
//void SVGParser::parseStroke(Stroke& stroke, const std::string& styleStr) {
//    std::string s = styleStr;
//    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
//    size_t pos = s.find(':');
//    if (pos == std::string::npos) return;
//
//    std::string key = s.substr(0, pos);
//    std::string value = s.substr(pos + 1);
//
//    if (key == "stroke") stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
//    else if (key == "stroke-width") stroke.strokeWidth = stof(value);
//    else if (key == "stroke-opacity") stroke.strokeOpacity = stof(value);
//}
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
        stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
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
}

// void SVGParser::parseRectangle(SVGRectangle* rect, tinyxml2::XMLElement* Node) {
//     float x = Node->FloatAttribute("x");
//     float y = Node->FloatAttribute("y");
//     float w = Node->FloatAttribute("width");
//     float h = Node->FloatAttribute("height");
//     rect->setTopLeftCorner(CustomPoint(x, y));
//     rect->setWidth(w);
//     rect->setHeight(h);
// }

// void SVGParser::parseSquare(SVGSquare* sq, tinyxml2::XMLElement* Node) {
//     float x = Node->FloatAttribute("x");
//     float y = Node->FloatAttribute("y");
//     float size = Node->FloatAttribute("width"); // SVG square dùng width = height
//     sq->setTopLeftCorner(CustomPoint(x, y));
//     sq->setWidth(size);
//     sq->setHeight(size);
// }

// void SVGParser::parseEllipse(SVGEllipse* el, tinyxml2::XMLElement* Node) {
//     float cx = Node->FloatAttribute("cx");
//     float cy = Node->FloatAttribute("cy");
//     float rx = Node->FloatAttribute("rx");
//     float ry = Node->FloatAttribute("ry");
//     el->setCenter(CustomPoint(cx, cy));
//     el->setRadiusX(rx);
//     el->setRadiusY(ry);
// }

// void SVGParser::parseCircle(SVGCircle* circle, tinyxml2::XMLElement* Node) {
//     float cx = Node->FloatAttribute("cx");
//     float cy = Node->FloatAttribute("cy");
//     float r = Node->FloatAttribute("r");
//     circle->setCenter(CustomPoint(cx, cy));
//     circle->setRadius(r);
// }

// void SVGParser::parseLine(SVGLine* line, tinyxml2::XMLElement* Node) {

//     float x1 = Node->FloatAttribute("x1");
//     float y1 = Node->FloatAttribute("y1");
//     float x2 = Node->FloatAttribute("x2");
//     float y2 = Node->FloatAttribute("y2");
//     line->setStartPoint(CustomPoint(x1, y1));
//     line->setEndPoint(CustomPoint(x2, y2));
// }

// void SVGParser::parsePolyshape(SVGPolyshapeBase* poly, tinyxml2::XMLElement* Node) {

//     const char* pointsStr = Node->Attribute("points");
//     if (!pointsStr) return;

//     vector<CustomPoint> pts;
//     stringstream ss(pointsStr);
//     string token;
//     while (getline(ss, token, ' ')) {
//         if (token.empty()) continue;
//         size_t comma = token.find(',');
//         if (comma == string::npos) continue;
//         float x = stof(token.substr(0, comma));
//         float y = stof(token.substr(comma + 1));
//         pts.emplace_back(x, y);
//     }
//     poly->setPoints(pts);
// }

// void SVGParser::parseText(SVGText* text, tinyxml2::XMLElement* Node) {

//     float x = Node->FloatAttribute("x");
//     float y = Node->FloatAttribute("y");
//     const char* content = Node->GetText();
//     text->setStart(CustomPoint(x, y));
//     text->setContent(content ? content : "");
// }

// void SVGParser::parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node) {

//     /*const char* transformStr = Node->Attribute("transform");
//     if (transformStr) {
//         group->setTransform(std::string(transformStr));
//     }*/


//     if (std::string(Node->Name()) == "svg") {

//         float w = Node->FloatAttribute("width");
//         if (w <= 0.0f) { w = 300.0f; }
//         group->setWidth(w);


//         float h = Node->FloatAttribute("height");
//         if (h <= 0.0f) { h = 150.0f; }
//         group->setHeight(h);


//         const char* viewBoxStr = Node->Attribute("viewBox");
//         if (viewBoxStr) {
//             group->setViewBox(std::string(viewBoxStr));
//         }
//         else {
//             group->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
//         }
//     }
// }
