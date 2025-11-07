#pragma once
#include <vector>
#include <string>
#include "Shape.h"
#include "rapidxml.hpp"

class SVGParser {
private:
    std::vector<Shape*> shapes;

public:
    bool loadFromFile(const std::string& filename);
    const std::vector<Shape*>& getShapes() const { return shapes; }

private:
    void parseElement(rapidxml::xml_node<>* node);
    Shape* createShapeFromNode(rapidxml::xml_node<>* node);
};
