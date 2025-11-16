#pragma once
#include "CustomPoint.h"
#include "SVGElement.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"

class SVGText : public SVGElement {
private: 
	float font_size;
	CustomPoint start;
	std::string content;
public: 
	SVGText();
	SVGText(float, const CustomPoint&, const std::string&);
	SVGText(const SVGText&);
	SVGText& operator=(const SVGText&);
	~SVGText() override = default;
	SVGElement* clone() const override;
	
	float getFontSize() const;
	void setFontSize(float);
	CustomPoint getStart() const;
	void setStart(const CustomPoint&);
	std::string getContent() const;
	void setContent(const std::string&);
	
	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*) override;
};