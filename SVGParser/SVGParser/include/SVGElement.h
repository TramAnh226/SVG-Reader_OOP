#pragma once
#include <string>
#include "SVGStyle.h"
#include "SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

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
	// virtual : obligatory (to delete derived object)
	virtual ~SVGElement() = default;

	std::string getTagName() const;
	void setTagName(const std::string&);
	std::string getId() const;
	void setId(const std::string&);
	SVGStyle getSVGStyle() const;
	void setSVGStyle(const SVGStyle&);

	// polymorphism and separation of concerns
	virtual void parse(SVGParser&) const = 0;
	virtual void render(SVGRenderer&) const = 0;
	// subsequent development
	// virtual void transform(Matrix* m) = 0;
};
