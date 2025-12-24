//#include <iostream>
//#include "SVGStyle.h"
//#include "Stroke.h" 
////#include "CustomColor.h" 
//#include "SVGGradient.h"
//#include "SVGDocumentContext.h"
//
////using namespace Gdiplus;
//
//Stroke* deepCopyStroke(const Stroke* original) {
//    if (!original) return nullptr;
//    return new Stroke(*original);
//}
//
//SVGStyle::SVGStyle() : fillOpacity(1.0f), stroke(nullptr), grad(nullptr), gradId("") {
//}
//
//SVGStyle::SVGStyle(const CustomColor& color, float opacity, Stroke* newStroke)
//    : fillColor(color), fillOpacity(opacity),
//    stroke(deepCopyStroke(newStroke)),
//    grad(nullptr), gradId("")
//{
//}
//
//SVGStyle::SVGStyle(const SVGStyle& other)
//    : fillColor(other.fillColor), fillOpacity(other.fillOpacity),
//    stroke(deepCopyStroke(other.stroke)),
//    grad(nullptr), // Luôn reset, sẽ được resolve sau
//    gradId(other.gradId) // Sao chép ID Gradient
//{
//}
//
//SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
//    if (this != &other) {
//        Stroke* tempStroke = deepCopyStroke(other.stroke);
//        delete this->stroke;
//        this->stroke = tempStroke;
//
//        this->fillColor = other.fillColor;
//        this->fillOpacity = other.fillOpacity;
//
//        this->gradId = other.gradId;
//        this->grad = nullptr; // Luôn reset và resolve sau
//    }
//    return *this;
//}
//
//SVGStyle::~SVGStyle() {
//    delete stroke;
//}
//CustomColor SVGStyle::getFillColor() const {
//    return fillColor;
//}
//void SVGStyle::setFillColor(const CustomColor& color) {
//    fillColor = color;
//}
//float SVGStyle::getFillOpacity() const {
//    return fillOpacity;
//
//}
//void SVGStyle::setFillOpacity(const float& opacity) {
//    fillOpacity = opacity;
//}
//Stroke* SVGStyle::getStroke() const {
//    return stroke;
//}
//
//void SVGStyle::setStroke(Stroke* newStroke) {
//    delete this->stroke;
//    this->stroke = deepCopyStroke(newStroke);
//}
//
//Gdiplus::Color SVGStyle::getGdiFillColor() const {
//    // 1. Chuyển đổi Opacity (0.0f - 1.0f) thành Alpha (0 - 255)
//    // Giá trị phải được giới hạn trong khoảng [0, 255]
//    BYTE alpha = static_cast<BYTE>(std::round(fillOpacity * 255.0f));
//
//    // 2. Lấy màu RGB cơ bản (sử dụng toán tử chuyển đổi của CustomColor)
//    Gdiplus::Color baseColor = fillColor;
//
//    // 3. Tạo màu ARGB mới
//    return Gdiplus::Color(
//        alpha,
//        baseColor.GetR(),
//        baseColor.GetG(),
//        baseColor.GetB()
//    );
//}
//void SVGStyle::parse(const std::string& styleStr) {
//    // p.parseStyle(*this, styleString);
//    std::stringstream ss(styleStr);
//    std::string item;
//    while (getline(ss, item, ';')) {
//        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
//        if (item.empty()) continue;
//
//        size_t pos = item.find(':');
//        if (pos == std::string::npos) continue;
//
//        std::string key = item.substr(0, pos);
//        std::string value = item.substr(pos + 1);
//
//        if (key == "fill") this->setFillColor(CustomColor::fromStringToCustomColor(value));
//        else if (key == "fill-opacity") this->setFillOpacity(stof(value));
//        else if (key.find("stroke") != std::string::npos) {
//            Stroke* stroke = this->getStroke();
//            // parseStroke(stroke, item);
//            std::string s = styleStr;
//            s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
//            size_t pos = s.find(':');
//            if (pos == std::string::npos) return;
//            std::string key = s.substr(0, pos);
//            std::string value = s.substr(pos + 1);
//            if (key == "stroke") stroke->strokeColor = CustomColor::fromStringToCustomColor(value);
//            else if (key == "stroke-width") stroke->strokeWidth = stof(value);
//            else if (key == "stroke-opacity") stroke->strokeOpacity = stof(value);
//
//            this->setStroke(stroke);
//        }
//    }
//}
//
//const std::string& SVGStyle::getGradId() const { 
//    return gradId; 
//}
//const SVGGradient* SVGStyle::getGrad() const { 
//    return grad; 
//}
//void SVGStyle::setGradId(const std::string& id) { 
//    this->gradId = id; 
//}
//
//void SVGStyle::setGrad(SVGGradient* ptr) {
//    this->grad = ptr;
//}
//
//// Hàm kiểm tra: Có Gradient cần fill và đã được resolve chưa?
//bool SVGStyle::hasGradientFill() const {
//    return grad != nullptr;
//}
//// void SVGStyle::render(SVGRenderer& r) const {
////     r.renderStyle(*this);
//// }  
//// void transform(Matrix*);         
//
//void SVGStyle::resolveGradient(const SVGDocumentContext& context) {
//    if (!gradId.empty() && grad == nullptr) {
//
//        // Trích xuất ID: loại bỏ dấu #
//        std::string id = gradId;
//        if (id.size() > 0 && id[0] == '#') {
//            id = id.substr(1);
//        }
//
//        // 2. Tìm kiếm ID Gradient trong Context
//        const SVGGradient* gradient = context.getGradientById(id);
//
//        if (gradient) {
//            // BƯỚC MỚI: GIẢI QUYẾT CHUỖI THAM CHIẾU
//            const SVGGradient* resolvedGradient = gradient->resolveReference(context);
//
//            // 3. Gán con trỏ thực tế (trỏ đến Gradient cuối cùng)
//            this->grad = const_cast<SVGGradient*>(resolvedGradient);
//        }
//        else {
//            // 4. Nếu không tìm thấy, reset
//            std::cerr << "Warning: Gradient ID #" << id << " not found during resolution.\n";
//            this->gradId = "";
//            this->grad = nullptr;
//        }
//    }
//}



