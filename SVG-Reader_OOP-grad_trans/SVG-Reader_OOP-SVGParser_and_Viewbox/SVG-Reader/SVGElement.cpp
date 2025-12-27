#include "tinyxml2.h"
#include "Stroke.h"
#include "SVGElement.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include <iostream>

SVGElement::SVGElement() {
    this->tag_name = "";
    this->id = "";
    this->style = SVGStyle();

    //this->transformMatrix = new Gdiplus::Matrix();
}
SVGElement::SVGElement(const std::string& tag, const std::string& identifier, const SVGStyle& svg_style) {
    this->tag_name = tag;
    this->id = identifier;
    this->style = svg_style;

    //this->transformMatrix = new Gdiplus::Matrix();
}

// it is unnescessary write copy constructor, copy assignment and destructor 
// because they are delete and default => force us: override at derived classes

// 1. Copy Constructor
SVGElement::SVGElement(const SVGElement& other)
    : tag_name(other.tag_name), id(other.id), style(other.style)
{
    //// Sử dụng Clone() để sao chép ma trận an toàn
    //if (other.transformMatrix) {
    //    this->transformMatrix = other.transformMatrix->Clone();
    //}
    //else {
    //    this->transformMatrix = new Gdiplus::Matrix();
    //}
}
// 2. Copy Assignment Operator
SVGElement& SVGElement::operator=(const SVGElement& other) {
    if (this != &other) {
        // Sao chép thuộc tính đơn giản và style
        this->tag_name = other.tag_name;
        this->id = other.id;
        this->style = other.style;

        //// Xóa ma trận cũ
        //delete this->transformMatrix;

        //// Clone ma trận mới
        //if (other.transformMatrix) {
        //    this->transformMatrix = other.transformMatrix->Clone();
        //}
        //else {
        //    this->transformMatrix = new Gdiplus::Matrix();
        //}
    }
    return *this;
}

//SVGElement::~SVGElement() {
//    delete this->transformMatrix;
//}
//const Gdiplus::Matrix* SVGElement::getTransformMatrix() const {
//    return this->transformMatrix;
//}
//void SVGElement::setTransformMatrix(const Gdiplus::Matrix& matrix) {
//    // 1. Kiểm tra xem đã có đối tượng ma trận chưa
//    if (transformMatrix == nullptr) {
//        // Nếu chưa, cấp phát bộ nhớ mới
//        transformMatrix = new Gdiplus::Matrix(matrix);
//    }
//    else {
//        // Nếu đã có, chỉ cần sao chép giá trị từ đối tượng được truyền vào
//        // Trong GDI+, cách đơn giản nhất để sao chép là:
//
//        // 1. Hủy đối tượng cũ
//        delete transformMatrix;
//
//        // 2. Cấp phát lại với giá trị mới
//        // Lưu ý: Gdiplus::Matrix không có toán tử gán trực tiếp (=) hiệu quả, 
//        // nên việc xóa và cấp phát lại là cần thiết nếu bạn muốn đảm bảo 
//        // không có lỗi xảy ra.
//        transformMatrix = new Gdiplus::Matrix(matrix);
//
//        /* Hoặc, cách tối ưu hơn (nhưng yêu cầu Gdiplus::Matrix hỗ trợ sao chép):
//        * Nếu bạn biết Gdiplus::Matrix hỗ trợ toán tử gán (=) hoặc hàm Set:
//        * *transformMatrix = matrix;
//        * Hoặc: transformMatrix->SetElements(...) (phức tạp hơn)
//        */
//    }
//}
//void SVGElement::setTransformMatrix(const Gdiplus::Matrix& matrix) {
//    // 1. Nếu đã có ma trận cũ, phải hủy nó trước để tránh rò rỉ bộ nhớ (memory leak)
//    if (transformMatrix != nullptr) {
//        delete transformMatrix;
//        transformMatrix = nullptr;
//    }
//
//    // 2. Sử dụng hàm Clone() để tạo một bản sao hợp lệ trên Heap
//    // Hàm Clone() trả về một con trỏ tới đối tượng Gdiplus::Matrix mới.
//    transformMatrix = matrix.Clone();
//
//    // Lưu ý: Clone() trả về Gdiplus::Matrix* nên không cần dùng 'new'
//}
std::string SVGElement::getTagName() const {
    return tag_name;
}
void SVGElement::setTagName(const std::string& tag) {
    this->tag_name = tag;
}
std::string SVGElement::getId() const {
    return id;
}
void SVGElement::setId(const std::string& identifier) {
    this->id = identifier;
}
//SVGStyle SVGElement::getSVGStyle() const {
//    return style;
//}
// Phiên bản không const (để sửa đổi)
SVGStyle& SVGElement::getSVGStyle() {
    return style;
}
// Phiên bản const (cho các hàm const)
const SVGStyle& SVGElement::getSVGStyle() const {
    return style;
}
void SVGElement::setSVGStyle(const SVGStyle& svg_style) {
    this->style = svg_style;
}

