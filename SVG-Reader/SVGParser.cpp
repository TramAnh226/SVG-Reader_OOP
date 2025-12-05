#include "SVGParser.h"
#include <sstream>
#include <algorithm>
#include <iostream>

SVGGroup* SVGParser::readXML(const std::string& filename, SVGFactoryPattern& factory) {
    tinyxml2::XMLDocument doc;

    // 1. Kiểm tra tải file
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Cannot read SVG file: " << filename << '\n';
        return nullptr;
    }

    tinyxml2::XMLElement* rootXML = doc.RootElement();

    // 2. Kiểm tra phần tử gốc (Root Element)
    if (!rootXML) {
        std::cerr << " SVG file is invalid (No root element found)!" << '\n';
        return nullptr;
    }

    // 3. Khởi tạo Root Group (phải là SVGGroup)
    // Cần đảm bảo root element là <svg> hoặc <g>, nếu không thì lỗi
    if (std::string(rootXML->Name()) != "svg") {
        std::cerr << "Root element must be <svg>!" << '\n';
        return nullptr;
    }

    SVGGroup* rootGroup = new SVGGroup();
    rootGroup->setParent(nullptr);
    rootGroup->setTagName(rootXML->Name());

    // 4. Parse thuộc tính của thẻ <svg> (width, height, viewBox, id, style)
    // Các thuộc tính cơ bản (id, style) sẽ được parse trong parseAttributes.
    // Các thuộc tính cụ thể của Group (width, height, viewBox) thường được parse trong rootGroup->parse().

    // Thứ tự này rất quan trọng:
    // a. Parse các thuộc tính chung (parseAttributes)
    parseAttributes(rootXML, rootGroup);

    // b. Parse các thuộc tính chuyên biệt của Group (SVGGroup::parse)
    // Hàm này phải đọc width, height, viewBox và lưu trữ chúng vào rootGroup.
    rootGroup->parse(rootXML);

    // c. Parse các node con (parseNode)
    parseNode(rootXML, rootGroup, factory);

    return rootGroup;
}


// sua thanh da hinh (bo if-else)
void SVGParser::parseNode(tinyxml2::XMLElement* xmlNode, SVGGroup* parentGroup, SVGFactoryPattern& factory) {
    for (tinyxml2::XMLElement* child = xmlNode->FirstChildElement(); child; child = child->NextSiblingElement()) {
        // lay tagname
        std::string tagName = child->Name();
        // tao doi tuong moi
        SVGElement* element = factory.getElement(tagName);

        if (!element) {
            continue;
        }
        // parse cac thuoc tinh chung
        parseAttributes(child, element);
        // dung polymorphism goi parse cua tung element
        element->parse(child);
        // them element vao group
        parentGroup->addElement(element);

        // dung de quy goi parseNode cua tung node con
        if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
            parseNode(child, group, factory);
        }
    }
}

void SVGParser::parseAttributes(tinyxml2::XMLElement* xmlNode, SVGElement* element) {
    // 1. Lấy đối tượng style hiện tại (sao chép giá trị từ element)
    // Lưu ý: currentStyle là một bản sao giá trị, sửa đổi nó không làm thay đổi element cho đến khi setSVGStyle được gọi.
    SVGStyle currentStyle = element->getSVGStyle();

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
            currentStyle.parse(value);
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
            if (s) { // Chỉ chạy nếu Stroke đã được khởi tạo ở bước trên
                // SỬ DỤNG LOGIC TƯƠNG TỰ NHƯ Stroke::parse()
                if (name == "stroke") {
                    s->strokeColor = CustomColor::fromStringToCustomColor(value);
                }
                else if (name == "stroke-width") {
                    try { s->strokeWidth = std::max(0.0f, stof(value)); }
                    catch (...) {}
                }
                else if (name == "stroke-opacity") {
                    try { s->strokeOpacity = std::min(1.0f, std::max(0.0f, stof(value))); }
                    catch (...) {}
                }
            }
        }

        attr = attr->Next();
    }
    // 3. Gán lại style sau khi tất cả các thuộc tính đã được phân tích cú pháp
    element->setSVGStyle(currentStyle);
}