#include <iostream>
#include "SVGStyle.h"
#include "Stroke.h" 
//#include "CustomColor.h" 
#include "SVGGradient.h"
#include "SVGDocumentContext.h"

//using namespace Gdiplus;

Stroke* deepCopyStroke(const Stroke* original) {
    if (!original) return nullptr;
    return new Stroke(*original);
}

SVGStyle::SVGStyle() : fillOpacity(1.0f), stroke(nullptr), grad(nullptr), gradId("") {
}

SVGStyle::SVGStyle(const CustomColor& color, float opacity, Stroke* newStroke)
    : fillColor(color), fillOpacity(opacity),
    stroke(deepCopyStroke(newStroke)),
    grad(nullptr), gradId("")
{
}

SVGStyle::SVGStyle(const SVGStyle& other)
    : fillColor(other.fillColor), fillOpacity(other.fillOpacity),
    stroke(deepCopyStroke(other.stroke)),
    grad(nullptr), // Luôn reset, sẽ được resolve sau
    gradId(other.gradId) // Sao chép ID Gradient
{
}

SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        Stroke* tempStroke = deepCopyStroke(other.stroke);
        delete this->stroke;
        this->stroke = tempStroke;

        this->fillColor = other.fillColor;
        this->fillOpacity = other.fillOpacity;

        this->gradId = other.gradId;
        this->grad = nullptr; // Luôn reset và resolve sau
    }
    return *this;
}