// polymorphism and separation of concerns
//void SVGElement::parse(tinyxml2::XMLElement* node) {
//    // Gọi hàm phân tích cú pháp thuộc tính chung (ID, STYLE, FILL...)
//    // Dạng 1: Nếu parseAttributes là hàm static trong SVGParser:
//    // SVGParser::parseAttributes(node, this); 
//
//    // Dạng 2: Nếu bạn dùng một hàm thành viên (non-static) của SVGParser
//    // SVGParser parser; 
//    // parser.parseAttributes(node, this); 
//
//    // Giả sử parseAttributes là static và bạn đã include SVGParser.h:
//    extern SVGParser g_parser; // Nếu g_parser là biến toàn cục
//    g_parser.parseAttributes(node, this);
//}
void SVGElement::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    parser.parseAttributes(xmlNode, this);
}

// ... (Các hàm khác)
// void SVGElement::render(SVGRenderer&, Gdiplus::Graphics&) const {}     
// virtual void SVGElement::transform(Matrix* m) = 0; // pure virtual function
// Trong SVGElement.cpp (thêm hàm này)

// Yêu cầu include <iostream> và <sstream>
// Thêm vào cuối SVGElement.cpp

void SVGElement::printDebugAttributes(std::ostream& os, int idx) const {
    os << idx << "\n--- DEBUG ELEMENT ---" << std::endl;
    idx++;
    os << "Tag: " << this->tag_name << ", ID: " << this->id << std::endl;

    // Gọi hàm tiện ích để in Style/Stroke
    this->printStyleDebug(os);
}
void SVGElement::printStyleDebug(std::ostream& os) const {
    os << "Fill: " << this->style.getFillColor().fromCustomColorToString() << std::endl;
    os << "Fill Opacity: " << this->style.getFillOpacity() << std::endl;

    Stroke* strokeObj = this->style.getStroke();
    if (strokeObj != nullptr) {
        os << "  --- STROKE INFO ---" << std::endl;
        os << "  Width: " << strokeObj->strokeWidth << std::endl;
        os << "  Opacity: " << strokeObj->strokeOpacity << std::endl;
        os << "  Color (RGB): " << strokeObj->strokeColor.fromCustomColorToString() << std::endl;
        os << "---------------------" << std::endl;
    }
    else {
        os << "Stroke: NONE" << std::endl;
    }

    os << "GradId: " << this->style.getGradId() << std::endl;

    const SVGGradient* gradObj = this->style.getGrad();
    if (gradObj) {
        os << "  --- GRADIENT INFO ---" << std::endl;
        os << "  ID: " << gradObj->getGradientID() << std::endl;
        os << "  Units: " << gradObj->getGradientUnits() << std::endl; // Quan trọng để check userSpaceOnUse

        // THÊM VÒNG LẶP IN STOPS TẠI ĐÂY
        const std::vector<SVGStop>& stops = gradObj->getStopArray();
        os << "  Stops Count: " << stops.size() << std::endl;

        for (size_t i = 0; i < stops.size(); ++i) {
            os << "    Stop [" << i << "]: Offset = " << stops[i].getOffset()
                << ", Color = " << stops[i].getStopColor().fromCustomColorToString()
                << ", Opacity = " << stops[i].getStopOpacity() << std::endl;
        }
        os << "  ---------------------" << std::endl;
    }
}

