#pragma once
#include "CustomColor.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"

struct Stroke {
public: 
	CustomColor strokeColor;
	float strokeWidth;
	float strokeOpacity;

	Stroke();
	Stroke(CustomColor, float, float);
	Stroke(const Stroke&);
	Stroke& operator=(const Stroke&);
	~Stroke();

	// there is no getter/setter because all attributes are public

<<<<<<< HEAD
	void parse(SVGParser&, const std::string&);
	// void render(SVGRenderer&);
=======
	void parse(SVGParser&);
	void render(SVGRenderer&) const;
>>>>>>> 0e98a1fd97ea090b926142e9f550a5fe7b856f9e
	// void transform(Matrix*);
};