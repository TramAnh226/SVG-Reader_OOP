#pragma once
#include <string>
#include "SVGStyle.h"
// #include "../../SVGParser/SVGParser/include/SVGParser.h"
// #include "../../SVGRenderer/SVGRenderer.h"

class SVGRenderer;
namespace Gdiplus { class Graphics; }
namespace tinyxml2 { class XMLElement; }

class SVGElement {
protected: 
	std::string tag_name;
	std::string id;
	SVGStyle style;
public: 
	SVGElement();
	SVGElement(const std::string&, const std::string&, const SVGStyle&);
	SVGElement(const SVGElement&) = delete;
	SVGElement& operator=(const SVGElement&) = delete;
	~SVGElement();
	virtual SVGElement* clone() const = 0;

	std::string getTagName() const;
	void setTagName(const std::string&);
	std::string getId() const;
	void setId(const std::string&);
	SVGStyle getSVGStyle() const;
	void setSVGStyle(const SVGStyle&);

	// polymorphism and separation of concerns
	virtual void parse(tinyxml2::XMLElement*);
	virtual void render(SVGRenderer&, Gdiplus::Graphics&) const;
	// virtual void transform(Matrix* m) = 0;
};