//std::vector<SVGElement::TransformCommand> SVGElement::parseTransform(const std::string& svgTransform) {
//    std::vector<TransformCommand> commands;
//    if (svgTransform.empty()) return commands;
//
//    std::string::size_type currentPos = 0;
//
//    // Hàm lambda để bỏ qua khoảng trắng
//    auto skipWhitespace = [&]() {
//        currentPos = svgTransform.find_first_not_of(" \t\n\r", currentPos);
//        if (currentPos == std::string::npos) currentPos = svgTransform.length();
//        };
//
//    while (currentPos < svgTransform.length()) {
//        skipWhitespace();
//        if (currentPos == svgTransform.length()) break;
//
//        // 1. Tìm tên lệnh
//        std::string::size_type openParen = svgTransform.find('(', currentPos);
//        if (openParen == std::string::npos) break;
//
//        std::string commandName = svgTransform.substr(currentPos, openParen - currentPos);
//
//        // Loại bỏ khoảng trắng/tab thừa trong tên lệnh (nếu có)
//        // (Đây là bước đơn giản hóa, có thể cần regex/trim phức tạp hơn cho production)
//
//        TransformCommand cmd;
//        cmd.name = commandName;
//        currentPos = openParen + 1; // Nhảy qua '('
//
//        // 2. Trích xuất các tham số
//        std::string::size_type closeParen = svgTransform.find(')', currentPos);
//        if (closeParen == std::string::npos) break;
//
//        std::string paramsString = svgTransform.substr(currentPos, closeParen - currentPos);
//        // Chuẩn hóa: Thay thế dấu phẩy (,) bằng khoảng trắng để dễ phân tích số
//        for (char& c : paramsString) {
//            if (c == ',' || c == '\t') c = ' ';
//        }
//
//        std::stringstream ss(paramsString);
//        float value;
//        while (ss >> value) {
//            cmd.parameters.push_back(value);
//        }
//
//        commands.push_back(cmd);
//        currentPos = closeParen + 1; // Nhảy qua ')'
//    }
//    return commands;
//}
//void SVGElement::applyTransform(const std::vector<TransformCommand>& commands) {
//
//    // 1. Reset ma trận về ma trận đơn vị
//    this->transformMatrix->Reset();
//
//    // 2. Áp dụng các lệnh theo thứ tự:
//    for (const auto& cmd : commands) {
//        if (cmd.name == "translate") {
//            if (cmd.parameters.size() >= 1) {
//                float x = cmd.parameters[0];
//                float y = (cmd.parameters.size() >= 2) ? cmd.parameters[1] : 0.0f;
//                this->transformMatrix->Translate(x, y);
//            }
//        }
//        else if (cmd.name == "rotate") {
//            if (cmd.parameters.size() >= 1) {
//                float angle = cmd.parameters[0]; // GDI+ nhận góc bằng độ
//                this->transformMatrix->Rotate(angle);
//            }
//        }
//        else if (cmd.name == "scale") {
//            if (cmd.parameters.size() >= 1) {
//                float x = cmd.parameters[0];
//                // scale(d) nếu chỉ có 1 tham số, scale(x, y) nếu có 2
//                float y = (cmd.parameters.size() >= 2) ? cmd.parameters[1] : x;
//                this->transformMatrix->Scale(x, y);
//            }
//        }
//        // ... (Thêm matrix, skewX, skewY nếu cần)
//    }
//}
//void SVGElement::setTransform(const std::string& svgTransformString) {
//    if (svgTransformString.empty()) {
//        this->transformMatrix->Reset();
//        return;
//    }
//
//    // 1. Phân tích cú pháp chuỗi
//    std::vector<TransformCommand> commands = this->parseTransform(svgTransformString);
//
//    // 2. Áp dụng các lệnh vào ma trận của đối tượng
//    this->applyTransform(commands);
//}
// Trong SVGElement.cpp

// Xóa: std::vector<SVGElement::TransformCommand> SVGElement::parseTransform(...)
// Xóa: void SVGElement::applyTransform(...)

//void SVGElement::setTransform(const std::string& svgTransformString) {
//    using namespace Gdiplus;
//
//    if (svgTransformString.empty()) {
//        this->transformMatrix->Reset();
//        return;
//    }
//
//    // 1. Phân tích cú pháp chuỗi bằng lớp SVGTransform chuyên biệt
//    Gdiplus::Matrix t = SVGTransform::parse(svgTransformString);
//
//    // 2. Reset ma trận của đối tượng
//    this->transformMatrix->Reset();
//
//    // 3. Áp dụng các phép biến đổi đã phân tích vào ma trận GDI+
//    //t.applyToMatrix(*this->transformMatrix);
//}
//void SVGElement::setTransformMatrix(const Gdiplus::Matrix& matrix) {
//
//    // 1. Dọn dẹp đối tượng cũ (nếu có)
//    // Phải hủy đối tượng cũ trước để tránh rò rỉ bộ nhớ (memory leak)
//    if (transformMatrix != nullptr) {
//        delete transformMatrix;
//        transformMatrix = nullptr;
//    }
//
//    // 2. TẠO BẢN SAO HỢP LỆ bằng phương thức Clone()
//    // Clone() trả về một con trỏ Gdiplus::Matrix* mới được cấp phát trên heap.
//    // KHÔNG cần dùng 'new'.
//    transformMatrix = matrix.Clone();
//
//    // Lưu ý: Nếu Clone() thất bại, nó có thể trả về nullptr,
//    // nhưng trong hầu hết các trường hợp sử dụng cơ bản, nó sẽ thành công.
//}
//
// Đảm bảo Destructor của SVGElement cũng gọi delete:
//SVGElement::~SVGElement() {
//    if (transformMatrix) {
//        delete transformMatrix;
//        transformMatrix = nullptr;
//    }
//}
void SVGElement::resolve(const SVGDocumentContext& context) {
    // 1. Element này (ví dụ: Circle, Rect, Text) tự resolve Style của nó
    cout << "after resolve element: " << this->getTagName() << '\n';
    this->getSVGStyle().resolveGradient(context);

    // 2. Không cần gọi đệ quy vì element này không có con.
    // Lớp SVGGroup sẽ chịu trách nhiệm gọi đệ quy cho các con của nó.
}
