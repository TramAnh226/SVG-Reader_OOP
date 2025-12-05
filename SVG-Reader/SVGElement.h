#pragma once
#include <string>
#include "SVGStyle.h"
//#include "SVGParser.h"
//#include "SVGRenderer.h"

class SVGParser;
class SVGRenderer;
namespace Gdiplus { class Graphics; }
namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
	class XMLAttribute; 
	class XMLNode;
}
class SVGElement {
protected:
//public:
	std::string tag_name;
	std::string id;
	SVGStyle style;
public:
	SVGElement();
	SVGElement(const std::string&, const std::string&, const SVGStyle&);
	SVGElement(const SVGElement&) = default;
	SVGElement& operator=(const SVGElement&) = default;
	virtual ~SVGElement() = default;
	virtual SVGElement* clone() const = 0;

	std::string getTagName() const;
	void setTagName(const std::string&);
	std::string getId() const;
	void setId(const std::string&);
	//SVGStyle getSVGStyle() const;
	SVGStyle& getSVGStyle();
	const SVGStyle& getSVGStyle() const;
	void setSVGStyle(const SVGStyle&);

	// polymorphism and separation of concerns
	//virtual void parse(tinyxml2::XMLElement*) = 0;
	virtual void parse(SVGParser&, tinyxml2::XMLElement*);
	virtual void render(SVGRenderer&, Gdiplus::Graphics&) const = 0;
	// virtual void transform(Matrix* m) = 0;

	virtual void printDebugAttributes(std::ostream&) const;
	void printStyleDebug(std::ostream& os) const;
};
