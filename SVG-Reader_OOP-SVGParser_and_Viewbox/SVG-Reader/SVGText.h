//#pragma once
//#include "SVGElement.h"
//#include "CustomPoint.h"
//
//class SVGText : public SVGElement {
//private:
//	float font_size;
//	CustomPoint start;
//	std::wstring content;
//public:
//	SVGText();
//	SVGText(float, const CustomPoint&, const std::wstring&);
//	SVGText(const SVGText&);
//	SVGText& operator=(const SVGText&);
//	~SVGText() override = default;
//	SVGElement* clone() const override;
//
//	float getFontSize() const;
//	void setFontSize(float);
//	CustomPoint getStart() const;
//	void setStart(const CustomPoint&);
//	std::wstring getContent() const;
//	void setContent(const std::wstring&);
//
//	//void parse(tinyxml2::XMLElement*);
//	void parse(SVGParser&, tinyxml2::XMLElement*) override;
//	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
//	// void transform(Matrix*) override;
//
//	void printDebugAttributes(std::ostream&, int) const;
//};


#pragma once
#include "SVGElement.h"
#include "CustomPoint.h"

// Các giá trị mặc định cho text-anchor
enum class TextAnchor {
	Start,
	Middle,
	End
};

// Các giá trị mặc định cho font-style
enum class FontStyles {
	Normal,
	Italic
	// oblique có thể coi là Italic trong GDI+
};

// Các giá trị mặc định cho font-weight
enum class FontWeight {
	Normal, // 400
	Bold    // 700
};
class SVGText : public SVGElement {
private:
	float font_size;
	CustomPoint start;
	std::wstring content;

	std::string font_family;

	TextAnchor text_anchor = TextAnchor::Start; // text-anchor
	FontStyles font_style = FontStyles::Normal;   // font-style
	FontWeight font_weight = FontWeight::Normal; // font-weight

	float dx = 0.0f; 
	float dy = 0.0f;
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

	std::string getFontFamily() const;
	void setFontFamily(const std::string&);

	// --- Getters và Setters MỚI ---
	TextAnchor getTextAnchor() const;
	void setTextAnchor(TextAnchor);

	FontStyles getFontStyle() const;
	void setFontStyle(FontStyles);

	FontWeight getFontWeight() const;
	void setFontWeight(FontWeight);

	float getDx() const { return dx; }
	void setDx(float val) { dx = val; }
	float getDy() const { return dy; }
	void setDy(float val) { dy = val; }

	//void parse(tinyxml2::XMLElement*);
	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
	// void transform(Matrix*) override;

	void printDebugAttributes(std::ostream&, int) const;
};