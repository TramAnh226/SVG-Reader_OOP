#pragma once
#include <string>
#include "SVGStyle.h"
#include "SVGTransform.h"

class SVGParser;
class SVGRenderer;
class SVGDocumentContext;

class SVGElement {
protected:
	//public:
	std::string tag_name;
	std::string id;
	SVGStyle style;

	//Gdiplus::Matrix* transformMatrix;

	//SVGTransform transform;
public:

	SVGElement();
	SVGElement(const std::string&, const std::string&, const SVGStyle&);
	SVGElement(const SVGElement&);
	SVGElement& operator=(const SVGElement&);

	//virtual ~SVGElement() {
	//	if (transformMatrix) {
	//		delete transformMatrix;
	//		transformMatrix = nullptr;
	//	}
	//}
	
	virtual ~SVGElement() = default;

	virtual SVGElement* clone() const = 0;
	//const Gdiplus::Matrix* getTransformMatrix() const {
	//	return this->transformMatrix;
	//}
	std::string getTagName() const;
	void setTagName(const std::string&);
	std::string getId() const;
	void setId(const std::string&);
	//SVGStyle getSVGStyle() const;
	SVGStyle& getSVGStyle();
	const SVGStyle& getSVGStyle() const;
	void setSVGStyle(const SVGStyle&);
	/*void setTransformMatrix(Gdiplus::Matrix t) {
		this->transformMatrix = t;
	}*/

	//void setTransformMatrix(const Gdiplus::Matrix&);

	//void setTransformMatrix(Gdiplus::Matrix* newMatrix) {
	//	if (transformMatrix != nullptr) {
	//		delete transformMatrix;
	//	}
	//	transformMatrix = newMatrix;
	//}


	//const SVGTransform& getTransform() const { return transform; }
	//SVGTransform& getTransform() { return transform; }
	//void setTransform(const SVGTransform& t) { transform = t; }



	// polymorphism and separation of concerns
	//virtual void parse(tinyxml2::XMLElement*) = 0;
	virtual void parse(SVGParser&, tinyxml2::XMLElement*);
	virtual void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const = 0;
	// virtual void transform(Matrix* m) = 0;

	virtual void printDebugAttributes(std::ostream&, int) const;
	void printStyleDebug(std::ostream& os) const;

	virtual void resolve(const SVGDocumentContext& context);




	// thiết lập transform
	//virtual void setTransform(const std::string& svgTransformString);
	// parse transform: xây một cái vector để lưu các thông số của Matrix transformMatrix
	//std::vector<TransformCommand> parseTransform(const std::string& svgTransform);
	// áp dụng transform cho các SVGElement con
	//void applyTransform(const std::vector<TransformCommand>& commands);
};
