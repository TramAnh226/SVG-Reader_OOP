#pragma once
#ifndef SVG_ELEMENT_H
#define SVG_ELEMENT_H

#include <string>
#include "SvgStyle.h"
#include "SvgParser.h"
#include "SvgRenderer."

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
	virtual void parse(const map<string, string>& attrs) = 0;
	virtual void render(SvgRenderer& r) = 0;
	// subsequent development
	virtual void transform(Matrix* m) = 0;
};
#endif