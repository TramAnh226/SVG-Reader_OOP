#pragma once
#include "SVGElement.h"
#include "CustomPoint.h"

class SVGText : public SVGElement {
private:
	float font_size;
	CustomPoint start;
	std::wstring content;
public:
	SVGText();
	SVGText(float, const CustomPoint&, const std::wstring&);
	SVGText(const SVGText&);
	SVGText& operator=(const SVGText&);
	~SVGText() override = default;
	SVGElement* clone() const override;

	float getFontSize() const;
	void setFontSize(float);
	CustomPoint getStart() const;
	void setStart(const CustomPoint&);
	std::wstring getContent() const;
	void setContent(const std::wstring&);

	//void parse(tinyxml2::XMLElement*);
	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*) override;

	void printDebugAttributes(std::ostream&) const;
};