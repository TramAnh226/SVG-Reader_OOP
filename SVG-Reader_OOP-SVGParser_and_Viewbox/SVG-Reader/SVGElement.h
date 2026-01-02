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
	// tại sao để public thì mới rect trong SVGRenderer.cpp mới access được (đáng lẽ protected cũng được chứ)

	// nên thêm transform nào SVGElement thay vì SVGStyle
	// vì đây là thuộc tính làm thay đổi định dạng của các SVGElement con
	// khác biệt về tính chất so với các thuộc tính trong SVGStyle hiện tại
	Gdiplus::Matrix* transformMatrix;
public:

	SVGElement();
	SVGElement(const std::string&, const std::string&, const SVGStyle&);
	SVGElement(const SVGElement&);
	SVGElement& operator=(const SVGElement&);
	
	virtual ~SVGElement();
	//virtual ~SVGElement() = default;
	
	virtual SVGElement* clone() const = 0;
	const Gdiplus::Matrix* getTransformMatrix() const;

	std::string getTagName() const;
	void setTagName(const std::string&);
	
	std::string getId() const;
	void setId(const std::string&);
	
	//SVGStyle getSVGStyle() const;
	SVGStyle& getSVGStyle();
	const SVGStyle& getSVGStyle() const;
	void setSVGStyle(const SVGStyle&);

	void setTransformMatrix(Gdiplus::Matrix*); 

	// polymorphism and separation of concerns
	virtual void parse(SVGParser&, tinyxml2::XMLElement*);
	virtual void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const = 0;

	virtual void resolve(const SVGDocumentContext& context);
};
#endif