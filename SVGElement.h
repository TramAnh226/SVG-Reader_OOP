#pragma once
#include <string>
#include "SVGStyle.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

using namespace std;

class SVGElement {
private: 
	string tag_name;
	string id;
	SVGStyle style;
public: 
	SVGElement();
	SVGElement(const string&, const string&, const SVGStyle&);
	SVGElement(const SVGElement&) = delete;
	SVGElement& operator=(const SVGElement&) = delete;
	// virtual : obligatory (to delete derived object)
	virtual ~SVGElement() = default;

	string getTagName() const;
	void setTagName(const string&);
	string getId() const;
	void setId(const string&);
	SvgStyle getSVGStyle() const;
	void setSVGStyle(const SvgStyle&);

	// polymorphism and separation of concerns
	virtual void parse(SVGParser&) const = 0;
	virtual void render(SVGRenderer&) const = 0;
	// subsequent development
	// virtual void transform(Matrix* m) = 0;
};
