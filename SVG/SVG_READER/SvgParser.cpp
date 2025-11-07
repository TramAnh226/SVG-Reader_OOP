#include "SvgParser.h"
#include <fstream>
#include <sstream>
#include <iostream>


using namespace rapidxml;

// ---------------------------
// Đọc toàn bộ file .svg
// ---------------------------
bool SVGParser::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Khong the mo file: " << filename << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    xml_document<> doc;
    try {
        doc.parse<0>(&content[0]);
    }
    catch (parse_error& e) {
        std::cerr << "Loi parse XML: " << e.what() << std::endl;
        return false;
    }

    xml_node<>* svgNode = doc.first_node("svg");
    if (!svgNode) {
        std::cerr << "Khong tim thay the <svg>" << std::endl;
        return false;
    }

    // Duyệt tất cả các node con trong <svg>
    for (xml_node<>* node = svgNode->first_node(); node; node = node->next_sibling()) {
        parseElement(node);
    }

    return true;
}


// ---------------------------
// Phân loại node và tạo Shape
// ---------------------------
void SVGParser::parseElement(xml_node<>* node) {
    Shape* shape = createShapeFromNode(node);
    if (shape) shapes.push_back(shape);
}


// ---------------------------
// Tạo đối tượng Shape cụ thể
// ---------------------------
Shape* SVGParser::createShapeFromNode(xml_node<>* node) {
    std::string name = node->name();

    if (name == "rect") {
        float x = std::stof(node->first_attribute("x")->value());
        float y = std::stof(node->first_attribute("y")->value());
        float width = std::stof(node->first_attribute("width")->value());
        float height = std::stof(node->first_attribute("height")->value());

        return new Ellipse(x + width / 2, y + height / 2, width / 2, height / 2,
            Color(255, 0, 255, 0), Color(255, 0, 0, 0), 2);
    }
    else if (name == "circle") {
        float cx = std::stof(node->first_attribute("cx")->value());
        float cy = std::stof(node->first_attribute("cy")->value());
        float r = std::stof(node->first_attribute("r")->value());

        return new Ellipse(cx, cy, r, r, Color(255, 0, 0, 255), Color(255, 0, 0, 0), 2);
    }
    else if (name == "line") {
        float x1 = std::stof(node->first_attribute("x1")->value());
        float y1 = std::stof(node->first_attribute("y1")->value());
        float x2 = std::stof(node->first_attribute("x2")->value());
        float y2 = std::stof(node->first_attribute("y2")->value());

        return new Line(x1, y1, x2, y2, Color(255, 0, 0, 0), 2);
    }
    else if (name == "text") {
        float x = std::stof(node->first_attribute("x")->value());
        float y = std::stof(node->first_attribute("y")->value());
        std::wstring text;
        std::string content = node->value();
        text.assign(content.begin(), content.end());
        return new Text(text, x, y, Color(255, 0, 0, 255), 20);
    }

    // nếu không nhận ra loại thẻ => bỏ qua
    return nullptr;
}
