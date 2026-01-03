#ifndef SVGELEMENT_H
#define SVGELEMENT_H

#include <string>
#include "SVGStyle.h"

class SVGParser;
class SVGRenderer;
class SVGDocumentContext;

class SVGElement {
protected:
	std::string tag_name;
	std::string id;
	SVGStyle style;
public:

	SVGElement();
	SVGElement(const std::string&, const std::string&, const SVGStyle&);
	SVGElement(const SVGElement&);
	SVGElement& operator=(const SVGElement&);
	
	virtual ~SVGElement() = default;
	
	virtual SVGElement* clone() const = 0;

	std::string getTagName() const;
	void setTagName(const std::string&);
	
	std::string getId() const;
	void setId(const std::string&);
	
	SVGStyle& getSVGStyle();
	const SVGStyle& getSVGStyle() const;
	void setSVGStyle(const SVGStyle&);

	// polymorphism and separation of concerns
	virtual void parse(SVGParser&, tinyxml2::XMLElement*);
	virtual void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const = 0;

	virtual void resolve(const SVGDocumentContext& context);
};
#endif