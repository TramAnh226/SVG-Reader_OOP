#include "SVGStyle.h"
// #include <sstream>
// #include <algorithm>
// #include <stdexcept>
// #include <cctype> 

// Default Constructor
SVGStyle::SVGStyle() 
    : fillColor(CustomColor()), 
      fillOpacity(1.0f), 
      stroke(Stroke()) 
{}

// Parameterized Constructor
SVGStyle::SVGStyle(CustomColor fill_color, float fill_opacity, const Stroke& svg_stroke)
    : fillColor(fill_color),
      fillOpacity(fill_opacity),
      stroke(svg_stroke)
{}

// Copy Constructor
SVGStyle::SVGStyle(const SVGStyle& other)
    : fillColor(other.fillColor),
      fillOpacity(other.fillOpacity),
      stroke(other.stroke)
{}

// Assignment Operator 
SVGStyle& SVGStyle::operator=(const SVGStyle& other) {
    if (this != &other) {
        this->fillColor = other.fillColor;
        this->fillOpacity = other.fillOpacity;
        this->stroke = other.stroke;
    }
    return *this;
}

// Destructor
SVGStyle::~SVGStyle() {}

CustomColor SVGStyle::getFillColor() const {
    return fillColor;
}
void SVGStyle::setFillColor(const CustomColor& color) {
    this->fillColor = color;
}

float SVGStyle::getFillOpacity() const {
    return fillOpacity;
}
void SVGStyle::setFillOpacity(const float& opacity) {
    this->fillOpacity = opacity;
}

// Getter trả về bản sao Stroke (vì hàm không trả về tham chiếu)
Stroke SVGStyle::getStroke() const {
    return stroke;
}
void SVGStyle::setStroke(const Stroke& svg_stroke) {
    this->stroke = svg_stroke;
}

void SVGStyle::parse(const std::string& styleStr) {
	std::stringstream ss(styleStr);
	std::string item;
    
	while (getline(ss, item, ';')) {
		
        // Xóa khoảng trắng ở đầu và cuối chuỗi (trimming)
        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
		if (item.empty()) continue;

        // Tìm dấu hai chấm (:) để tách Key và Value
		size_t pos = item.find(':');
		if (pos == std::string::npos) continue;

		std::string key = item.substr(0, pos);
		std::string value = item.substr(pos + 1);

        // 2. Phân tích các thuộc tính
		if (key == "fill") {
            // Sử dụng CustomColor::fromStringToCustomColor (Factory Method)
			this->setFillColor(CustomColor::fromStringToCustomColor(value));
		} 
        else if (key == "fill-opacity") {
			try {
                // Chuyển đổi chuỗi sang float
                this->setFillOpacity(stof(value));
            } catch (const std::exception& e) {
                // Xử lý lỗi chuyển đổi
            }
        }
        else if (key == "fill-rule") {
            if (value == "nonzero")
                fillRule = FillRule::NonZero;
            else if (value == "evenodd")
                fillRule = FillRule::EvenOdd;

        }
        // Ghi chú: Xử lý các thuộc tính stroke
        else if (key.find("stroke") != std::string::npos) {
            // Ở đây, chúng ta phải quyết định thuộc tính này là gì (stroke, stroke-width, stroke-opacity)
            
            // LƯU Ý: Do bạn không muốn gọi hàm parseStroke phức tạp từ đây,
            // chúng ta sẽ gán trực tiếp vào các thuộc tính của Stroke (vì Stroke là public struct)
            
            Stroke currentStroke = this->getStroke(); // Lấy bản sao hiện tại
            
            if (key == "stroke") {
                currentStroke.strokeColor = CustomColor::fromStringToCustomColor(value);
            } else if (key == "stroke-width") {
                try { currentStroke.strokeWidth = stof(value); } catch (...) {}
            } else if (key == "stroke-opacity") {
                try { currentStroke.strokeOpacity = stof(value); } catch (...) {}
            }
            if (name == "stroke-miterlimit") {
                stroke.miterlimit = std::stof(value);
            }
            this->setStroke(currentStroke); // Gán lại Stroke đã được cập nhật
        }
	}
}