SVGStyle::~SVGStyle() {
    delete stroke;
}
CustomColor SVGStyle::getFillColor() const {
    return fillColor;
}
void SVGStyle::setFillColor(const CustomColor& color) {
    fillColor = color;
}
float SVGStyle::getFillOpacity() const {
    return fillOpacity;

}
void SVGStyle::setFillOpacity(const float& opacity) {
    fillOpacity = opacity;
}
Stroke* SVGStyle::getStroke() const {
    return stroke;
}

void SVGStyle::setStroke(Stroke* newStroke) {
    delete this->stroke;
    this->stroke = deepCopyStroke(newStroke);
}

Gdiplus::Color SVGStyle::getGdiFillColor() const {
    // 1. Chuyển đổi Opacity (0.0f - 1.0f) thành Alpha (0 - 255)
    // Giá trị phải được giới hạn trong khoảng [0, 255]
    BYTE alpha = static_cast<BYTE>(std::round(fillOpacity * 255.0f));

    // 2. Lấy màu RGB cơ bản (sử dụng toán tử chuyển đổi của CustomColor)
    Gdiplus::Color baseColor = fillColor;

    // 3. Tạo màu ARGB mới
    return Gdiplus::Color(
        alpha,
        baseColor.GetR(),
        baseColor.GetG(),
        baseColor.GetB()
    );
}
void SVGStyle::parse(const std::string& styleStr) {
    // p.parseStyle(*this, styleString);
    std::stringstream ss(styleStr);
    std::string item;
    while (getline(ss, item, ';')) {
        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.empty()) continue;

        size_t pos = item.find(':');
        if (pos == std::string::npos) continue;

        std::string key = item.substr(0, pos);
        std::string value = item.substr(pos + 1);

        if (key == "fill") this->setFillColor(CustomColor::fromStringToCustomColor(value));
        else if (key == "fill-opacity") this->setFillOpacity(stof(value));
        else if (key.find("stroke") != std::string::npos) {
            Stroke* stroke = this->getStroke();
            // parseStroke(stroke, item);
            std::string s = styleStr;
            s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
            size_t pos = s.find(':');
            if (pos == std::string::npos) return;
            std::string key = s.substr(0, pos);
            std::string value = s.substr(pos + 1);
            if (key == "stroke") stroke->strokeColor = CustomColor::fromStringToCustomColor(value);
            else if (key == "stroke-width") stroke->strokeWidth = stof(value);
            else if (key == "stroke-opacity") stroke->strokeOpacity = stof(value);

            this->setStroke(stroke);
        }
    }
}

const std::string& SVGStyle::getGradId() const {
    return gradId;
}
const SVGGradient* SVGStyle::getGrad() const {
    return grad;
}
void SVGStyle::setGradId(const std::string& id) {
    this->gradId = id;
}

void SVGStyle::setGrad(SVGGradient* ptr) {
    this->grad = ptr;
}

// Hàm kiểm tra: Có Gradient cần fill và đã được resolve chưa?
bool SVGStyle::hasGradientFill() const {
    return grad != nullptr;
}
// void SVGStyle::render(SVGRenderer& r) const {
//     r.renderStyle(*this);
// }  
// void transform(Matrix*);         

void SVGStyle::resolveGradient(const SVGDocumentContext& context) {
    if (!gradId.empty() && grad == nullptr) {
        std::string id = gradId;
        if (id.size() > 0 && id[0] == '#') id = id.substr(1);

        const SVGGradient* gradient = context.getGradientById(id);

        if (gradient) {
            // SỬA TẠI ĐÂY:
            // 1. Bảo gradient tự đi tìm màu từ xlink:href nếu nó chưa có stops
            gradient->resolveReference(context);

            // 2. Gán trực tiếp vì 'gradient' bây giờ đã chứa dữ liệu stops cần thiết
            this->grad = const_cast<SVGGradient*>(gradient);
        }
        else {
            std::cerr << "Warning: Gradient ID #" << id << " not found.\n";
            this->gradId = "";
            this->grad = nullptr;
        }
    }
}