//void SVGParser::parseStroke(Stroke& stroke, const std::string& styleStr) {
//    // Lưu ý: styleStr được kỳ vọng là chuỗi "key:value" (ví dụ: "stroke-width:10") VÀ KHÔNG CÓ KHOẢNG TRẮNG.
//    // <rect fill - opacity = "0.01" stroke = "rgb(200,200,200)" stroke - width = "2" x = "25" y = "25" width = "800" height = "400" / >
//
//    std::string s = styleStr;
//
//    // Nếu hàm này được gọi từ parseAttributes, chuỗi đã được làm sạch khoảng trắng. 
//    // Chúng ta chỉ cần phân tách key/value lần nữa (đảm bảo nó khớp với logic của parseStyle).
//    size_t pos = s.find(':');
//    if (pos == std::string::npos) return;
//
//    std::string key = s.substr(0, pos);
//    std::string value = s.substr(pos + 1);
//
//    // Xóa khoảng trắng một lần nữa (Phòng ngừa)
//    value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
//
//    // Cập nhật thuộc tính Stroke (với xử lý ngoại lệ và giới hạn an toàn)
//    if (key == "stroke") {
//        stroke.strokeColor = CustomColor::fromStringToCustomColor(value);
//    }
//    else if (key == "stroke-width") {
//        // KIỂM TRA ĐIỀU KIỆN CHUỖI RỖNG
//        if (value.empty()) {
//            stroke.strokeWidth = 1.0f;
//            return;
//        }
//        try {
//            float width = std::stof(value);
//            // Giới hạn giá trị: Đảm bảo width không âm và có giá trị tối thiểu
//            stroke.strokeWidth = (((0.1f) > (width)) ? (0.1f) : (width));
//            
//        }
//        catch (const std::exception& e) {
//            stroke.strokeWidth = 1.0f; // Giá trị mặc định an toàn
//        }
//    }
//    else if (key == "stroke-opacity") {
//        // KIỂM TRA ĐIỀU KIỆN CHUỖI RỖNG
//        if (value.empty()) {
//            stroke.strokeOpacity = 1.0f;
//            return;
//        }
//        try {
//            float opacity = std::stof(value);
//            // Giới hạn giá trị: Đảm bảo opacity nằm trong khoảng [0.0, 1.0]
//            stroke.strokeOpacity = (((0.1f) > ((((0.0f) > (opacity)) ? (0.0f) : (opacity)))) ? (0.1f) : ((((0.0f) > (opacity)) ? (0.0f) : (opacity))));
//        }
//        catch (const std::exception& e) {
//            stroke.strokeOpacity = 1.0f; // Giá trị mặc định là mờ đục
//        }
//    }
//}
//void SVGParser::parseStyle(SVGStyle& style, const std::string& styleStr) {
//    std::stringstream ss(styleStr);
//    std::string item;
//
//    // Sử dụng std::remove_if từ <algorithm>
//    while (std::getline(ss, item, ';')) {
//        // Loại bỏ khoảng trắng và kiểm tra chuỗi rỗng
//        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
//        if (item.empty()) continue;
//
//        // Tìm vị trí của dấu hai chấm (:)
//        size_t pos = item.find(':');
//        if (pos == std::string::npos) continue;
//
//        std::string key = item.substr(0, pos);
//        std::string value = item.substr(pos + 1);
//
//        // 1. Xử lý Fill
//        if (key == "fill") {
//            style.setFillColor(CustomColor::fromStringToCustomColor(value));
//        }
//        else if (key == "fill-opacity") {
//            try {
//                style.setFillOpacity(std::stof(value));
//            }
//            catch (const std::exception& e) {
//                std::cerr << "Style parsing error (fill-opacity): " << e.what() << std::endl;
//            }
//        }
//        // 2. Xử lý Stroke (Chuyển trách nhiệm cho parseStroke)
//        else if (key.find("stroke") != std::string::npos) {
//
//            // Đảm bảo Stroke được cấp phát trước khi sửa đổi (Logic này thường được gọi trong parseAttributes)
//            Stroke* currentStroke = style.getStroke();
//
//            if (!currentStroke) {
//                Stroke tempStroke;
//                style.setStroke(&tempStroke);
//                currentStroke = style.getStroke();
//            }
//
//            // GỌI HÀM ĐƯỢC THIẾT KẾ ĐỂ XỬ LÝ CHUỖI KEY:VALUE
//            if (currentStroke) {
//                // Gửi toàn bộ item ("key:value") đến parseStroke
//                this->parseStroke(*currentStroke, item);
//            }
//        }
//    }
//}

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
     std::stringstream ss(pointsStr);
     std::string token;
     while (getline(ss, token, ' ')) {
         if (token.empty()) continue;
         size_t comma = token.find(',');
         if (comma == std::string::npos) continue;
         float x = stof(token.substr(0, comma));
         float y = stof(token.substr(comma + 1));
         pts.emplace_back(x, y);
     }
     poly->setPoints(pts);
 }

 //void SVGParser::parseText(SVGText* text, tinyxml2::XMLElement* Node) {
 //    float x = Node->FloatAttribute("x");
 //    float y = Node->FloatAttribute("y");
 //    const char* content = Node->GetText();
 //    text->setStart(CustomPoint(x, y));
 //    text->setContent(content ? content : "");
 //}
 void SVGParser::parseText(SVGText* text, tinyxml2::XMLElement* Node) {
    float x = Node->FloatAttribute("x");
    float y = Node->FloatAttribute("y");
    const char* content = Node->GetText();
    text->setStart(CustomPoint(x, y));
    const char* content_char = Node->GetText();

    // Chuyển đổi char* sang wstring để lưu trữ
    if (content_char) {
        std::string content_str(content_char);
        text->setContent(std::wstring(content_str.begin(), content_str.end()));
    }
    else {
        text->setContent(L"");
    }
}

 void SVGParser::parseGroup(SVGGroup* group, tinyxml2::XMLElement* Node) {
     /*const char* transformStr = Node->Attribute("transform");
     if (transformStr) {
         group->setTransform(std::string(transformStr));
     }*/
     if (std::string(Node->Name()) == "svg") {
         float w = Node->FloatAttribute("width");
         if (w <= 0.0f) { w = 300.0f; }
         group->setWidth(w);
         float h = Node->FloatAttribute("height");
         if (h <= 0.0f) { h = 150.0f; }
         group->setHeight(h);
         const char* viewBoxStr = Node->Attribute("viewBox");
         if (viewBoxStr) {
             group->setViewBox(std::string(viewBoxStr));
         }
         else {
             group->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
         }
     }
 }
