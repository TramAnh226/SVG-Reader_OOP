#pragma once
#include "Point.h"
#include "SVGElement.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRender.h"

class SVGText : public SVGElement {
private: 
	float font_size;
	Point start;
	std::string content;
public: 
	SVGText();
	SVGText(float, const Point&, const std::string&);
	SVGText(const SVGText&);
	SVGText& operator=(const SVGText&);
	~SVGText() override = default;

	float getFontSize() const;
	void setFontSize(float);
	Point getStart() const;
	void setStart(const Point&);
	std::string getContent() const;
	void setContent(const std::string&);
	
	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};