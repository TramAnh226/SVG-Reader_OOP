#include "SVGParser.h"
#include <sstream>
#include <algorithm>
#include <iostream>

// using namespace tinyxml2;
// using namespace std;

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

        element->setSVGStyle(parentGroup->getSVGStyle());

        parseAttributes(child, element);
        element->parse(*this, child);
        parentGroup->addElement(element);

        if (auto group = dynamic_cast<SVGGroup*>(element)) {
            group->setParent(parentGroup);
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

void SVGParser::parseGradient(SVGGradient* grad, tinyxml2::XMLElement* node) {
    if (!grad || !node) return;

    // Xử lý ID (Rất quan trọng cho tham chiếu)
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

    // Xử lý Transform
    const char* transformAttr = node->Attribute("gradientTransform");
    if (transformAttr) {
        grad->setTransform(transformAttr);
    }

     grad->clearStops(); 

    for (tinyxml2::XMLElement* child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string tagName = child->Name();

        if (tagName == "stop") {
            float offset = child->FloatAttribute("offset");
            const char* stopColorAttr = child->Attribute("stop-color");
            const char* stopOpacityAttr = child->Attribute("stop-opacity");

            CustomColor color(0, 0, 0);
            float opacity = 1.0f;

            if (stopColorAttr) {
                color = CustomColor::fromStringToCustomColor(stopColorAttr);
            }
            if (stopOpacityAttr) {
                try { opacity = std::stof(stopOpacityAttr); }
                catch (...) {  }
            }

            grad->addStop(SVGStop(color, offset, opacity));
        }
    }
}

void SVGParser::parseLinearGradient(SVGLinearGradient* grad,tinyxml2::XMLElement* node) {
    if (!grad || !node) return;

    const char* x1Attr = node->Attribute("x1");
    if (x1Attr) { try { grad->setX1(std::stof(x1Attr)); } catch (...) {} }

    const char* y1Attr = node->Attribute("y1");
    if (y1Attr) { try { grad->setY1(std::stof(y1Attr)); } catch (...) {} }

    const char* x2Attr = node->Attribute("x2");
    if (x2Attr) { try { grad->setX2(std::stof(x2Attr)); } catch (...) {} }

    const char* y2Attr = node->Attribute("y2");
    if (y2Attr) { try { grad->setY2(std::stof(y2Attr)); } catch (...) {} }

    const char* href = node->Attribute("href");
    if (!href) {
        href = node->Attribute("xlink:href");
    }

    if (href) {
        grad->setHrefID(href); 
    }
}

void SVGParser::parseRadialGradient(SVGRadialGradient* grad, tinyxml2::XMLElement* node) {
    if (!grad || !node) return;

    const char* cxAttr = node->Attribute("cx");
    if (cxAttr) { try { grad->setCX(std::stof(cxAttr)); } catch (...) {} }
    else { grad->setCX(0.5f); }

    const char* cyAttr = node->Attribute("cy");
    if (cyAttr) { try { grad->setCY(std::stof(cyAttr)); } catch (...) {} }
    else { grad->setCY(0.5f); }

    const char* rAttr = node->Attribute("r");
    if (rAttr) { try { grad->setR(std::stof(rAttr)); } catch (...) {} }
    else { grad->setR(0.5f); }

    const char* fxAttr = node->Attribute("fx");
    const char* fyAttr = node->Attribute("fy");

    if (fxAttr) { try { grad->setFX(std::stof(fxAttr)); } catch (...) {} }
    else { grad->setFX(grad->getCX()); }
    if (fyAttr) { try { grad->setFY(std::stof(fyAttr)); } catch (...) {} }
    else { grad->setFY(grad->getCY()); }

    const char* href = node->Attribute("href");
    if (!href) {
        href = node->Attribute("xlink:href");
    }

    if (href) {
        grad->setHrefID(href);
    }
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
        else if (name == "style") {
            // Xử lý style tổng hợp: Gọi parseStyle để áp dụng nhiều thuộc tính cùng lúc
            // Không cần tạo đối tượng style mới, chỉ cần sửa đổi currentStyle
            this->parseStyle(currentStyle, value);
        }
        // Xử lý Fill riêng lẻ
        else if (name == "fill") {
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
            else if (value == "none" || value == "transparent") {
                // None
                currentStyle.setGradId("");
                currentStyle.setFillOpacity(0.0f);
            }
            else {
                // Màu đơn
                currentStyle.setGradId("");
                currentStyle.setFillColor(CustomColor::fromStringToCustomColor(value));
            }
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
     if (!group || !Node) return;

     // --- 1. Xử lý thuộc tính Transform (Áp dụng cho cả <svg> và <g>) ---

     //const char* transformStr = Node->Attribute("transform");
     //if (transformStr) {
     //    // Cần một hàm hỗ trợ trong SVGParser để phân tích chuỗi transform
     //    // và trả về Gdiplus::Matrix*
     //    Gdiplus::Matrix* matrix = parseTransform(transformStr);

     //    if (matrix) {
     //        // Giả định SVGGroup (hoặc SVGElement) có setTransform(Gdiplus::Matrix*)
     //        group->setTransform(matrix);
     //    }
     //}

     // --- 2. Xử lý thuộc tính đặc biệt của <svg> (width, height, viewBox) ---

     if (std::string(Node->Name()) == "svg") {

         // Đọc Width
         float w = Node->FloatAttribute("width");
         if (w <= 0.0f) { w = 300.0f; } // Mặc định
         group->setWidth(w);

         // Đọc Height
         float h = Node->FloatAttribute("height");
         if (h <= 0.0f) { h = 150.0f; } // Mặc định
         group->setHeight(h);

         // Đọc ViewBox
         const char* viewBoxStr = Node->Attribute("viewBox");
         if (viewBoxStr) {
             group->setViewBox(std::string(viewBoxStr));
         }
         else {
             // Gán ViewBox mặc định bằng Kích thước (nếu không có viewBox)
             group->setViewBox("0 0 " + std::to_string(w) + " " + std::to_string(h));
         }
     }
 }
