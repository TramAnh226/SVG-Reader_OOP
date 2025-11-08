#pragma once
#include <string>
#include "SVGStyle.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

using namespace std;

class SvgElement {
private: 
	string tag_name;
	string id;
	SvgStyle style;
public: 
	SvgElement();
	SvgElement(const SvgElement&) = delete;
	SvgElement& operator=(const SvgElement&) = delete;
	// virtual : obligatory (to delete derived object)
	virtual ~SvgElement() = default;

	string getTagName();
	void setTagName(const string&);
	string getId();
	void setId(const string&);
	SvgStyle getSvgStyle();
	void setSvgStyle(const SvgStyle&);

	// polymorphism and separation of concerns
	virtual void parse(SVGParser&) const = 0;
	virtual void render(SVGRenderer& r) const = 0;
	// subsequent development
	// virtual void transform(Matrix* m) = 0;
};
