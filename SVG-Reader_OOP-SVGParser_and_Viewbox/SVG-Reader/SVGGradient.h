//#pragma once
//#include "tinyxml2.h"
//#include "SVGStop.h"
////#include "SVGParser.h"
//#include <string>
//#include <vector>
//
//class SVGParser;
//class SVGDocumentContext;
//
//class SVGGradient {
//protected:
//	std::string gradientID; // id de dinh danh trong GradientMap
//	std::vector<SVGStop> stopArray;
//	std::string gradientUnits;
//	std::string spreadMethod;
//	std::string transform;
//	std::string hrefID;
//public:
//	SVGGradient();
//	SVGGradient(const SVGGradient& other); 
//	SVGGradient& operator=(const SVGGradient& other);
//	virtual ~SVGGradient() = default;
//
//	const std::string& getGradientID() const;
//	const std::vector<SVGStop>& getStopArray() const;
//	const std::string& getGradientUnits() const;
//	const std::string& getSpreadMethod() const;
//	const std::string& getTransform() const;
//	const std::string& getHrefID() const;
//
//	void setGradientID(const std::string&);
//	void setStopArray(const std::vector<SVGStop>&);
//	void setGradientUnits(const std::string&);
//	void setSpreadMethod(const std::string&);
//	void setTransform(const std::string&);
//	void setHrefID(const std::string&);
//
//	void addStop(const SVGStop&);
//	virtual SVGGradient* clone() const = 0;
//	void clearStops();
//
//	virtual void parse(SVGParser&, tinyxml2::XMLElement*);
//
//	bool isReferencing() const; // Kiểm tra xem có đang tham chiếu không
//
//	virtual const SVGGradient* resolveReference(const SVGDocumentContext& context) const;
//};

#pragma once
#include "tinyxml2.h"
#include "SVGStop.h"
//#include "SVGParser.h"
#include <string>
#include <vector>
#include <unordered_set>

class SVGParser;
class SVGDocumentContext;

class SVGGradient {
protected:
	std::string gradientID; // id de dinh danh trong GradientMap
	std::string gradientUnits;
	std::string spreadMethod;
	Gdiplus::Matrix* gradientTransform;
	std::string hrefID;

	mutable std::vector<SVGStop> stopArray;
public:
	SVGGradient();
	SVGGradient(const SVGGradient& other);
	SVGGradient& operator=(const SVGGradient& other);
	virtual ~SVGGradient();

	const std::string& getGradientID() const;
	const std::vector<SVGStop>& getStopArray() const;
	const std::string& getGradientUnits() const;
	const std::string& getSpreadMethod() const;
	const Gdiplus::Matrix* getGradientTransform() const;
	const std::string& getHrefID() const;

	void setGradientID(const std::string&);
	void setStopArray(const std::vector<SVGStop>&);
	void setGradientUnits(const std::string&);
	void setSpreadMethod(const std::string&);
	void setGradientTransform(Gdiplus::Matrix* matrix);
	void setHrefID(const std::string&);

	void addStop(const SVGStop&);
	virtual SVGGradient* clone() const = 0;
	void clearStops();

	virtual void parse(SVGParser&, tinyxml2::XMLElement*);

	bool isReferencing() const; // Kiểm tra xem có đang tham chiếu không

	/*virtual const SVGGradient* resolveReference(const SVGDocumentContext& context) const;*/
	void resolveReference(const SVGDocumentContext& context) const;
};