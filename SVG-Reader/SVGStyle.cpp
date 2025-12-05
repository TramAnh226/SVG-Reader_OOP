#include "SVGStyle.h"
#include "Stroke.h" 
#include "CustomColor.h" 
#include <windows.h>
#include <gdiplus.h>
#include <iostream>

using namespace Gdiplus;

SVGStyle::SVGStyle() : fillOpacity(1.0f) {
    this->stroke = new Stroke;
}

SVGStyle::SVGStyle(CustomColor fillC, float fillO, Stroke* s)
    : fillColor(fillC), fillOpacity(fillO), stroke(s ? new Stroke(*s) : nullptr) {
}
SVGStyle::SVGStyle(const SVGStyle& other) {
    this->fillColor = other.fillColor;
    this->fillOpacity = other.fillOpacity;
    this->stroke = other.stroke ? new Stroke(*other.stroke) : nullptr; 
}
SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        delete this->stroke;
        this->fillColor = other.fillColor;
        this->fillOpacity = other.fillOpacity;
        this->stroke = other.stroke ? new Stroke(*other.stroke) : nullptr;
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
    if (stroke != nullptr) {
        delete stroke;
        stroke = nullptr; 
    }

    if (newStroke != nullptr) {
        stroke = new Stroke(*newStroke);
    }
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
    //std::stringstream ss(styleStr);
    //std::string item;
    //while (getline(ss, item, ';')) {
    //    item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
    //    if (item.empty()) continue;

    //    size_t pos = item.find(':');
    //    if (pos == std::string::npos) continue;

    //    std::string key = item.substr(0, pos);
    //    std::string value = item.substr(pos + 1);

    //    if (key == "fill") this->setFillColor(CustomColor::fromStringToCustomColor(value));
    //    else if (key == "fill-opacity") this->setFillOpacity(stof(value));
    //    else if (key.find("stroke") != std::string::npos) {
    //        Stroke* stroke = this->getStroke();
    //        // parseStroke(stroke, item);
    //        std::string s = styleStr;
    //        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    //        size_t pos = s.find(':');
    //        if (pos == std::string::npos) return;
    //        std::string key = s.substr(0, pos);
    //        std::string value = s.substr(pos + 1);
    //        if (key == "stroke") stroke->strokeColor = CustomColor::fromStringToCustomColor(value);
    //        else if (key == "stroke-width") stroke->strokeWidth = stof(value);
    //        else if (key == "stroke-opacity") stroke->strokeOpacity = stof(value);

    //        this->setStroke(stroke);
    //    }
    //}
    std::stringstream ss(styleStr);
    std::string item;

    // Sử dụng std::remove_if từ <algorithm>
    while (std::getline(ss, item, ';')) {
        // Loại bỏ khoảng trắng và kiểm tra chuỗi rỗng
        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.empty()) continue;

        // Tìm vị trí của dấu hai chấm (:)
        size_t pos = item.find(':');
        if (pos == std::string::npos) continue;

        std::string key = item.substr(0, pos);
        std::string value = item.substr(pos + 1);

        // 1. Xử lý Fill
        if (key == "fill") {
            this->setFillColor(CustomColor::fromStringToCustomColor(value));
        }
        else if (key == "fill-opacity") {
            try {
                this->setFillOpacity(std::stof(value));
            }
            catch (const std::exception& e) {
                std::cerr << "Style parsing error (fill-opacity): " << e.what() << std::endl;
            }
        }
        // 2. Xử lý Stroke (Chuyển trách nhiệm cho parseStroke)
        else if (key.find("stroke") != std::string::npos) {

            Stroke* currentStroke = this->getStroke();

            // Khởi tạo Stroke nếu chưa tồn tại (Logic này có vẻ đúng, nhưng nên dùng new/delete cho an toàn)
            if (!currentStroke) {
                Stroke* tempStroke = new Stroke(); // SỬA: Dùng heap cho an toàn
                this->setStroke(tempStroke);
                delete tempStroke; // setStroke đã copy, nên xóa bản gốc
                currentStroke = this->getStroke();
            }

            if (currentStroke) {
                // [SỬA ĐỔI QUAN TRỌNG NHẤT] Gửi key:value hiện tại ('item')
                currentStroke->parse(item); // <--- SỬA styleStr thành item
            }
        }
    }
}
// void SVGStyle::render(SVGRenderer& r) const {
//     r.renderStyle(*this);
// }  
// void transform(Matrix*);         