//#pragma once
//#include <string>
//#include "SVGStyle.h"
//#include "SVGTransform.h"
//
//class SVGParser;
//class SVGRenderer;
//class SVGDocumentContext;
//
////namespace Gdiplus { class Graphics; }
////namespace tinyxml2 {
////	class XMLDocument;
////	class XMLElement;
////	class XMLAttribute; 
////	class XMLNode;
////}
//class SVGElement {
//protected:
////public:
//	std::string tag_name;
//	std::string id;
//	SVGStyle style;
//	// tại sao để public thì mới rect trong SVGRenderer.cpp mới access được (đáng lẽ protected cũng được chứ)
//
//	// nên thêm transform nào SVGElement thay vì SVGStyle
//	// vì đây là thuộc tính làm thay đổi định dạng của các SVGElement con
//	// khác biệt về tính chất so với các thuộc tính trong SVGStyle hiện tại
//	SVGTransform transformMatrix;
//public:
//
//	// cấu trúc dữ liệu trung gian hỗ trợ cho transfromMatrix
//	//struct TransformCommand {
//	//	std::string name;             
//	//	std::vector<float> parameters; 
//	//};
//
//	SVGElement();
//	SVGElement(const std::string&, const std::string&, const SVGStyle&);
//	SVGElement(const SVGElement&) = default;
//	SVGElement& operator=(const SVGElement&) = default;
//	//virtual ~SVGElement();
//	virtual ~SVGElement() = default;
//	virtual SVGElement* clone() const = 0;
//	/*const SVGTransform* getTransformMatrix() const {
//		return this->transformMatrix;
//	}*/
//	const SVGTransform& getTransformMatrix() const { return transformMatrix; }
//	SVGTransform& getTransformMatrix() { return transformMatrix; }
//	void setTransformMatrix(const SVGTransform& t) { 
//		transformMatrix = t;
//	}
//
//	std::string getTagName() const;
//	void setTagName(const std::string&);
//	std::string getId() const;
//	void setId(const std::string&);
//	//SVGStyle getSVGStyle() const;
//	SVGStyle& getSVGStyle();
//	const SVGStyle& getSVGStyle() const;
//	void setSVGStyle(const SVGStyle&);
//
//	// polymorphism and separation of concerns
//	//virtual void parse(tinyxml2::XMLElement*) = 0;
//	virtual void parse(SVGParser&, tinyxml2::XMLElement*);
//	virtual void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const = 0;
//	// virtual void transform(Matrix* m) = 0;
//
//	virtual void printDebugAttributes(std::ostream&, int) const;
//	virtual void printStyleDebug(std::ostream& os) const;
//
//	virtual void resolve(const SVGDocumentContext& context);
//
//
//
//
//	// thiết lập transform
//	//virtual void setTransform(const std::string& svgTransformString);
//	// parse transform: xây một cái vector để lưu các thông số của Matrix transformMatrix
//	//std::vector<TransformCommand> parseTransform(const std::string& svgTransform);
//	// áp dụng transform cho các SVGElement con
//	//void applyTransform(const std::vector<TransformCommand>& commands);
//};
#pragma once
#include <string>
#include "SVGStyle.h"
#include "SVGTransform.h"

class SVGParser;
class SVGRenderer;
class SVGDocumentContext;

//namespace Gdiplus { class Graphics; }
//namespace tinyxml2 {
//	class XMLDocument;
//	class XMLElement;
//	class XMLAttribute; 
//	class XMLNode;
//}
class SVGElement {
protected:
	//public:
	std::string tag_name;
	std::string id;
	SVGStyle style;
	// tại sao để public thì mới rect trong SVGRenderer.cpp mới access được (đáng lẽ protected cũng được chứ)

	// nên thêm transform nào SVGElement thay vì SVGStyle
	// vì đây là thuộc tính làm thay đổi định dạng của các SVGElement con
	// khác biệt về tính chất so với các thuộc tính trong SVGStyle hiện tại
	Gdiplus::Matrix* transformMatrix;
public:

	// cấu trúc dữ liệu trung gian hỗ trợ cho transfromMatrix
	//struct TransformCommand {
	//	std::string name;             
	//	std::vector<float> parameters; 
	//};
	//Gdiplus::Matrix* transformMatrix;

	SVGElement();
	SVGElement(const std::string&, const std::string&, const SVGStyle&);
	SVGElement(const SVGElement&);
	SVGElement& operator=(const SVGElement&);
	//virtual ~SVGElement();
	virtual ~SVGElement() {
		if (transformMatrix) {
			delete transformMatrix;
			transformMatrix = nullptr;
		}
	}
	//virtual ~SVGElement() = default;
	virtual SVGElement* clone() const = 0;
	const Gdiplus::Matrix* getTransformMatrix() const {
		return this->transformMatrix;
	}
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
	void setTransformMatrix(Gdiplus::Matrix* newMatrix) {

		// BƯỚC 1: Dọn dẹp đối tượng ma trận cũ (nếu nó tồn tại)
		// Nếu transformMatrix đang trỏ đến một ma trận đã cấp phát bằng 'new', 
		// chúng ta phải 'delete' nó để giải phóng bộ nhớ cũ.
		if (transformMatrix != nullptr) {
			delete transformMatrix;
		}

		// BƯỚC 2: Gán con trỏ ma trận mới
		// Con trỏ mới (newMatrix) giờ đây được sở hữu bởi SVGElement.
		// Giả định rằng newMatrix đã được cấp phát trên heap (ví dụ: bằng new Gdiplus::Matrix() hoặc Clone()).
		transformMatrix = newMatrix;
	}
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
